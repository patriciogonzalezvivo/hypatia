
#include "hypatia/Observer.h"

#include "hypatia/MathOps.h"
#include "hypatia/CoordOps.h"

#include "hypatia/models/VSOP87.h"
#include <iostream>
#include <math.h>

Observer::Observer() :
m_cityId(0),
m_jd(0.0),
m_jcentury(0.0),
m_obliquity(0.0),
m_lst(0.0),
m_tzOffsetST(0.0),
m_tzOffsetDST(0.0),
m_tzIndex(0),
m_bLocation(false) {
}

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

void Observer::setLocation(double _lng_deg, double _lat_deg) {
    if (m_location.getLongitude(DEGS) != _lng_deg ||
        m_location.getLatitude(DEGS) != _lat_deg ) {
        m_location.setLongitude(_lng_deg, DEGS);
        m_location.setLatitude(_lat_deg, DEGS);
        m_bLocation = true;
        update();
    }
}

size_t Observer::searchLocation(double _lng_deg, double _lat_deg) {
    setLocation(Geodetic(_lng_deg, _lat_deg, 0., DEGS, KM));
    m_cityId = GeoOps::findClosestCity(_lng_deg, _lat_deg);
    if ( m_cityId == 0 ) {
        // No city found, use the current location and construct the correct timezone "Etc/GMT{+X/-X}" basedo on the longitude
        
        int timezone_number = int( m_location.getLongitude(DEGS) / 15.0);
        std::string timezone_name = "Etc/GMT";
        if ( timezone_number > 0 ) {
            timezone_name += "-" + std::to_string(abs(timezone_number));
        }
        else if ( timezone_number < 0 ) {
            timezone_name += "+" + std::to_string(abs(timezone_number));
        }
        setTimezone(timezone_name.c_str());
    } 
    else {
        setTimezoneIndex(GeoOps::getCityTimezoneIndex(m_cityId));
    }
    return m_cityId;
}

void Observer::setCityId(size_t _cityId) {
    if ( _cityId != m_cityId ) {
        m_cityId = _cityId;
        m_location = GeoOps::getCityLocation(_cityId);
        setTimezoneIndex(GeoOps::getCityTimezoneIndex(_cityId));
        m_bLocation = true;
        update();
    }
}

void Observer::setTimezoneIndex(size_t _tz) {
    m_tzIndex = _tz;
    m_tzOffsetST = GeoOps::tzOffsetInDaysST(_tz);
    m_tzOffsetDST = GeoOps::tzOffsetInDaysDST(_tz);
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

// Formula from https://en.wikipedia.org/wiki/Ascendant#Calculation
double Observer::getAscendant( ANGLE_UNIT _type ) const {
    double oL = getLST();
    double e = getObliquity();
    double lat = getLocation().getLatitude(RADS);


    double y = cos(oL);
    double x = -(sin(oL) * cos(e) + tan(lat) * sin(e));

    double asc_rad = atan2(y, x);

    return MathOps::normalize(MathOps::toDegrees(asc_rad), DEGS);
}

double Observer::getMidheaven( ANGLE_UNIT _type ) const {
    double oL = getLST();
    double e = getObliquity();
    double mc_rad = atan2( sin(oL), cos(oL) * cos(e));
    if ( _type == RADS ) {
        return MathOps::normalize(mc_rad, RADS);
    }
    return MathOps::normalize(MathOps::toDegrees(mc_rad), DEGS);
}



double Observer::getNorthNode( ANGLE_UNIT _type ) const { // Compute the true ecliptic longitude of the lunar ascending node in the requested unit.
    const double jd = getJD(); // Obtain the observer Julian day to anchor the node calculation.
    const double jCentury = TimeOps::toJC(jd); // Convert the Julian day into Julian centuries referenced to J2000 using TimeOps utilities.
    const double jCenturySq = jCentury * jCentury; // Precompute the squared Julian century term required by the polynomial model.
    const double jCenturyCu = jCenturySq * jCentury; // Precompute the cubic Julian century term required by the polynomial model.
    const double nodeLongitudeDeg = 125.04452 - 1934.136261 * jCentury + 0.0020708 * jCenturySq + jCenturyCu / 450000.0; // Evaluate the mean ecliptic longitude of the lunar ascending node in degrees (Meeus 1998, Ch. 47).
    const double normalizedLongitudeDeg = MathOps::normalize(nodeLongitudeDeg, DEGS); // Normalize the mean longitude into the canonical 0-360 degree domain.
    const double a1Deg = MathOps::normalize(119.75 + 131.849 * jCentury, DEGS); // Compute the first periodic argument used to correct the node to its true position.
    const double a2Deg = MathOps::normalize(53.09 + 479264.29 * jCentury, DEGS); // Compute the second periodic argument used to correct the node to its true position.
    const double nodeTrueDeg = normalizedLongitudeDeg + 0.0004664 * cos(MathOps::toRadians(a1Deg)) + 0.0000754 * cos(MathOps::toRadians(a2Deg)); // Apply the periodic terms that yield the true (osculating) node longitude.
    const double normalizedTrueDeg = MathOps::normalize(nodeTrueDeg, DEGS); // Normalize the corrected longitude back into the 0-360 degree range.
    if (_type == DEGS) { // Return in degrees when the caller specifies DEGS.
        return normalizedTrueDeg; // Provide the true node longitude in degrees.
    }
    return MathOps::toRadians(normalizedTrueDeg); // Convert the normalized true longitude into radians through MathOps helpers.
}

// -----------------------------------------------------------------------------