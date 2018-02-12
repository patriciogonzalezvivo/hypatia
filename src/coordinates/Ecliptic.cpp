#include "Ecliptic.h"

#include "../AstroOps.h"
#include <math.h>

Ecliptic::Ecliptic() : m_radius(1.0) {
}

Ecliptic::Ecliptic(const Vector& _parent) {
    m_phi = atan2(_parent.y, _parent.x);
    m_theta = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
    m_radius = _parent.getMagnitud();
}

Ecliptic::Ecliptic(double _lng, double _lat, double _radius, ANGLE_TYPE _type) {
    if ( _type == RADS ) {
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

double Ecliptic::getLongitude(ANGLE_TYPE _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_phi );
    }
    else {
        return m_phi;
    }
}

double Ecliptic::getLatitude(ANGLE_TYPE _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_theta );
    }
    else {
        return m_theta;
    }
}

double Ecliptic::getRadius() const {
    return m_radius;
};

Ecliptic& Ecliptic::operator= (const Vector& _vec) {
    m_phi = atan2(_vec.y, _vec.x);
    m_theta = atan2(_vec.z, sqrt(_vec.x * _vec.x + _vec.y * _vec.y));
    m_radius = _vec.getMagnitud();
    
    return *this;
}

Vector Ecliptic::getVector() const {
    return Vector(*this) * m_radius;
}
