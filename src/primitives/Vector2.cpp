#include "hypatia/primitives/Vector2.h"
#include "hypatia/MathOps.h"

#include <math.h>

Vector2::Vector2() : x(0.0), y(0.0) {
}

Vector2::Vector2(const Polar& _polar) {
    x = cos( _polar[0] );
    y = sin( _polar[1] );
}

Vector2::Vector2(const double _x, const double _y): x(_x), y(_y) {
}

Vector2::~Vector2() {
}

Vector2 Vector2::getNormalized() const {
    double length = getMagnitud();
    if ( length > 0 ) {
        return Vector2( x/length, y/length );
    } 
    else {
        return Vector2();
    }
}

double Vector2::getMagnitud() const {
    return sqrt(x*x + y*y);
}

double Vector2::dot(const Vector2& _vec) const {
    return (x * _vec.x) + (y * _vec.y);
}

Vector2& Vector2::normalize() {
    double length = getMagnitud();
    if ( length > 0 ) {
        x /= length;
        y /= length;
    }
    return *this;
}

double * Vector2::getPtr() {
    return (double*)&x;
}
const double * Vector2::getPtr() const {
    return (const double *)&x;
}

double& Vector2::operator[]( int _n ){
    return getPtr()[_n];
}

double Vector2::operator[]( int _n ) const {
    return getPtr()[_n];
}

Vector2 Vector2::operator+ (const Vector2& _vec) const {
    Vector2 rta;
    rta.x = x + _vec.x;
    rta.y = y + _vec.y;
    return rta;
}

Vector2 Vector2::operator- (const Vector2& _vec) const {
    Vector2 rta;
    rta.x = x - _vec.x;
    rta.y = y - _vec.y;
    return rta;
}

Vector2 Vector2::operator* (const Vector2& _vec) const {
    Vector2 rta;
    rta.x = x * _vec.x;
    rta.y = y * _vec.y;
    return rta;
}

Vector2 Vector2::operator/ (const Vector2& _vec) const {
    Vector2 rta;
    rta.x = x / _vec.x;
    rta.y = y / _vec.y;
    return rta;
}

Vector2 Vector2::operator+ (double _d) const {
    Vector2 rta;
    rta.x = x + _d;
    rta.y = y + _d;
    return rta;
}

Vector2 Vector2::operator- (double _d) const {
    Vector2 rta;
    rta.x = x - _d;
    rta.y = y - _d;
    return rta;
}

Vector2 Vector2::operator* (double _d) const {
    Vector2 rta;
    rta.x = x * _d;
    rta.y = y * _d;
    return rta;
}

Vector2 Vector2::operator/ (double _d) const {
    Vector2 rta;
    rta.x = x / _d;
    rta.y = y / _d;
    return rta;
}

Vector2& Vector2::operator+= (const Vector2& _vec) {
    x += _vec.x;
    y += _vec.y;
    return *this;
}

Vector2& Vector2::operator-= (const Vector2& _vec) {
    x -= _vec.x;
    y -= _vec.y;
    return *this;
}

Vector2& Vector2::operator*= (const Vector2& _vec) {
    x *= _vec.x;
    y *= _vec.y;
    return *this;
}

Vector2& Vector2::operator/= (const Vector2& _vec) {
    x /= _vec.x;
    y /= _vec.y;
    return *this;
}

Vector2& Vector2::operator+= (double _d) {
    x += _d;
    y += _d;
    return *this;
}
Vector2& Vector2::operator-= (double _d) {
    x -= _d;
    y -= _d;
    return *this;
}

Vector2& Vector2::operator*= (double _d) {
    x *= _d;
    y *= _d;
    return *this;
}

Vector2& Vector2::operator/= (double _d) {
    x /= _d;
    y /= _d;
    return *this;
}
