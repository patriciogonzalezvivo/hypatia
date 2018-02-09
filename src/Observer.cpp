
#include "Observer.h"

#include "MathOps.h"
#include "AstroOps.h"
#include "TimeOps.h"

#include "Vsop.h"

#include "EcPoint.h"

Observer::Observer() : m_jd(0.0), m_jcentury(0.0), m_obliquity(0.0), m_lst(0.0) {
}

Observer::Observer(const GeoPoint& _location, double _jd) : m_location(_location), m_jd(0.0), m_jcentury(0.0), m_obliquity(0.0), m_lst(0.0) {
    if ( _jd == 0 ) {
        setJD(TimeOps::now( true ));
    }
    else {
        setJD( _jd );
    }
}

Observer::Observer( double _lng_deg, double _lat_deg, double _jd) : m_location(_lng_deg, _lat_deg), m_jd(0.0), m_jcentury(0.0), m_obliquity(0.0), m_lst(0.0){
    if ( _jd == 0 ) {
        setJD(TimeOps::now( true ));
    }
    else {
        setJD( _jd );
    }
}

Observer::~Observer() {
}

void Observer::setJD(double _jd) {
    if ( m_jd != _jd ) {
        m_jd = _jd;
        m_jcentury = TimeOps::toJC(m_jd);
        m_obliquity = AstroOps::meanObliquity(m_jcentury);
        m_lst = TimeOps::toLST(m_jd, m_location.getLongitudeRadians(), true);
        
        m_changed = true;
    }
}

Vector Observer::getHeliocentricVector() {
    if (m_changed) {
        double pLng, pLat, pRad = 0.0;
        Vsop::calcAllLocs(pLng, pLat, pRad, m_jcentury, EARTH);
        m_heliocentricLoc = EcPoint(pLng, pLat, pRad, true).getVector();
        m_changed = false;
    }
    
    return m_heliocentricLoc;
}
