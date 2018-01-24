
#include "EqPoint.h"
#include "AstroOps.h"

EqPoint::EqPoint() : m_ra(0.0), m_dec(0.0), m_alt(0.0), m_az(0.0) {
}

EqPoint::EqPoint( double _ra_deg, double _dec_deg ) {
    m_ra = MathOps::toRadians(_ra_deg);
    m_dec = MathOps::toRadians(_dec_deg);
    m_alt = 0.0;
    m_az = 0.0;
}

EqPoint::~EqPoint() {
}

double EqPoint::setInRadians(double _ra_rad, double _dec_rad) {
    m_ra = _ra_rad;
    m_dec = _dec_rad;
    m_alt = 0.0;
    m_az = 0.0;
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
