#include "MathOps.h"

#include <math.h>
#include <stdio.h>

// In-class constants cannot be initialized in the class declaration. Sigh.
//

const double MathOps::PI = 3.1415926535897932384626433832795028841971693993751058209749445923;
const double MathOps::TAU = 6.2831853071795864769252867665590;
const double MathOps::PI_OVER_TWO = 1.57079632679489661923;
const double MathOps::TWO_OVER_PI = 0.63661977236758134308;

const double MathOps::RADS_TO_DEGS = 5.7295779513082320877e1;
const double MathOps::DEGS_TO_RADS = 1.7453292519943295769e-2;

const double MathOps::RAD_PER_CIRCLE = TAU;
const double MathOps::DEG_PER_CIRCLE = 360.;
const double MathOps::MINUTES_PER_DEGREE = 60.;
const double MathOps::SECONDS_PER_DEGREE = 3600.;

const double MathOps::RADS_PER_DEGREE = RAD_PER_CIRCLE / DEG_PER_CIRCLE;

const double MathOps::DEGS_TO_HRS = 1. / 15.;

const double MathOps::RADS_TO_ARCS = 2.0626480624709635516e5;
const double MathOps::ARCS_TO_RADS = 4.8481368110953599359e-6;

const double MathOps::RADIAN = (180.0 / MathOps::PI);

const double MathOps::ROUND   = 0.5;
const double MathOps::INVALID = -1.0;

/**
 * secToRadians(): convert arcseconds to radians
 *
 * @param an angle in arcseconds
 *
 * @return an angle in radians
 */
double MathOps::secToRadians( double _sec ) {
    return (_sec/MathOps::SECONDS_PER_DEGREE) * MathOps::RADS_PER_DEGREE;
}

//----------------------------------------------------------------------------
// reduce an angle in degrees to (0 <= d < 360)
//
double MathOps::normalizeDegrees ( double d ) {
    if (d >= 0.0 && d < MathOps::DEG_PER_CIRCLE)
        return d;

//     d = fmod( d, MathOps::DEG_PER_CIRCLE);
//     if ( d < 0.)
//         d += MathOps::DEG_PER_CIRCLE;
//     return d;

    double temp = (int)(d / MathOps::DEG_PER_CIRCLE);
    if (d < 0.0)
        temp --;
    temp *= MathOps::DEG_PER_CIRCLE;
    return d - temp;
}

//----------------------------------------------------------------------------
// reduce an angle in radians to (0 <= r < 2PI)
//
double MathOps::normalizeRadians ( double r ) {
    if (r >= 0.0 && r < MathOps::TAU)
        return r;

    // r = fmod( r, MathOps::TAU );
    // if ( r < 0. )
    //     r += MathOps::TAU;
    // return r;

    double temp = (int)(r / MathOps::TAU);
    if (r < 0.0)
        temp --;
    temp *= MathOps::TAU;
    return r - temp;
}

// reduce (0 < d < 360) a positive angle in degreesand convert to radians
//
/**
 * normalizeDeg2Rad(): reduce an angle in degrees to the range (0 <= d < 360),
 *                     then convert to radians
 *
 * @param degrees
 *
 * @return radians
 */
double MathOps::normalizeDeg2Rad( double _deg ) {
    return normalizeDegrees( _deg ) * MathOps::RADS_PER_DEGREE;
}

/**
 *  quadrant(): returns the quadrant ( 0, 1, 2, or 3 ) of the spec'd angle
 *
 * @param angle - in radians
 *
 * @return quadrant of angle
 */

int MathOps::quadrant( double _angle ) {
    return (int)( normalizeRadians( _angle ) * MathOps::TWO_OVER_PI );
}

/**
 * acose(): "safe" acos which prevents overflow errors
 *
 * @param angle
 *
 * @return acos (0 ... PI)
 */
double MathOps::acose( double _angle ) {
    if( _angle >= 1. )
        return( 0. );
    else if( _angle <= -1. )
        return( MathOps::PI );
    else
        return( acos( _angle ) );
}

/**
 * asine(): "safe" asine which prevents overflow errors
 *
 * @param angle
 *
 * @return asin (PI/2 ... -PI/2)
 */
double MathOps::asine( double _angle ) {
    if( _angle >= 1. )
        return( MathOps::PI_OVER_TWO );
    else if( _angle <= -1. )
        return( -MathOps::PI_OVER_TWO );
    else
        return( asin( _angle ) );
}

void MathOps::toDMS ( double degrees, int &_deg, int &_min, double &_sec ) {
    // int totalSeconds = (int)round( rangeDegrees(_rad) * SECONDS_PER_DEGREE);
    // _sec = totalSeconds % 60;
    // _min = (totalSeconds / 60) % 60;
    // _deg = totalSeconds / (360);
    
    double dtemp;
    int sign = 1;
    
    if (degrees < 0)
        sign = -1;
    
    degrees = fabs(degrees);
    _deg = (int)degrees;
    
    /* multiply remainder by 60 to get minutes */
    dtemp = 60*(degrees - _deg);
    _min = (unsigned short)dtemp;
    
    /* multiply remainder by 60 to get seconds */
    _sec = 60*(dtemp - _min);
    
    /* catch any overflows */
    if (_sec > 59) {
        _sec = 0;
        _min ++;
    }
    if (_min > 59) {
        _min = 0;
        _deg ++;
    }
    
    _deg *= sign;
}

void MathOps::toHMS ( double degrees, int &_hrs, int &_min, double &_sec ) {
    double dtemp;
    degrees = normalizeDegrees(degrees);
    
    /* divide degrees by 15 to get the hours */
    dtemp = degrees * DEGS_TO_HRS;
    _hrs = (unsigned short) dtemp;
    
    /* multiply remainder by 60 to get minutes */
    dtemp = 60 * ( dtemp - _hrs );
    _min = (unsigned short) dtemp;
    
    /* multiply remainder by 60 to get seconds */
    _sec = 60 * ( dtemp - _min );
    
    /* catch any overflows */
    if ( _sec > 59 ) {
        _sec = 0;
        _min ++;
    }
    if ( _min > 59)  {
        _min = 0;
        _hrs ++;
    }
}

/**
 * formatDegrees(): format degrees into a string
 *
 * @param degrees in
 * @param format type
 *
 * @return formated string
 *
 */
char* MathOps::formatDegrees ( double _deg, ANGLE_FMT _fmt ) {
    char *buf = new char[32];
    
    if (_fmt == DEGRESS) {
        sprintf ( buf, "%.4f°", _deg);
        return buf;
    }
    
    int first, m;
    double s;
    
    char sign = ' ';
    if (_deg < 0) {
        sign = '-';
    }
    
    switch (_fmt) {
        case D_M_S:
            MathOps::toDMS(_deg, first, m, s);
            sprintf ( buf, "%c %02d° %02d' %.2f\"", sign, (int)fabs(first), (int)fabs(m), fabs(s) );
            break;
        case H_M_S:
            MathOps::toHMS(_deg, first, m, s);
            sprintf ( buf, "%c %02dhs %02dm %.2fs", sign, (int)fabs(first), (int)fabs(m), fabs(s) );
            break;
        case DEGRESS:
            break;
    }
    
    return buf;
}

/**
 * formatRadians(): format degrees into a string
 *
 * @param radians in
 * @param format type
 *
 * @return formated string
 *
 */
char* MathOps::formatRadians ( double _rad, ANGLE_FMT _fmt ) {
    return formatDegrees(MathOps::toDegrees(_rad), _fmt);
}
