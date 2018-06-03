
#include "Observer.h"

#include "MathOps.h"
#include "AstroOps.h"
#include "TimeOps.h"

#include "models/VSOP87.h"

Observer::Observer() : m_jd(0.0), m_jcentury(0.0), m_obliquity(0.0), m_lst(0.0) {
}

Observer::Observer(const Geodetic& _location, double _jd) : m_location(_location), m_jd(0.0), m_jcentury(0.0), m_obliquity(0.0), m_lst(0.0) {
    if ( _jd == 0 ) {
        setJD( TimeOps::now(UTC) );
    }
    else {
        setJD( _jd );
    }
}

Observer::Observer( double _lng_deg, double _lat_deg, double _jd) : m_location(_lng_deg, _lat_deg, 0., DEGS, KM), m_jd(0.0), m_jcentury(0.0), m_obliquity(0.0), m_lst(0.0){
    if ( _jd == 0 ) {
        setJD( TimeOps::now(UTC) );
    }
    else {
        setJD( _jd );
    }
}

Observer::~Observer() {
}

void Observer::setSeconds(unsigned long _sec) {
    if ( _sec == 0 ) {
        _sec = TimeOps::nowSeconds();
    }
    setJD( TimeOps::toJD(_sec) );
}

void Observer::setJD(double _jd) {
    if ( m_jd != _jd ) {
        m_jd = _jd;
        update();
    }
}

void Observer::setLocation(const Geodetic &_location) {
    if (m_location.getLongitude(RADS) != _location.getLongitude(RADS) ||
        m_location.getLatitude(RADS) != _location.getLatitude(RADS) ) {
        m_location = _location;
        update();
    }
}

void Observer::update() {
    m_jcentury = TimeOps::toJC(m_jd);
    m_obliquity = AstroOps::meanObliquity(m_jcentury);
    m_lst = TimeOps::toLocalSideralTime(m_jd, m_location.getLongitude(RADS), RADS);
    
    m_changed = true;
}

Vector Observer::getHeliocentricVector(UNIT_TYPE _type) {
    if (m_changed) {
        double pLng, pLat, pRad = 0.0;
        VSOP87::calcAllLocs(pLng, pLat, pRad, m_jcentury, EARTH);
        m_heliocentricLoc = Ecliptic(pLng, pLat, pRad, RADS, AU).getVector(_type);
        m_changed = false;
    }
    
    return m_heliocentricLoc;
}
