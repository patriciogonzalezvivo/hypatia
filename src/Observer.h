/*****************************************************************************\
 * ObsInf
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
 \*****************************************************************************/

#pragma once

#include "Vector.h"

// bodies of interest
//
enum BodyId {
    NAB=-1, // NotABody
    SUN=0,
    MERCURY=1, VENUS=2, EARTH=3, MARS=4, JUPITER=5, SATURN=6, URANUS=7, NEPTUNE=8, PLUTO=9,
    LUNA=10
};

// * * * * * Observer's Location Info * * * * *
class Observer {
public:
    // c'tor: lon & lat are passed in DEGREES
    Observer(double _lng_deg = 0, double _lat_deg = 0, unsigned long _sec = 0, BodyId _body = EARTH);
    
    void    setLatitude(double _deg);
    void    setLongitude(double _deg);
    void    setTime(unsigned long _sec = 0);
    void    setJuliaDay(double _jd);
    
    double  getLongitude();
    double  getLongitudeRadians();
    
    double  getLatitude();
    double  getLatitudeRadians();
    
    unsigned long  getTime();
    double  getJulianDate();
    double  getJulianCentury();
    double  getObliquity();
    double  getLST();
    
    Vector  getEclipticHelioLocation();
    
    void    update();
    
private:
    Vector m_eclipticHelioLocation;
    
    double m_longitude;   // in radians, N positive
    double m_latitude;    // in radians, E positive
    
    unsigned long m_sec;
    double m_jd;
    double m_jcentury;
    double m_obliquity;
    double m_lst;
    
    BodyId m_body;
    
    bool m_change;
};
