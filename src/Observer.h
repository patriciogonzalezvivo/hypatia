/*****************************************************************************\
 * ObsInf
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
 \*****************************************************************************/

#pragma once

#include "TimeOps.h"
#include "GeoPoint.h"

// * * * * * Observer's Location Info * * * * *
class Observer {
public:
    // c'tor: lon & lat are passed in DEGREES
    Observer();
    Observer(const GeoPoint& _location, double _jd = 0);
    Observer(double _lng_deg, double _lat_deg, double _jd = 0);
    virtual ~Observer();
    
    void    setJD(double _jd);
    
    double  getJD() const { return m_jd; }
    double  getJC() const { return m_jcentury; }
    double  getLST() const { return m_lst; }
    double  getObliquity() const { return m_obliquity; }
    GeoPoint getLocation() const { return m_location; }
    
    Vector  getHeliocentricVector();

private:
    Vector      m_heliocentricLoc;
    GeoPoint    m_location;
    
    double      m_jd;
    double      m_jcentury;
    double      m_obliquity;
    double      m_lst;
    
    bool        m_changed;
};

inline std::ostream& operator<<(std::ostream& strm, const Observer& o) {
    strm << "Observer, " << o.getLocation();
    strm << ", jd:" << std::setw(8) << o.getJD();
    strm << ", time:" << std::setw(8) << TimeOps::formatDateTime(o.getJD(), Y_M_D_HM);
    strm << ", obliq:" << std::setw(8) << MathOps::formatRadians(o.getObliquity(), Dd);
    strm << ", lst:" << std::setw(8) << MathOps::formatDegrees(o.getLST(), Dd);
}

