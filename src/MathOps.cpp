#include "MathOps.h"
#include <math.h>

// In-class constants cannot be initialized in the class declaration. Sigh.
//

const double MathOps::HD_PI = 3.1415926535897932384626433832795028841971693993751058209749445923;
const double MathOps::TAU = 6.2831853071795864769252867665590;
const double MathOps::PI_OVER_TWO = HD_PI / 2.;
const double MathOps::TWO_OVER_PI = 2. / HD_PI;

const double MathOps::RADS_TO_DEGS = 5.7295779513082320877e1;
const double MathOps::DEGS_TO_RADS = 1.7453292519943295769e-2;

const double MathOps::DEG_PER_CIRCLE = 360.;
const double MathOps::MINUTES_PER_DEGREE = 60.;
const double MathOps::SECONDS_PER_DEGREE = 3600.;

const double MathOps::DEGS_TO_HRS = 1. / 15.;

const double MathOps::RADS_TO_ARCS = 2.0626480624709635516e5;
const double MathOps::ARCS_TO_RADS = 4.8481368110953599359e-6;

const double MathOps::RADIAN = (180.0 / HD_PI);

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
    degrees = rangeDegrees(degrees);

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

//----------------------------------------------------------------------------
// reduce an angle in degrees to (0 <= d < 360)
//
double MathOps::rangeDegrees ( double d ) {
    if (d >= 0.0 && d < MathOps::DEG_PER_CIRCLE)
        return d;

    // d = fmod( d, MathOps::DEG_PER_CIRCLE);
    // if ( d < 0.)
    //     d += MathOps::DEG_PER_CIRCLE;
    // return d;

    double temp = (int)(d / MathOps::DEG_PER_CIRCLE);
    if (d < 0.0)
        temp --;
    temp *= MathOps::DEG_PER_CIRCLE;
    return d - temp;
}

//----------------------------------------------------------------------------
// reduce an angle in radians to (0 <= r < 2PI)
//
double MathOps::rangeRadians ( double r ) {
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
