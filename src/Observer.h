/*****************************************************************************\
 * ObsInf
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
 \*****************************************************************************/

#pragma once

#include "GeoPoint.h"

// * * * * * Observer's Location Info * * * * *
class Observer {
public:
    // c'tor: lon & lat are passed in DEGREES
    Observer();
    Observer(const GeoPoint& _location, unsigned long _sec = 0);
    Observer(double _lng_deg, double _lat_deg, unsigned long _sec = 0);
    virtual ~Observer();
    
    void    setLocation(const GeoPoint& _location);
    
    void    setSeconds(unsigned long _sec = 0);
    void    setJD(double _jd);
    
    GeoPoint getLocation() const { return m_location; };
    
    unsigned long  getSeconds();
    double  getJD();
    double  getJC();
    double  getObliquity();
    double  getLST();
    
    Vector  getHeliocentricVector();
    
    virtual std::string getString() const;
    
    void    update();
    
private:
    Vector      m_heliocentricLoc;
    GeoPoint    m_location;
    
    unsigned long m_sec;
    double m_jd;
    double m_jcentury;
    double m_obliquity;
    double m_lst;
    
    bool m_change;
};
