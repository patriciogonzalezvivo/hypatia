#include "EcPoint.h"

#include <math.h>

EcPoint::EcPoint() : m_lng(0.0), m_lat(0.0), m_radius(0.0) {
}

EcPoint::EcPoint(const Vector& _parent) {
    m_lng = atan2(_parent.y, _parent.x);
    m_lat = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
    m_radius = _parent.getMagnitud();
}

EcPoint::EcPoint(double _lng, double _lat, double _radius, bool _radiant) {
    setLongitude(_lng, _radiant);
    setLatitude(_lat, _radiant);
    setRadius(_radius);
}

EcPoint::~EcPoint() {
}

EcPoint& EcPoint::operator= (const Vector& _vec) {
    m_lng = atan2(_vec.y, _vec.x);
    m_lat = atan2(_vec.z, sqrt(_vec.x * _vec.x + _vec.y * _vec.y));
    m_radius = _vec.getMagnitud();
    
    return *this;
}

void EcPoint::setLongitude( double _lng, bool _radians ) {
    if (_radians) {
        m_lng = _lng;
    }
    else {
        m_lng = MathOps::toRadians( _lng );
    }
}

void EcPoint::setLatitude( double _lat, bool _radians ) {
    if (_radians) {
        m_lat = _lat;
    }
    else {
        m_lat = MathOps::toRadians( _lat );
    }
}

void EcPoint::setRadius( double _radius ) {
    m_radius = _radius;
}

Vector EcPoint::getEclipticVector() const {
    Vector v;
    v.setPolar(m_lng, m_lat, true);
    return v * m_radius;
}
