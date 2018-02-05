#include "EqPoint.h"

#include <math.h>
#include "AstroOps.h"

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
    const double cosLat = cos(m_dec);
    
    Vector v;
    v.x = cos(m_ra) * cosLat;
    v.y = sin(m_ra) * cosLat;
    v.z = sin(m_dec);
    
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

