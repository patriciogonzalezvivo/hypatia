#include "hypatia/coordinates/Horizontal.h"

#include <math.h>

Horizontal::Horizontal () {
}

Horizontal::Horizontal ( const Vector& _parent ) {
    m_phi = atan2(_parent.y, _parent.x);
    m_theta = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
}

Horizontal::Horizontal ( const double _alt, const double _az, ANGLE_UNIT _type ) {
    if ( _type == RADS ) {
        m_phi = _alt;
        m_theta = _az;
    }
    else {
        m_phi = MathOps::toRadians( _alt );
        m_theta = MathOps::toRadians( _az );
    }
}

Horizontal::~Horizontal () {
}

double Horizontal::getAltitud(ANGLE_UNIT _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_phi );
    }
    else {
        return m_phi;
    }
}

double Horizontal::getAzimuth(ANGLE_UNIT _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_theta );
    }
    else {
        return m_theta;
    }
}

Vector Horizontal::getVector () const {
    return Vector(Polar(m_phi, -m_theta, RADS));
}
