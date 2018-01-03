
#include "ObsInf.h"
#include "MathOps.h"
#include "AstroOps.h"
#include "TimeOps.h"
#include "Vsop.h"

#include <string.h>
#include <iostream>

ObsInfo::ObsInfo( double _lng_deg, double _lat_deg, Planet _planet, unsigned long _sec) : m_longitude(MathOps::toRadians(_lng_deg)), m_latitude(MathOps::toRadians(_lat_deg)), m_sec(_sec), m_jd(0.0), m_century(0.0), m_obliquity(0.0), m_lst(0.0), m_planet(_planet), m_change(true) {
}

// set: lat passed in DEGREES
void ObsInfo::setLatitude(double _deg) {
    m_latitude = MathOps::toRadians(_deg);
    m_change = true;
}

// set: lon passed in DEGREES
void ObsInfo::setLongitude(double _deg) {
    m_longitude = MathOps::toRadians(_deg);
    m_change = true;
}

void ObsInfo::setTime(unsigned long _sec) {
    if (_sec == 0) {
        _sec = TimeOps::getCurrentSeconds();
    }
    m_sec = _sec;
    setJuliaDay( TimeOps::julianDates(m_sec) );
}

void ObsInfo::setJuliaDay (double _jd ) {
    m_jd = _jd;
    m_century = TimeOps::toMillenia(m_jd);
    m_obliquity = AstroOps::meanObliquity(m_century);
    m_lst = TimeOps::localSiderealTime(m_jd, MathOps::toDegrees(m_longitude));
    
    double planet_eclipticLon;
    double planet_eclipticLat;
    double planet_eclipticRad;
    Vsop::calcAllLocs(planet_eclipticLon, planet_eclipticLat, planet_eclipticRad, m_century, m_planet);
    m_eclipticHelioLocation = AstroVector(planet_eclipticLon, planet_eclipticLat, planet_eclipticRad);
    
    //std::cout << "Observation data UPDATED" << std::endl;
    m_change = false;
}

double ObsInfo::getLongitude() {
    if (m_change)
        update();
    return m_longitude;
}

double ObsInfo::getDegLongitude() {
    return MathOps::toDegrees(getLongitude());
}

double ObsInfo::getLatitude() {
    if (m_change)
        update();
    return m_latitude;
}

double ObsInfo::getDegLatitude() {
    return MathOps::toDegrees(getLatitude());
}

unsigned long ObsInfo::getTime() {
    if (m_change)
        update();
    return m_sec;
}

double ObsInfo::getJulianDate() {
    if (m_change)
        update();
    return m_jd;
}

double ObsInfo::getCenturyDate() {
    if (m_change)
        update();
    return m_century;
}

double ObsInfo::getObliquity() {
    if (m_change)
        update();
    return m_obliquity;
}

double ObsInfo::getLST() {
    if (m_change)
        update();
    return m_lst;
}

AstroVector ObsInfo::getEclipticHelioLocation() {
    if (m_change)
        update();
    return m_eclipticHelioLocation;
}

void ObsInfo::update() {
    if (m_change) {
        setTime(m_sec);
    }
}
