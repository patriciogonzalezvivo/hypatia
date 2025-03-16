
#include "hypatia/Observer.h"

#include "hypatia/MathOps.h"
#include "hypatia/CoordOps.h"

#include "hypatia/models/VSOP87.h"
#include <iostream>
#include <math.h>


Observer::Observer(double _jd) :
m_cityId(0),
m_jd(0.0),
m_jcentury(0.0),
m_obliquity(0.0),
m_lst(0.0),
m_tzOffsetST(0.0),
m_tzOffsetDST(0.0),
m_tzIndex(0),
m_bLocation(false) {
    if ( _jd == 0 ) {
        setJD( TimeOps::now(UTC) );
    }
    else {
        setJD( _jd );
    }
}

Observer::Observer(const Geodetic& _location, double _jd) :
m_location(_location),
m_cityId(0),
m_jd(0.0),
m_jcentury(0.0),
m_obliquity(0.0),
m_lst(0.0),
m_tzOffsetST(0.0),
m_tzOffsetDST(0.0),
m_tzIndex(0),
m_bLocation(true) {
    if ( _jd == 0 ) {
        setJD( TimeOps::now(UTC) );
    }
    else {
        setJD( _jd );
    }
}

Observer::Observer( double _lng_deg, double _lat_deg, double _jd) :
m_location(_lng_deg, _lat_deg, 0., DEGS, KM),
m_cityId(0),
m_jd(0.0),
m_jcentury(0.0),
m_obliquity(0.0),
m_lst(0.0),
m_tzOffsetST(0.0),
m_tzOffsetDST(0.0),
m_tzIndex(0),
m_bLocation(true) {
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

void Observer::setJDLocal(double _jd) {
    if ( !haveLocation() || m_tzIndex == 0 ) {
        setJD(_jd);
        return;
    }

    int month, day, year;
    TimeOps::toDMY(_jd, day, month, year);
    setJD(_jd - (GeoOps::tzIsDST(m_location.getLatitude(RADS), month, day)? m_tzOffsetDST : m_tzOffsetST));
}

void Observer::setTimezone(const char* _tz) {
    size_t tzIndex = GeoOps::tzNameToIndex(_tz);
    if ( tzIndex != m_tzIndex )
        setTimezoneIndex(tzIndex);
}

void Observer::setLocation(const Geodetic &_location) {
    if (m_location.getLongitude(RADS) != _location.getLongitude(RADS) ||
        m_location.getLatitude(RADS) != _location.getLatitude(RADS) ) {
        m_location = _location;
        m_bLocation = true;
        update();
    }
}

void Observer::setTimezoneIndex(size_t _tz) {
    m_tzIndex = _tz;
    m_tzOffsetST = GeoOps::tzOffsetInDaysST(_tz);
    m_tzOffsetDST = GeoOps::tzOffsetInDaysDST(_tz);
}



bool Observer::searchLocation(double _lng_deg, double _lat_deg) {
    // std::cout << "Searching location: " << _lng_deg << ", " << _lat_deg << std::endl;
    setLocation(Geodetic(_lng_deg, _lat_deg, 0., DEGS, KM));
    m_cityId = GeoOps::findClosestCity(_lng_deg, _lat_deg);
    setTimezoneIndex(GeoOps::getCityTimezoneIndex(m_cityId));
    return m_cityId != 0;
}

double Observer::getJDLocal() const { 
    if ( !haveLocation() )
        return m_jd;

    int month, day, year;
    TimeOps::toDMY(m_jd, day, month, year);
    return m_jd + (GeoOps::tzIsDST(m_location.getLatitude(RADS), month, day)? m_tzOffsetDST : m_tzOffsetST);
}

void Observer::update() {
    m_jcentury = TimeOps::toJC(m_jd);
    m_obliquity = CoordOps::meanObliquity(m_jcentury);
    
    if ( haveLocation() ) {
        m_lst = TimeOps::toLocalSideralTime(m_jd, m_location.getLongitude(RADS), RADS);
    }
    
    m_changed = true;
}

Geodetic Observer::getLocation() const {
    if ( haveLocation() ) {
        return m_location;
    }
    else {
        return Geodetic();
    }
}

double Observer::getLST() const {
    if ( haveLocation() ) {
        return m_lst;
    }
    else {
        return 0.0;
    }
}

std::string Observer::getTimezone() const {
    return std::string( GeoOps::tzIndexToName(m_tzIndex) );
}

Vector3 Observer::getHeliocentricVector(DISTANCE_UNIT _type) {
    if (m_changed) {
        double pLng, pLat, pRad = 0.0;
        VSOP87::calcAllLocs(pLng, pLat, pRad, m_jcentury, EARTH);
        m_heliocentricLoc = Ecliptic(pLng, pLat, pRad, RADS, AU).getVector(_type);
        m_changed = false;
    }
    
    return m_heliocentricLoc;
}
