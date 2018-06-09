//
//  Satellite.cpp
//
//  Created by Patricio González Vivo on 6/2/18.
//

#include "Satellite.h"

#include "CoordOps.h"

Satellite::Satellite(): m_name("NAN") {
    m_bodyId = SATELLITE;
}

Satellite::Satellite(const TLE& _tle) {
    m_bodyId = SATELLITE;
    setTLE(_tle);
}

Satellite::~Satellite() {
}

void Satellite::setTLE(const TLE& _tle) {
    m_sgp4.setTLE(_tle);
    
    m_name = _tle.getName();
}

char* Satellite::getName() const {
    return (char *)m_name.c_str();
}

void Satellite::compute(Observer &_obs) {
    if (m_jcentury != _obs.getJC()) {
        m_jcentury = _obs.getJC();
        
        m_eci = m_sgp4.getECI(_obs.getJD());
        
        // TODO:
        //   - ECI to GEOCENTRIC
        //
//        m_geocentric = m_eci.getPosition(AU);
//        m_heliocentric = CoordOps::toHeliocentric(_obs, m_geocentric);
//        m_equatorial = CoordOps::toEquatorial( _obs, m_geocentric );
        m_equatorial = Equatorial(m_eci.getPosition(AU));
        m_ha = MathOps::normalize(CoordOps::toHourAngle( _obs, m_equatorial ), RADS);
        m_geodetic = CoordOps::toGeodetic( m_eci );
        m_horizontal = CoordOps::toHorizontal( _obs, m_equatorial );
    }
}
