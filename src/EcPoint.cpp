#include "EcPoint.h"

#include <math.h>

EcPoint::EcPoint() : m_lng(0.0), m_lat(0.0), m_radius(0.0) {
}

EcPoint::EcPoint(const Vector& _parent) {
    m_lng = atan2(_parent.y, _parent.x);
    m_lat = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
    m_radius = _parent.magnitud();
}

EcPoint::EcPoint(double _lng, double _lat, double _radius, bool _radiant) {
    setLongitude(_lng, _radiant);
    setLatitude(_lng, _radiant);
    setRadius(_radius);
}

EcPoint::~EcPoint() {
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
    const double cosLat = cos(m_lat);
    
    Vector v;
    v.x = cos(m_lng) * cosLat * m_radius;
    v.y = sin(m_lng) * cosLat * m_radius;
    v.z = sin(m_lat) * m_radius;
    
    return v;
}
