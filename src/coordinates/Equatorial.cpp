#include "Equatorial.h"

#include <math.h>

Equatorial::Equatorial() {
}

Equatorial::Equatorial(const Vector& _parent) {
    m_phi = atan2(_parent.y, _parent.x);
    m_theta = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
}

Equatorial::Equatorial(const double _ra, const double _dec, ANGLE_UNIT _type) {
    if ( _type == RADS ) {
        m_phi = _ra;
        m_theta = _dec;
    }
    else {
        m_phi = MathOps::toRadians( _ra );
        m_theta = MathOps::toRadians( _dec );
    }
}

Equatorial::~Equatorial() {
}

double Equatorial::getRightAscension(ANGLE_UNIT _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_phi );
    }
    else {
        return m_phi;
    }
}

double Equatorial::getDeclination(ANGLE_UNIT _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_theta );
    }
    else {
        return m_theta;
    }
}

Vector Equatorial::getVector() const {
    return Vector(*this);
}
