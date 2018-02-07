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
     setHorizontalAngles(_alt, _az, _radians);
}

HorPoint::~HorPoint() {
}

void HorPoint::setHorizontalAngles( double _alt, double _az, bool _radians ) {
    if (_radians) {
        m_alt = _alt;
        m_az = _az;
    }
    else {
        m_alt = MathOps::toRadians( _alt );
        m_az = MathOps::toRadians( _az );
    }
}

Vector HorPoint::getHorizontalVector() const {
    Vector v;
    v.setPolar(m_alt, m_az, true);
    return v;
}

std::string HorPoint::getString() const {
    std::stringstream ss;
    ss << std::right << std::fixed << std::setprecision(3);
    ss << ", Alt: " << std::setw(8) << MathOps::formatDegrees(getAltitud(), DEGREES);
    ss << ", Az: " << std::setw(8) << MathOps::formatDegrees(getAzimuth(), DEGREES);
    return ss.str();
}
