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

const double AstroOps::SPEED_OF_LIGHT = 299792.458;

const double AstroOps::KM_TO_AU = 0.000000006684587;
const double AstroOps::AU_TO_KM = 1.495978707e+8;
const double AstroOps::AU_TO_M = (AstroOps::AU_TO_KM * 1000.);
const double AstroOps::AU_PER_DAY = (86400. * AstroOps::SPEED_OF_LIGHT / AstroOps::AU_TO_KM);

/*----------------------------------------------------------------------------
 * The obliquity formula (and all the magic numbers below) come from Meeus,
 * Astro Algorithms p 135.
 *
 * Input _jcentury is time in julian centuries from 2000.
 * Valid range is the years -8000 to +12000 (t = -100 to 100).
 *
 * return value is mean obliquity (epsilon sub 0) in radians.
 *
 * https://github.com/Bill-Gray/lunar/blob/master/obliquit.cpp
 */
double AstroOps::meanObliquity( double _jcentury ){
    double u, u0;
    unsigned i;
    const double obliquit_minus_100_cen = 24.232841111 * MathOps::DEGS_TO_RADS;
    const double obliquit_plus_100_cen =  22.611485556 * MathOps::DEGS_TO_RADS;
    static double j2000_obliquit = 23. * 3600. + 26. * 60. + 21.448;
    static double t0 = 30000., rval;
    static long coeffs[10] = { -468093L, -155L, 199925L, -5138L,
        -24967L, -3905L, 712L, 2787L, 579L, 245L };
    
    if( _jcentury == 0.)      /* common J2000 case;  don't do any math */
        return( j2000_obliquit * MathOps::ARCS_TO_RADS );
#ifndef CLIP_OBLIQUITY
    else if( _jcentury > 100.)      /* Diverges outside +/- 10 millennia,  */
        return( obliquit_plus_100_cen );
    else if( _jcentury < -100.)  /* so we might as well clip to that  */
        return( obliquit_minus_100_cen );
#endif
    
    if( t0 == _jcentury )    /* return previous answer */
        return( rval );
    
    rval = j2000_obliquit;
    t0 = _jcentury;
    u = u0 = _jcentury / 100.;     /* u is in julian 10000's of years */
    for( i = 0; i < 10; i++, u *= u0) {
        rval += u * (double)coeffs[i] / 100.;
    }
    rval *= MathOps::ARCS_TO_RADS;
    return( rval );
}

//----------------------------------------------------------------------------

// https://github.com/slowe/VirtualSky/blob/gh-pages/0.7.0/virtualsky.js#L1736
void AstroOps::eclipticToEquatorial ( Observer &_obs, double _lng, double _lat, double &_ra, double &_dec ) {
    double obliq = _obs.getObliquity();
    double sin_obliq = sin(obliq);
    double cos_obliq = cos(obliq);
    
    double sl = sin(_lng);
    double cl = cos(_lng);
    double sb = sin(_lat);
    double cb = cos(_lat);
    double tb = tan(_lat);

    _ra = atan2((sl * cos_obliq - tb * sin_obliq),(cl));
    _dec = asin(sb * cos_obliq + cb * sin_obliq * sl);
    
    // Make sure RA is positive
    if (_ra < 0)
        _ra += MathOps::TAU;
}

EqPoint AstroOps::eclipticToEquatorial ( Observer &_obs, const Vector &_ecliptic ) {
    double lng = _ecliptic.getLongitudeRadians();
    double lat = _ecliptic.getLatitudeRadians();
    double ra, dec;
    eclipticToEquatorial(_obs, lng, lat, ra, dec);
    EqPoint rta;
    rta.setInRadians(ra, dec);
    rta.compute(_obs);
    return rta;
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
    Vector planet_eclipticLoc = AstroOps::heliocentricToGeocentric(_obs, Vector(_body_eclipticLon, _body_eclipticLat, _body_rad));
    
//    Vector earth_eclipticLoc = _obs.getHeliocentricVector();
//    Vector planet_eclipticLoc = Vector(_body_eclipticLon, _body_eclipticLat, _body_rad);
//    planet_eclipticLoc -= earth_eclipticLoc;
    
    _body_eclipticLon = planet_eclipticLoc.getLongitudeRadians();
    _body_eclipticLat = planet_eclipticLoc.getLatitudeRadians();
    _body_rad = planet_eclipticLoc.getRadius();
}

Vector AstroOps::heliocentricToGeocentric( Observer &_obs, const Vector &_heliocentric ) {
//    Vector planet_eclipticLoc = _heliocentric - _obs.getHeliocentricVector();
//    return planet_eclipticLoc;
    return _heliocentric - _obs.getHeliocentricVector();
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
