/*****************************************************************************\
 * AstroOps.cpp
 *
 * AstroOps is a 'catch-all' class that performs misc. handy calculations
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#include "AstroOps.h"
#include "MathOps.h"
#include "TimeOps.h"

#include "Vector.h"

#include <math.h>
#include <string.h>

/*----------------------------------------------------------------------------
 * The obliquity formula (and all the magic numbers below) come from Meeus,
 * Astro Algorithms.
 *
 * Input t is time in julian centuries from 2000.
 * Valid range is the years -8000 to +12000 (t = -100 to 100).
 *
 * return value is mean obliquity (epsilon sub 0) in radians.
 */

double AstroOps::meanObliquity( double t ){
    double u, u0;
    static double t0 = 30000.;
    static double rval = 0.;
    static const double rvalStart =  23. * MathOps::SECONDS_PER_DEGREE +
                                     26. * MathOps::MINUTES_PER_DEGREE +
                                     21.448;
    
    static const int OBLIQ_COEFFS = 10;
    static const double coeffs[ OBLIQ_COEFFS ] = {
        -468093.,  -155.,  199925.,  -5138.,  -24967.,
        -3905.,    712.,   2787.,    579.,    245.
    };

    if( t0 != t ) {

        t0 = t;
        u = u0 = t / 100.;     // u is in julian 10000's of years
        rval = rvalStart;

        for( int i=0; i<OBLIQ_COEFFS; i++ ) {
            rval += u * coeffs[i] / 100.;
            u *= u0;
        }
        // convert from seconds to radians
        rval = MathOps::toRadians( rval / MathOps::SECONDS_PER_DEGREE );
    }
    return rval;

    //double T2 = t*t;
    //double T3 = T2*t;
    //return Astro::toRadians(23.4392917 - 0.0130041667*t - 0.00000016667*T2 + 0.0000005027778*T3);
}

//----------------------------------------------------------------------------

void AstroOps::eclipticToEquatorial ( Observer &_loc, double _lng, double _lat, double &_ra, double &_dec ) {
    double e = _loc.getObliquity();
    double sl = sin(_lng);
    double cl = cos(_lng);
    double sb = sin(_lat);
    double cb = cos(_lat);
    double tb = tan(_lat);
    double se = sin(e);
    double ce = cos(e);
    _ra = atan2((sl*ce - tb*se),(cl));
    _dec = asin(sb*ce+cb*se*sl);
    
    // Make sure RA is positive
    if (_ra < 0)
        _ra += MathOps::TAU;
}

void AstroOps::equatorialToHorizontal ( Observer &_obs, double _ra, double _dec, double &_alt, double &_az ) {
    // compute hour angle in degrees
    double ha = (MathOps::HD_PI*_obs.getLST()/12.) - _ra;
    double sd = sin(_dec);
    double sl = sin(_obs.getLatitudeRadians());
    double cl = cos(_obs.getLatitudeRadians());
    // compute altitude in radians
    _alt = asin(sd*sl + cos(_dec)*cl*cos(ha));
    // compute azimuth in radians
    // divide by zero error at poles or if alt = 90 deg (so we should've already limited to 89.9999)
    _az = acos((sd - sin(_alt)*sl)/(cos(_alt)*cl));
    // choose hemisphere
    if (sin(ha) > 0.0)
        _az = 2.*MathOps::HD_PI - _az;
}


void AstroOps::heliocentricToGeocentric ( Observer &_obs,
                                          double &_body_eclipticLon, double &_body_eclipticLat, double &_body_rad) {
    // http://www.astrosurf.com/jephem/astro/ephemeris/et520transfo_en.htm

    Vector earth_eclipticLoc = _obs.getEclipticHelioLocation();
    Vector planet_eclipticLoc = Vector(_body_eclipticLon, _body_eclipticLat, _body_rad);

    planet_eclipticLoc.x -= earth_eclipticLoc.x;
    planet_eclipticLoc.y -= earth_eclipticLoc.y;
    planet_eclipticLoc.z -= earth_eclipticLoc.z;
    
    _body_eclipticLon = planet_eclipticLoc.getLongitud();
    _body_eclipticLat = planet_eclipticLoc.getLatitud();
    _body_rad = planet_eclipticLoc.getRadius();
}


/* insure 0 <= *v < r.
 */
void range (double *v, double r) {
    *v -= r*floor(*v/r);
}

#define DCOS(x)     cos(MathOps::toRadians(x))
#define DSIN(x)     sin(MathOps::toRadians(x))
#define DASIN(x)    MathOps::toDegrees(asin(x))
#define DATAN2(y,x) MathOps::toDegrees(atan2((y),(x)))

/*
 * Copyright (c) 1990 by Craig Counterman. All rights reserved.
 *
 * This copy of the precess_hiprec() routine is, by permission of the
 * author, licensed under the same license as the PyEphem package in
 * which it is here included.
 *
 * Rigorous precession. From Astronomical Ephemeris 1989, p. B18
 *
 * 96-06-20 Hayo Hase <hase@wettzell.ifag.de>: theta_a corrected
 */
void AstroOps::precession( double _mjd1, double _mjd2, double &_ra, double &_dec ) {
    static double last_mjd1 = -213.432, last_from;
    static double last_mjd2 = -213.432, last_to;
    double zeta_A, z_A, theta_A;
    double T;
    double A, B, C;
    double alpha, delta;
    double from_equinox, to_equinox;
    double alpha2000, delta2000;

    /* convert mjds to years;
     * avoid the remarkably expensive calls to mjd_year()
     */
    if (last_mjd1 == _mjd1)
        from_equinox = last_from;
    else {
        from_equinox = TimeOps::MJDtoYears(_mjd1);
        last_mjd1 = _mjd1;
        last_from = from_equinox;
    }
    if (last_mjd2 == _mjd2)
        to_equinox = last_to;
    else {
        to_equinox = TimeOps::MJDtoYears(_mjd2);
        last_mjd2 = _mjd2;
        last_to = to_equinox;
    }

    /* convert coords in rads to degs */
    double alpha_in = MathOps::toDegrees(_ra);
    double delta_in = MathOps::toDegrees(_dec);

    /* precession progresses about 1 arc second in .047 years */
    /* From from_equinox to 2000.0 */
    if (fabs (from_equinox-2000.0) > .02) {
        T = (from_equinox - 2000.0)/100.0;
        zeta_A  = 0.6406161* T + 0.0000839* T*T + 0.0000050* T*T*T;
        z_A     = 0.6406161* T + 0.0003041* T*T + 0.0000051* T*T*T;
        theta_A = 0.5567530* T - 0.0001185* T*T - 0.0000116* T*T*T;

        A = DSIN(alpha_in - z_A) * DCOS(delta_in);
        B = DCOS(alpha_in - z_A) * DCOS(theta_A) * DCOS(delta_in)
          + DSIN(theta_A) * DSIN(delta_in);
        C = -DCOS(alpha_in - z_A) * DSIN(theta_A) * DCOS(delta_in)
          + DCOS(theta_A) * DSIN(delta_in);

        alpha2000 = DATAN2(A,B) - zeta_A;
        range (&alpha2000, 360.0);
        delta2000 = DASIN(C);
    } else {
        /* should get the same answer, but this could improve accruacy */
        alpha2000 = alpha_in;
        delta2000 = delta_in;
    };


    /* From 2000.0 to to_equinox */
    if (fabs (to_equinox - 2000.0) > .02) {
        T = (to_equinox - 2000.0)/100.0;
        zeta_A  = 0.6406161* T + 0.0000839* T*T + 0.0000050* T*T*T;
        z_A     = 0.6406161* T + 0.0003041* T*T + 0.0000051* T*T*T;
        theta_A = 0.5567530* T - 0.0001185* T*T - 0.0000116* T*T*T;

        A = DSIN(alpha2000 + zeta_A) * DCOS(delta2000);
        B = DCOS(alpha2000 + zeta_A) * DCOS(theta_A) * DCOS(delta2000)
          - DSIN(theta_A) * DSIN(delta2000);
        C = DCOS(alpha2000 + zeta_A) * DSIN(theta_A) * DCOS(delta2000)
          + DCOS(theta_A) * DSIN(delta2000);

        alpha = DATAN2(A,B) + z_A;
        range(&alpha, 360.0);
        delta = DASIN(C);
    } else {
        /* should get the same answer, but this could improve accruacy */
        alpha = alpha2000;
        delta = delta2000;
    };

    _ra = MathOps::toRadians(alpha);
    _dec = MathOps::toRadians(delta);
}

