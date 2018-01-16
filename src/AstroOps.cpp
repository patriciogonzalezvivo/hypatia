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
                                          double &_body_eclipticLon, double &_body_eclipticLat, double &_body_rad ) {
    // http://www.astrosurf.com/jephem/astro/ephemeris/et520transfo_en.htm

    Vector earth_eclipticLoc = _obs.getHeliocentricVector();
    Vector planet_eclipticLoc = Vector(_body_eclipticLon, _body_eclipticLat, _body_rad);

    // planet_eclipticLoc.x -= earth_eclipticLoc.x;
    // planet_eclipticLoc.y -= earth_eclipticLoc.y;
    // planet_eclipticLoc.z -= earth_eclipticLoc.z;

    planet_eclipticLoc -= earth_eclipticLoc;
    
    _body_eclipticLon = planet_eclipticLoc.getLongitudeRadians();
    _body_eclipticLat = planet_eclipticLoc.getLatitudeRadians();
    _body_rad = planet_eclipticLoc.getRadius();
}

/* compute parallactic angle given latitude, object dec and alt.
 * all angles in rads.
 * N.B. always return >= 0, caller must determine sign and degenerate cases at
 *   pole or zenith.
 */
// https://github.com/brandon-rhodes/pyephem/blob/592ecff661adb9c5cbed7437a23d705555d7ce57/libastro-3.7.7/parallactic.c#L15
double AstroOps::parallaticAngle(   Observer &_loc, 
                                    double _alt, double _dec ) {
    double ca = sin(_loc.getLatitudeRadians());
    double cb = sin(_dec);
    double sb = cos(_dec);
    double cc = sin(_alt);
    double sc = cos(_alt);
    double cpa;

    /* given three sides find an angle */
    if ( sb == 0.0 || sc == 0.0 ) {
        return (0.0);
    }
    cpa = (ca - cb * cc) / (sb * sc);
    if (cpa < -1.0) {
        cpa = -1.0;
    }
    if (cpa >  1) {
        cpa =  1.0;
    }
    return acos (cpa);
}
