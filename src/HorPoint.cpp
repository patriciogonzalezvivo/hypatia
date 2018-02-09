#include "HorPoint.h"

#include <math.h>
#include <sstream>
#include <iomanip>

HorPoint::HorPoint() :  m_alt(0.0), m_az(0.0) {
}

HorPoint::HorPoint(const Vector& _parent) {
    m_alt = atan2(_parent.y, _parent.x);
    m_az = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
}

HorPoint::HorPoint( double _alt, double _az, bool _radians ) {
    if (_radians) {
        m_alt = _alt;
        m_az = _az;
    }
    else {
        m_alt = MathOps::toRadians( _alt );
        m_az = MathOps::toRadians( _az );
    }
}

HorPoint::~HorPoint() {
}

Vector HorPoint::getVector() const {
    Vector v;
    v.setPolar(m_alt, -m_az, true);
    return v;
}
