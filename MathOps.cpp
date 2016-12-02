#include "MathOps.h"
#include <math.h>

// In-class constants cannot be initialized in the class declaration. Sigh.
//

const double MathOps::HD_PI = 3.1415926535897932384626433832795028841971693993751058209749445923;
const double MathOps::TAU = 6.2831853071795864769252867665590;
const double MathOps::PI_OVER_TWO = HD_PI / 2.;
const double MathOps::TWO_OVER_PI = 2. / HD_PI;

const double MathOps::DEG_PER_CIRCLE = 360.;
const double MathOps::MINUTES_PER_DEGREE = 60.;
const double MathOps::SECONDS_PER_DEGREE = 3600.;

const double MathOps::TO_RADS = HD_PI / 180.;


//----------------------------------------------------------------------------
// reduce an angle in degrees to (0 <= d < 360)
//
double MathOps::normalizeDegrees ( double d ) {
    d = fmod( d, MathOps::DEG_PER_CIRCLE);
    if( d < 0.)
        d += MathOps::DEG_PER_CIRCLE;
    return d;
}

//----------------------------------------------------------------------------
// reduce an angle in radians to (0 <= r < 2PI)
//
double MathOps::normalizeRadians ( double r ) {
    r = fmod( r, MathOps::TAU );
    if( r < 0. )
        r += MathOps::TAU;
    return r;
}

void MathOps::rotateVector( AstroVector& v, double angle, int axis ) {
    const double sinAng = sin( angle );
    const double cosAng = cos( angle );
    const int a = (axis + 1) % 3;
    const int b = (axis + 2) % 3;
    
    double temp = v[a] * cosAng - v[b] * sinAng;
    v[b] = v[b] * cosAng + v[a] * sinAng;
    v[a] = temp;
}

// convert polar coords to cartesian
//
void MathOps::polar3ToCartesian( AstroVector& v, double lon, double lat ) {
    const double cosLat = cos(lat);
    v[0] = cos(lon) * cosLat;
    v[1] = sin(lon) * cosLat;
    v[2] = sin(lat);
}

void MathOps::polar3ToCartesian( AstroVector& v, double lon, double lat, double rad ) {
    const double cosLat = cos(lat);
    v[0] = cos(lon) * cosLat * rad;
    v[1] = sin(lon) * cosLat * rad;
    v[2] = sin(lat) * rad;
}
