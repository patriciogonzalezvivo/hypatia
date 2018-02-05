
#include "Observer.h"
#include "MathOps.h"
#include "AstroOps.h"
#include "TimeOps.h"
#include "Vsop.h"

#include "EcPoint.h"

#include <string.h>
#include <iostream>

Observer::Observer() : m_sec(0.0), m_jd(0.0), m_jcentury(0.0), m_obliquity(0.0), m_lst(0.0), m_change(true) {
}

Observer::Observer(const GeoPoint& _location, unsigned long _sec) :  m_sec(_sec), m_jd(0.0), m_jcentury(0.0), m_obliquity(0.0), m_lst(0.0), m_change(true) {
    m_location = _location;
}

Observer::Observer( double _lng_deg, double _lat_deg, unsigned long _sec ) : m_sec(_sec), m_jd(0.0), m_jcentury(0.0), m_obliquity(0.0), m_lst(0.0), m_change(true) {
    m_location = GeoPoint(_lng_deg, _lat_deg);
}

Observer::~Observer() {
}


void Observer::setLocation(const GeoPoint& _location) {
    m_location = _location;
    m_change = true;
}

void Observer::setSeconds( unsigned long _sec ) {
    if ( _sec == 0 ) {
        _sec = TimeOps::getCurrentSeconds();
    }
    m_sec = _sec;
    setJD( TimeOps::toJD(m_sec) );
}

void Observer::setJD( double _jd ) {
    m_jd = _jd;
    m_jcentury = TimeOps::toJC(m_jd);
    m_obliquity = AstroOps::meanObliquity(m_jcentury);
    m_lst = TimeOps::toLST(m_jd, m_location.getLongitude());
    
    double pLng, pLat, pRad = 0.0;
    Vsop::calcAllLocs(pLng, pLat, pRad, m_jcentury, m_location.getBody());
    m_heliocentricLoc = EcPoint(pLng, pLat, pRad, true).getEclipticVector();
    
    //std::cout << "Observation data UPDATED" << std::endl;
    m_change = false;
}

unsigned long Observer::getSeconds() {
    if (m_change)
        update();
    return m_sec;
}

double Observer::getJD() {
    if (m_change)
        update();
    return m_jd;
}

double Observer::getJC() {
    if (m_change)
        update();
    return m_jcentury;
}

double Observer::getObliquity() {
    if (m_change)
        update();
    return m_obliquity;
}

double Observer::getLST() {
    if (m_change)
        update();
    return m_lst;
}

Vector Observer::getHeliocentricVector() {
    if (m_change)
        update();
    return m_heliocentricLoc;
}

void Observer::update() {
    if (m_change) {
        setSeconds(m_sec);
    }
}
