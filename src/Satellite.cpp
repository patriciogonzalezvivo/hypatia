//
//  Satellite.cpp
//
//  Created by Patricio González Vivo on 6/2/18.
//

#include "Satellite.h"

#include "AstroOps.h"

Satellite::Satellite(): m_name(NULL) {
    m_bodyId = SATELLITE;
}

Satellite::Satellite(const TLE& _tle) {
    m_bodyId = SATELLITE;
    setTLE(_tle);
}

Satellite::~Satellite() {
    if (m_name != NULL) {
        delete [] m_name;
    }
}

void Satellite::setTLE(const TLE& _tle) {
    std::string name = _tle.getName();
    m_name = new char[name.length() + 1];
    strcpy(m_name, name.c_str());

    m_sgp4.setTLE(_tle);
}

char* Satellite::getName() const {
    return m_name;
}

void Satellite::compute(Observer &_obs) {
    if (m_jcentury != _obs.getJC()) {
        m_jcentury = _obs.getJC();
        
        m_eci = m_sgp4.getECI(_obs.getJD());
        m_geocentric = m_eci.getPosition(AU);
        m_heliocentric = AstroOps::toHeliocentric(_obs, m_geocentric);
        m_equatorial = AstroOps::toEquatorial( _obs, m_geocentric );
        m_ha = MathOps::normalize(AstroOps::toHourAngle( _obs, m_equatorial ), RADS);
        m_geodetic = AstroOps::toGeodetic( m_eci );
        m_horizontal = AstroOps::toHorizontal( _obs, m_equatorial );
    }
}
