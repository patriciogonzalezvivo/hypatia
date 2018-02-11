#include "Equatorial.h"

#include <math.h>

Equatorial::Equatorial() {
}

Equatorial::Equatorial(const Vector& _parent) {
    m_phi = atan2(_parent.y, _parent.x);
    m_theta = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
}

Equatorial::Equatorial(const double _ra, const double _dec, bool _radians) {
    if (_radians) {
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

Vector Equatorial::getVector() const {
    return Vector(*this);
}
