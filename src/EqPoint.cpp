
#include "EqPoint.h"
#include "AstroOps.h"

EqPoint::EqPoint() : m_ra(0.0), m_dec(0.0), m_alt(0.0), m_az(0.0) {
}

EqPoint::EqPoint( double _ra, double _dec, bool _radians ) {
    if (!_radians) {
        m_ra = MathOps::toRadians(_ra);
        m_dec = MathOps::toRadians(_dec);
    }
    else {
        m_ra = _ra;
        m_dec = _dec;
    }
    m_alt = 0.0;
    m_az = 0.0;
}

EqPoint::~EqPoint() {
}

void EqPoint::compute( Observer &_obs ) {
    AstroOps::equatorialToHorizontal( _obs, m_ra, m_dec, m_alt, m_az );
}

void EqPoint::setEqAngles( double _ra, double _dec ) {
    setEqAnglesRadians( MathOps::toRadians( _ra ), 
                        MathOps::toRadians( _dec ) );
}

void EqPoint::setEqAnglesRadians( double _ra, double _dec ) {
    m_ra = _ra;
    m_dec = _dec;
    m_az = m_alt = 0.0;
}
