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
const double MathOps::TO_HRS = 1./15;

void MathOps::toDegreesMinSec ( double _rad, int &_deg, int &_min, int &_sec ) {
    int totalSeconds = (int)round( MathOps::normalizeRadians(_rad) * 360 * 60 * 60 / TAU);
    _sec = totalSeconds % 60;
    _min = (totalSeconds / 60) % 60;
    _deg = totalSeconds / (60 * 60);
}

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

AstroVector::AstroVector(): x(0.), y(0.), z(0.) {
}

AstroVector::AstroVector(double _radiant_lon, double _radiant_lat, double _radius): x(0.), y(0.), z(0.) {
    // http://www.stjarnhimlen.se/comp/tutorial.html
    const double cosLat = cos(_radiant_lat);
    x = cos(_radiant_lon) * cosLat * _radius;
    y = sin(_radiant_lon) * cosLat * _radius;
    z = sin(_radiant_lat) * _radius;
}

AstroVector AstroVector::operator+ (const AstroVector& _vec) const {
    AstroVector rta;
    rta.x = x + _vec.x;
    rta.y = y + _vec.y;
    rta.z = z + _vec.z;
    return rta;
}

AstroVector AstroVector::operator- (const AstroVector& _vec) const {
    AstroVector rta;
    rta.x = x - _vec.x;
    rta.y = y - _vec.y;
    rta.z = z - _vec.z;
    return rta;
}
AstroVector AstroVector::operator* (const AstroVector& _vec) const {
    AstroVector rta;
    rta.x = x * _vec.x;
    rta.y = y * _vec.y;
    rta.z = z * _vec.z;
    return rta;
}
AstroVector AstroVector::operator/ (const AstroVector& _vec) const {
    AstroVector rta;
    rta.x = x / _vec.x;
    rta.y = y / _vec.y;
    rta.z = z / _vec.z;
    return rta;
}
AstroVector AstroVector::operator+ (double _d) const {
    AstroVector rta;
    rta.x = x + _d;
    rta.y = y + _d;
    rta.z = z + _d;
    return rta;
}

AstroVector AstroVector::operator- (double _d) const {
    AstroVector rta;
    rta.x = x - _d;
    rta.y = y - _d;
    rta.z = z - _d;
    return rta;
}
AstroVector AstroVector::operator* (double _d) const {
    AstroVector rta;
    rta.x = x * _d;
    rta.y = y * _d;
    rta.z = z * _d;
    return rta;
}
AstroVector AstroVector::operator/ (double _d) const {
    AstroVector rta;
    rta.x = x / _d;
    rta.y = y / _d;
    rta.z = z / _d;
    return rta;
}

AstroVector& AstroVector::operator+= (const AstroVector& _vec) {
    x += _vec.x;
    y += _vec.y;
    z += _vec.z;
    return *this;
}
AstroVector& AstroVector::operator-= (const AstroVector& _vec) {
    x -= _vec.x;
    y -= _vec.y;
    z -= _vec.z;
    return *this;
}
AstroVector& AstroVector::operator*= (const AstroVector& _vec) {
    x *= _vec.x;
    y *= _vec.y;
    z *= _vec.z;
    return *this;
}
AstroVector& AstroVector::operator/= (const AstroVector& _vec) {
    x /= _vec.x;
    y /= _vec.y;
    z /= _vec.z;
    return *this;
}

AstroVector& AstroVector::operator+= (double _d) {
    x += _d;
    y += _d;
    z += _d;
    return *this;
}
AstroVector& AstroVector::operator-= (double _d) {
    x -= _d;
    y -= _d;
    z -= _d;
    return *this;
}
AstroVector& AstroVector::operator*= (double _d) {
    x *= _d;
    y *= _d;
    z *= _d;
    return *this;
}
AstroVector& AstroVector::operator/= (double _d) {
    x /= _d;
    y /= _d;
    z /= _d;
    return *this;
}

double AstroVector::getLongitud() {
    return atan2(y, x);
}

double AstroVector::getLatitud() {
    return atan2(z, sqrt(x*x + y*y));
}

double AstroVector::getRadius() {
    return sqrt(x*x + y*y + z*z);
}
