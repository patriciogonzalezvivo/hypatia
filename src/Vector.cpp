#include "Vector.h"

#include "MathOps.h"
#include <math.h>

Vector::Vector(): x(0.0), y(0.0), z(0.0) {
}

Vector::Vector(double _lon, double _lat, double _radius, bool _radiant) {
    // http://www.stjarnhimlen.se/comp/tutorial.html
    if (!_radiant) {
        double lon = MathOps::toRadians(_lon);
        double lat = MathOps::toRadians(_lat);
        const double cosLat = cos(lat);
        x = cos(lon) * cosLat * _radius;
        y = sin(lon) * cosLat * _radius;
        z = sin(lat) * _radius;
    }
    else {
        const double cosLat = cos(_lat);
        x = cos(_lon) * cosLat * _radius;
        y = sin(_lon) * cosLat * _radius;
        z = sin(_lat) * _radius;
    }
    
}

Vector::~Vector() {
}

Vector Vector::operator+ (const Vector& _vec) const {
    Vector rta;
    rta.x = x + _vec.x;
    rta.y = y + _vec.y;
    rta.z = z + _vec.z;
    return rta;
}

Vector Vector::operator- (const Vector& _vec) const {
    Vector rta;
    rta.x = x - _vec.x;
    rta.y = y - _vec.y;
    rta.z = z - _vec.z;
    return rta;
}

Vector Vector::operator* (const Vector& _vec) const {
    Vector rta;
    rta.x = x * _vec.x;
    rta.y = y * _vec.y;
    rta.z = z * _vec.z;
    return rta;
}

Vector Vector::operator/ (const Vector& _vec) const {
    Vector rta;
    rta.x = x / _vec.x;
    rta.y = y / _vec.y;
    rta.z = z / _vec.z;
    return rta;
}

Vector Vector::operator+ (double _d) const {
    Vector rta;
    rta.x = x + _d;
    rta.y = y + _d;
    rta.z = z + _d;
    return rta;
}

Vector Vector::operator- (double _d) const {
    Vector rta;
    rta.x = x - _d;
    rta.y = y - _d;
    rta.z = z - _d;
    return rta;
}

Vector Vector::operator* (double _d) const {
    Vector rta;
    rta.x = x * _d;
    rta.y = y * _d;
    rta.z = z * _d;
    return rta;
}

Vector Vector::operator/ (double _d) const {
    Vector rta;
    rta.x = x / _d;
    rta.y = y / _d;
    rta.z = z / _d;
    return rta;
}

Vector& Vector::operator+= (const Vector& _vec) {
    x += _vec.x;
    y += _vec.y;
    z += _vec.z;
    return *this;
}

Vector& Vector::operator-= (const Vector& _vec) {
    x -= _vec.x;
    y -= _vec.y;
    z -= _vec.z;
    return *this;
}

Vector& Vector::operator*= (const Vector& _vec) {
    x *= _vec.x;
    y *= _vec.y;
    z *= _vec.z;
    return *this;
}

Vector& Vector::operator/= (const Vector& _vec) {
    x /= _vec.x;
    y /= _vec.y;
    z /= _vec.z;
    return *this;
}

Vector& Vector::operator+= (double _d) {
    x += _d;
    y += _d;
    z += _d;
    return *this;
}
Vector& Vector::operator-= (double _d) {
    x -= _d;
    y -= _d;
    z -= _d;
    return *this;
}

Vector& Vector::operator*= (double _d) {
    x *= _d;
    y *= _d;
    z *= _d;
    return *this;
}

Vector& Vector::operator/= (double _d) {
    x /= _d;
    y /= _d;
    z /= _d;
    return *this;
}

double Vector::dot(const Vector& _vec) const {
    return (x * _vec.x) +
    (y * _vec.y) +
    (z * _vec.z);
}

double Vector::magnitud() const {
    return sqrt(x*x + y*y + z*z);
}

double Vector::getLongitude() const {
    return MathOps::toDegrees(getLongitudeRadians());
}

double Vector::getLongitudeRadians() const {
    return atan2(y, x);
}

double Vector::getLatitude() const {
    return MathOps::toDegrees(getLatitudeRadians());
}

double Vector::getLatitudeRadians() const {
    return atan2(z, sqrt(x*x + y*y));
}

double Vector::getRadius() const {
    return magnitud();
}
