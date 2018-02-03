#include "Vector.h"

#include "MathOps.h"
#include <math.h>

Vector::Vector() : x(0.0), y(0.0), z(0.0) {
}

Vector::Vector(const double _x, const double _y, const double _z): x(_x), y(_y), z(_z) {
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

double Vector::getMagnitud() const {
    return sqrt(x*x + y*y + z*z);
}

double * Vector::getPtr() {
    return (double*)&x;
}
const double * Vector::getPtr() const {
    return (const double *)&x;
}

double& Vector::operator[]( int _n ){
    return getPtr()[_n];
}

double Vector::operator[]( int _n ) const {
    return getPtr()[_n];
}

void Vector::rotate(double _angle, int _axis, bool _radians) {
    if (!_radians) {
        _angle = MathOps::toRadians(_angle);
    }
    const double sinAng = sin( _angle );
    const double cosAng = cos( _angle );
    const int a = (_axis + 1) % 3;
    const int b = (_axis + 2) % 3;
    
    double temp = getPtr()[a] * cosAng - getPtr()[b] * sinAng;
    getPtr()[b] = getPtr()[b] * cosAng + getPtr()[a] * sinAng;
    getPtr()[a] = temp;
}
