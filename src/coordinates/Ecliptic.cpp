#include "hypatia/coordinates/Ecliptic.h"

#include "hypatia/CoordOps.h"
#include <math.h>

Ecliptic::Ecliptic() : m_radius(1.0) {
}

Ecliptic::Ecliptic(const Vector3& _parent, DISTANCE_UNIT _type) {
    m_phi = atan2(_parent.y, _parent.x);
    m_theta = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
    if ( _type == AU ) {
        m_radius = _parent.getMagnitud();
    }
    else if ( _type == KM ) {
        m_radius = _parent.getMagnitud() * CoordOps::KM_TO_AU;
    }
    else if ( _type == LY ) {
        m_radius = _parent.getMagnitud() * CoordOps::LY_TO_AU;
    }
    else if ( _type == PC ) {
        m_radius = _parent.getMagnitud() * CoordOps::PC_TO_LY * CoordOps::LY_TO_AU;
    }
}

Ecliptic::Ecliptic(double _lng, double _lat, double _radius, ANGLE_UNIT _a_type, DISTANCE_UNIT _r_type) {
    if ( _a_type == RADS ) {
        m_phi = _lng;
        m_theta = _lat;
    }
    else {
        m_phi = MathOps::toRadians( _lng );
        m_theta = MathOps::toRadians( _lat );
    }
    
    if (_r_type == AU ) {
        m_radius = _radius;
    }
    else if ( _r_type == KM ) {
        m_radius = _radius * CoordOps::KM_TO_AU;
    }
    else if ( _r_type == LY ) {
        m_radius = _radius * CoordOps::LY_TO_AU;
    }
    else if ( _r_type == PC ) {
        m_radius = _radius * CoordOps::PC_TO_LY * CoordOps::LY_TO_AU;
    }
}

Ecliptic::~Ecliptic() {
}

double Ecliptic::getLongitude(ANGLE_UNIT _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_phi );
    }
    else {
        return m_phi;
    }
}

double Ecliptic::getLatitude(ANGLE_UNIT _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_theta );
    }
    else {
        return m_theta;
    }
}

Ecliptic& Ecliptic::operator= (const Vector3& _vec) {
    m_phi = atan2(_vec.y, _vec.x);
    m_theta = atan2(_vec.z, sqrt(_vec.x * _vec.x + _vec.y * _vec.y));
    m_radius = _vec.getMagnitud();
    return *this;
}

double Ecliptic::getRadius (DISTANCE_UNIT _type) const {
    if (_type == AU) {
        return m_radius;
    }
    else if ( _type == KM ) {
        return getRadius(AU) * CoordOps::AU_TO_KM;
    }
    else if ( _type == LY ) {
        return getRadius(AU) * CoordOps::AU_TO_LY;
    }
    else if ( _type == PC ) {
        return getRadius(LY) * CoordOps::LY_TO_PC;
    }
    else {
        return m_radius;
    }
}

Vector3 Ecliptic::getVector (DISTANCE_UNIT _type) const {
    return Vector3(*this) * getRadius(_type);
}
