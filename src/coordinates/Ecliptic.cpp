#include "Ecliptic.h"

#include <math.h>

Ecliptic::Ecliptic() : m_radius(1.0) {
}

Ecliptic::Ecliptic(const Vector& _parent) {
    m_phi = atan2(_parent.y, _parent.x);
    m_theta = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
    m_radius = _parent.getMagnitud();
}

Ecliptic::Ecliptic(double _lng, double _lat, double _radius, bool _radians) {
    if (_radians) {
        m_phi = _lng;
        m_theta = _lat;
    }
    else {
        m_phi = MathOps::toRadians( _lng );
        m_theta = MathOps::toRadians( _lat );
    }

    m_radius = _radius;
}

Ecliptic::~Ecliptic() {
}

Ecliptic& Ecliptic::operator= (const Vector& _vec) {
    m_phi = atan2(_vec.y, _vec.x);
    m_theta = atan2(_vec.z, sqrt(_vec.x * _vec.x + _vec.y * _vec.y));
    m_radius = _vec.getMagnitud();
    
    return *this;
}

//void EcPoint::setLongitude( double _lng, bool _radians ) {
//    if (_radians) {
//        m_lng = _lng;
//    }
//    else {
//        m_lng = MathOps::toRadians( _lng );
//    }
//}
//
//void EcPoint::setLatitude( double _lat, bool _radians ) {
//    if (_radians) {
//        m_lat = _lat;
//    }
//    else {
//        m_lat = MathOps::toRadians( _lat );
//    }
//}
//
//void EcPoint::setRadius( double _radius ) {
//    m_radius = _radius;
//}

Vector Ecliptic::getVector() const {
    return Vector(*this) * m_radius;
}
