#include "EqPoint.h"

#include "AstroOps.h"

#include <math.h>
#include <sstream>
#include <iomanip>

EqPoint::EqPoint() : m_ra(0.0), m_dec(0.0) {
}

EqPoint::EqPoint(const Vector& _parent) {
    m_ra = atan2(_parent.y, _parent.x);
    m_dec = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
}

EqPoint::EqPoint( double _ra, double _dec, bool _radians ) {
    setEquatorialAngles(_ra, _dec, _radians);
}

EqPoint::~EqPoint() {
}

void EqPoint::compute( Observer &_obs ) {
    m_hourAngle = _obs.getLST() - m_ra;
    AstroOps::equatorialToHorizontal( _obs, m_ra, m_dec, m_alt, m_az );
}

Vector EqPoint::getEquatorialVector() const {
    Vector v;
    v.setPolar(m_ra, m_dec, true);
    return v;
}

void EqPoint::setEquatorialAngles( double _ra, double _dec, bool _radians ) {
    if (_radians) {
        m_ra = _ra;
        m_dec = _dec;
    }
    else {
        m_ra = MathOps::toRadians( _ra );
        m_dec = MathOps::toRadians( _dec );
    }
    
    m_az = m_alt = 0.0;
}

std::string EqPoint::getString() const {
    std::stringstream ss;
    ss << std::right << std::fixed << std::setprecision(3);
    ss << HorPoint::getString();
    ss << ", RA: " << std::setw(8) << MathOps::formatDegrees(getRightAscension(), H_M_S);
    ss << ", Dec: " << std::setw(8) << MathOps::formatDegrees(getDeclination(), D_M_S);
    return ss.str();
}

