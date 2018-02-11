#include "Horizontal.h"

#include <math.h>

Horizontal::Horizontal() {
}

Horizontal::Horizontal(const Vector& _parent) {
    m_phi = atan2(_parent.y, _parent.x);
    m_theta = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
}

Horizontal::Horizontal(const double _alt, const double _az, bool _radians) {
    if (_radians) {
        m_phi = _alt;
        m_theta = _az;
    }
    else {
        m_phi = MathOps::toRadians( _alt );
        m_theta = MathOps::toRadians( _az );
    }
}

Horizontal::~Horizontal() {
}

Vector Horizontal::getVector() const {
    return Vector(Polar(m_phi, -m_theta, true));
}
