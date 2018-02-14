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
 * toDegrees(): convert radians to degrees
 *
 * @param an angle in radians
 *
 * @return an angle in degrees
 */
double MathOps::toDegrees( double _rad ) { 
    return _rad * RADS_TO_DEGS; 
};

/**
 * toDegrees(): convert radians to degrees
 *
 * @param an degrees
 * @param an minutes
 * @param an seconds
 *
 * @return an angle in degrees
 */
double MathOps::toDegrees( int _deg, int _min, double _sec ) {
    double sign = 1.0;
    if ( (_deg < 0) || (_min < 0) || (_sec < 0) ) {
        sign = -1.0;
    }
    return sign * (fabs(_deg) + fabs(_min)/MINUTES_PER_DEGREE + fabs(_sec)/SECONDS_PER_DEGREE);
}

/**
 * toRadians(): convert degrees to radians
 *
 * @param an angle in degrees
 *
 * @return an angle in radians
 */
double MathOps::toRadians( double _deg ) { 
    return _deg * DEGS_TO_RADS; 
};

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

/**
 * toHrs(): convert arcseconds to radians
 *
 * @param degrees
 *
 * @return hours double
 */
double MathOps::toHrs( double _angle, ANGLE_TYPE _type ) {
    if ( _type == RADS ) {
        _angle = MathOps::toDegrees(_angle);
    }
    return _angle * DEGS_TO_HRS;
};

//----------------------------------------------------------------------------
// reduce an angle in degrees to (0 <= d < 360) or (0 <= d << 2PI )
//
double MathOps::normalize ( double _angle, ANGLE_TYPE _type ) {
    
    if ( _type == DEGS ) {
        if (_angle >= 0.0 &&
            _angle < MathOps::DEG_PER_CIRCLE ) {
            return _angle;
        }
        
        _angle = MathOps::mod( _angle, MathOps::DEG_PER_CIRCLE);
        if ( _angle < 0.) {
            _angle += MathOps::DEG_PER_CIRCLE;
        }
        return _angle;
        
//        double temp = (int)(_angle / MathOps::DEG_PER_CIRCLE);
//        if ( _angle < 0.0 ) {
//            temp --;
//        }
//        temp *= MathOps::DEG_PER_CIRCLE;
//        return _angle - temp;
    }
    else {
        if (_angle >= 0.0 &&
            _angle < MathOps::TAU ) {
            return _angle;
        }
        
        _angle = MathOps::mod( _angle, MathOps::TAU );
        if ( _angle < 0. ) {
            _angle += MathOps::TAU;
        }
        return _angle;
        
//        double temp = (int)(_angle / MathOps::TAU);
//        if ( _angle < 0.0 ) {
//            temp --;
//        }
//        temp *= MathOps::TAU;
//        return _angle - temp;
    }
}

/**
 *  quadrant(): returns the quadrant ( 0, 1, 2, or 3 ) of the spec'd angle
 *
 * @param angle - in radians
 *
 * @return quadrant of angle
 */

int MathOps::quadrant( double _rad ) {
    return (int)( normalize( _rad, RADS ) * MathOps::TWO_OVER_PI );
}

void MathOps::toDMS ( double _angle, ANGLE_TYPE _type, int &_deg, int &_min, double &_sec ) {
    // int totalSeconds = (int)round( rangeDegrees(_rad) * SECONDS_PER_DEGREE);
    // _sec = totalSeconds % 60;
    // _min = (totalSeconds / 60) % 60;
    // _deg = totalSeconds / (360);
    
    double degrees = _angle;
    if ( _type == RADS) {
        degrees = MathOps::toDegrees(_angle);
    }
    
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

void MathOps::toHMS ( double _angle, ANGLE_TYPE _type, int &_hrs, int &_min, double &_sec ) {
    double degrees = _angle;
    if ( _type == RADS) {
        degrees = MathOps::toDegrees(_angle);
    }
    
    double dtemp;
    degrees = normalize(degrees, DEGS);
    
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
 * formatAngle(): format angle into a string
 *
 * @param angle value
 * @param angle type
 * @param format type
 *
 * @return formated string
 *
 */
char* MathOps::formatAngle ( double _angle, ANGLE_TYPE _type, ANGLE_FMT _fmt ) {
    double degrees = _angle;
    if ( _type == RADS) {
        degrees = MathOps::toDegrees(_angle);
    }
    
    char *buf = new char[32];
    
    if (_fmt == Dd) {
        sprintf ( buf, "%.1f°", degrees);
        return buf;
    }
    else if (_fmt == Ddd) {
        sprintf ( buf, "%.2f°", degrees);
        return buf;
    }
    else if (_fmt == Ddd) {
        sprintf ( buf, "%.3f°", degrees);
        return buf;
    }
    else if (_fmt == Hs) {
        sprintf ( buf, "%.1f°", toHrs(degrees, DEGS));
        return buf;
    }
    else if (_fmt == Hss) {
        sprintf ( buf, "%.2f°", toHrs(degrees, DEGS));
        return buf;
    }
    else if (_fmt == Hsss) {
        sprintf ( buf, "%.3f°", toHrs(degrees, DEGS));
        return buf;
    }
    
    int first, m;
    double s;
    
    char sign = ' ';
    if (degrees < 0) {
        sign = '-';
    }
    
    switch (_fmt) {
        case D_M_Ss:
            MathOps::toDMS(degrees, DEGS, first, m, s);
            sprintf ( buf, "%c %02d° %02d' %.2f\"", sign, (int)fabs(first), (int)fabs(m), fabs(s) );
            break;
        case D_Mm:
            MathOps::toDMS(degrees, DEGS, first, m, s);
            sprintf ( buf, "%c %02d° %.2f'", sign, (int)fabs(first), fabs(m + s/60.0));
            break;
        case H_M_Ss:
            MathOps::toHMS(degrees, DEGS, first, m, s);
            sprintf ( buf, "%c %02dhs %02dm %.2fs", sign, (int)fabs(first), (int)fabs(m), fabs(s) );
            break;
        case H_Mm:
            MathOps::toHMS(degrees, DEGS, first, m, s);
            sprintf ( buf, "%c %02dhs %.2fm", sign, (int)fabs(first), fabs(m + s/60.0));
            break;
        case Dd:
        case Ddd:
        case Dddd:
        case Hs:
        case Hss:
        case Hsss:
            break;
    }
    
    return buf;
}

/**
 * acose(): "safe" acos which prevents overflow errors
 *
 * @param angle
 *
 * @return acos (0 ... PI)
 */
double MathOps::acose ( double _angle ) {
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
double MathOps::asine ( double _angle ) {
    if( _angle >= 1. )
        return( MathOps::PI_OVER_TWO );
    else if( _angle <= -1. )
        return( -MathOps::PI_OVER_TWO );
    else
        return( asin( _angle ) );
}

/**
 * fract(): gives the fractional part of a number
 *
 * @param number
 *
 * @return fractional part of it 
 */
double MathOps::fract ( double _x ) {
    return _x - floor(_x);
}

/**
 * mod(): calculates x modulus y
 *
 * @param base
 *
 * @return modulus of
 */
double MathOps::mod ( double _x, double _y ) {
    return _y * MathOps::fract( _x / _y);
}

long MathOps::mod ( long x, long y ) {
    long rval = x % y;
    
    if( rval < 0L )
        rval += y;
    
    return rval;
}
