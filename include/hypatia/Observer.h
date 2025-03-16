/*****************************************************************************\
 * ObsInf
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
 \*****************************************************************************/

#pragma once

#include "TimeOps.h"
#include "GeoOps.h"
#include "coordinates/Geodetic.h"

// * * * * * Observer's Location Info * * * * *
class Observer {
public:
    // c'tor: lon & lat are passed in DEGREES
    Observer(double _jd = 0);
    Observer(const Geodetic& _location, double _jd = 0);
    Observer(double _lng_deg, double _lat_deg, double _jd = 0);
    virtual ~Observer();
    
    virtual void        setSeconds(unsigned long _sec = 0);
    virtual void        setLocation(const Geodetic& _location);
    virtual void        setJD(double _jd);
    virtual void        setJDLocal(double _jd);

    virtual void        setTimezone(const char* _tz);
    virtual void        setTimezoneIndex(size_t _tz);

    virtual bool        searchLocation(double _lng_deg, double _lat_deg);
    virtual size_t      getCityId() const { return m_cityId; }
    virtual std::string getCity() const { return GeoOps::getCityName(m_cityId); }
    virtual std::string getCountry() const { return GeoOps::getCountryName( GeoOps::getCityCountryIndex(m_cityId) ); }
    virtual std::string getCountry2LetterCode() const { return GeoOps::getCountry2LetterCode( GeoOps::getCityCountryIndex(m_cityId) ); }
    
    virtual double      getJD() const { return m_jd; }
    virtual double      getJDLocal() const;
    virtual double      getJC() const { return m_jcentury; }
    virtual double      getObliquity() const { return m_obliquity; }
    
    virtual bool        haveLocation() const { return m_bLocation; }
    virtual Geodetic    getLocation() const;
    virtual double      getLST() const;

    virtual double      getTZOffsetST() const { return m_tzOffsetST; }
    virtual double      getTZOffsetDST() const { return m_tzOffsetDST; }
    virtual size_t      getTimezoneIndex() const { return m_tzIndex; }
    virtual std::string getTimezone() const;
    
    virtual Vector3     getHeliocentricVector(DISTANCE_UNIT _type);
    
    virtual void        update();

private:
    Vector3             m_heliocentricLoc;
    Geodetic            m_location;
    size_t              m_cityId;

    double              m_jd;
    double              m_jcentury;
    double              m_obliquity;
    double              m_lst;

    double              m_tzOffsetST;
    double              m_tzOffsetDST;
    size_t              m_tzIndex;
    
    bool                m_changed;
    bool                m_bLocation;
};

inline std::ostream& operator<<(std::ostream& strm, const Observer& o) {
    strm << "Observer, jd:" << std::setw(8) << o.getJD();
    strm << ", " << std::setw(8) << TimeOps::formatDateTime(o.getJD(), Y_M_D_HM);
    strm << ", obliq:" << std::setw(8) << MathOps::formatAngle(o.getObliquity(), RADS, Dd);
    if ( o.haveLocation() ) {
        strm << ", " << o.getLocation();
        strm << ", lst:" << std::setw(8) << MathOps::formatAngle(o.getLST(), DEGS, Dd);
    }
    return strm;
}
