#include "Galactic.h"

#include "../CoordOps.h"
#include <math.h>

Galactic::Galactic() : m_radius(1.0) {
}

Galactic::Galactic(const Vector& _parent, DISTANCE_UNIT _type) {
    m_phi = atan2(_parent.y, _parent.x);
    m_theta = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
    
    if ( _type == PC ) {
        m_radius = _parent.getMagnitud();
    }
    else if ( _type == LY ) {
        m_radius = _parent.getMagnitud() * CoordOps::LY_TO_PC;
    }
    else if ( _type == AU ) {
        m_radius = _parent.getMagnitud() * CoordOps::AU_TO_LY * CoordOps::LY_TO_PC;
    }
    else if ( _type == KM ) {
        m_radius = _parent.getMagnitud() *  CoordOps::KM_TO_AU * CoordOps::AU_TO_LY * CoordOps::LY_TO_PC;
    }
}

Galactic::Galactic(double _lng, double _lat, ANGLE_UNIT _a_type) {
     if ( _a_type == RADS ) {
        m_phi = _lng;
        m_theta = _lat;
    }
    else {
        m_phi = MathOps::toRadians( _lng );
        m_theta = MathOps::toRadians( _lat );
    }

    m_radius = 1.0;
}

Galactic::Galactic(double _lng, double _lat, double _paralax, ANGLE_UNIT _a_type) {
    if ( _a_type == RADS ) {
        m_phi = _lng;
        m_theta = _lat;
    }
    else {
        m_phi = MathOps::toRadians( _lng );
        m_theta = MathOps::toRadians( _lat );
    }

    m_radius = (1.0/_paralax);
}

Galactic::Galactic (double _lng, double _lat, double _radius, ANGLE_UNIT _a_type, DISTANCE_UNIT _d_type) {
    if ( _a_type == RADS ) {
        m_phi = _lng;
        m_theta = _lat;
    }
    else {
        m_phi = MathOps::toRadians( _lng );
        m_theta = MathOps::toRadians( _lat );
    }

    if ( _d_type == PC ) {
        m_radius = _radius;
    }
    else if ( _d_type == LY ) {
        m_radius = _radius * CoordOps::LY_TO_PC;
    }
    else if ( _d_type == AU ) {
        m_radius = _radius * CoordOps::AU_TO_LY * CoordOps::LY_TO_PC;
    }
    else if ( _d_type == KM ) {
        m_radius = _radius *  CoordOps::KM_TO_AU * CoordOps::AU_TO_LY * CoordOps::LY_TO_PC;
    }
}

Galactic::~Galactic() {
}

double Galactic::getLongitude(ANGLE_UNIT _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_phi );
    }
    else {
        return m_phi;
    }
}

double Galactic::getLatitude(ANGLE_UNIT _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_theta );
    }
    else {
        return m_theta;
    }
}

Galactic& Galactic::operator= (const Vector& _vec) {
    m_phi = atan2(_vec.y, _vec.x);
    m_theta = atan2(_vec.z, sqrt(_vec.x * _vec.x + _vec.y * _vec.y));
    m_radius = _vec.getMagnitud();
    return *this;
}

Galactic& Galactic::operator= (const Polar& _pol) {
    m_phi = _pol[0];
    m_theta = _pol[1];
    return *this;
}

double Galactic::getRadius (DISTANCE_UNIT _type) const {
    if ( _type == PC) {
        return m_radius;
    }
    else if ( _type == LY ) {
       return getRadius(PC) * CoordOps::PC_TO_LY;
    }
    else if ( _type == AU ) {
        return getRadius(LY) * CoordOps::LY_TO_AU;
    }
    else if ( _type == KM ) {
        return getRadius(AU) * CoordOps::AU_TO_KM;
    }
    else {
        return m_radius;
    }
}

Vector Galactic::getVector (DISTANCE_UNIT _type) const {
    if ( _type == PC ) {
        return Vector(*this) * m_radius;
    }
    else if ( _type == LY ) {
       return getVector(PC) * CoordOps::PC_TO_LY;
    }
    else if ( _type == AU ) {
        return getVector(LY) * CoordOps::LY_TO_AU;
    }
    else if ( _type == KM ) {
        return getVector(AU) * CoordOps::AU_TO_KM;
    }
    else {
        return Vector(*this);
    }
}
