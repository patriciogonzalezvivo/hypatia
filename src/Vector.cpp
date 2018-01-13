#include "Vector.h"

#include <math.h>

Vector::Vector(): x(0.0), y(0.0), z(0.0) {
}

Vector::Vector(double _radiant_lon, double _radiant_lat, double _radius): x(0.0), y(0.0), z(0.0) {
    // http://www.stjarnhimlen.se/comp/tutorial.html
    const double cosLat = cos(_radiant_lat);
    x = cos(_radiant_lon) * cosLat * _radius;
    y = sin(_radiant_lon) * cosLat * _radius;
    z = sin(_radiant_lat) * _radius;
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

double Vector::getLongitud() {
    return atan2(y, x);
}

double Vector::getLatitud() {
    return atan2(z, sqrt(x*x + y*y));
}

double Vector::getRadius() {
    return sqrt(x*x + y*y + z*z);
}