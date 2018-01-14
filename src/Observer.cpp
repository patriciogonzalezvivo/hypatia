
#include "Observer.h"
#include "MathOps.h"
#include "AstroOps.h"
#include "TimeOps.h"
#include "Vsop.h"

#include <string.h>
#include <iostream>

Observer::Observer( double _lng_deg, double _lat_deg, unsigned long _sec, BodyId _body ) : m_longitude(MathOps::toRadians(_lng_deg)), m_latitude(MathOps::toRadians(_lat_deg)), m_sec(_sec), m_jd(0.0), m_jcentury(0.0), m_obliquity(0.0), m_lst(0.0), m_body(_body), m_change(true) {
}

// set: lat passed in DEGREES
void Observer::setLatitude( double _deg ) {
    m_latitude = MathOps::toRadians(_deg);
    m_change = true;
}

// set: lon passed in DEGREES
void Observer::setLongitude( double _deg ) {
    m_longitude = MathOps::toRadians(_deg);
    m_change = true;
}

void Observer::setTime( unsigned long _sec ) {
    if ( _sec == 0 ) {
        _sec = TimeOps::getCurrentSeconds();
    }
    m_sec = _sec;
    setJuliaDay( TimeOps::julianDates(m_sec) );
}

void Observer::setJuliaDay( double _jd ) {
    m_jd = _jd;
    m_jcentury = TimeOps::toMillenia(m_jd);
    m_obliquity = AstroOps::meanObliquity(m_jcentury);
    m_lst = TimeOps::localSiderealTime(m_jd, MathOps::toDegrees(m_longitude));
    
    double planet_eclipticLon;
    double planet_eclipticLat;
    double planet_eclipticRad;
    Vsop::calcAllLocs(planet_eclipticLon, planet_eclipticLat, planet_eclipticRad, m_jcentury, m_body);
    m_heliocentricLoc = Vector(planet_eclipticLon, planet_eclipticLat, planet_eclipticRad);
    
    //std::cout << "Observation data UPDATED" << std::endl;
    m_change = false;
}

double Observer::getLongitudeRadians() {
    if (m_change)
        update();
    return m_longitude;
}

double Observer::getLongitude() {
    return MathOps::toDegrees( getLongitudeRadians() );
}
double Observer::getLatitudeRadians() {
    if (m_change)
        update();
    return m_latitude;
}

double Observer::getLatitude() {
    return MathOps::toDegrees( getLatitudeRadians() );
}

unsigned long Observer::getTime() {
    if (m_change)
        update();
    return m_sec;
}

double Observer::getJulianDate() {
    if (m_change)
        update();
    return m_jd;
}

double Observer::getJulianCentury() {
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
        setTime(m_sec);
    }
}
