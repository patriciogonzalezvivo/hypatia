/*****************************************************************************\
 * ObsInf
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
 \*****************************************************************************/

#pragma once

#include "Vector.h"

enum BodyId {
    NAB=-1, // NotABody
    MERCURY=0, VENUS=1, EARTH=2, MARS=3, JUPITER=4, SATURN=5, URANUS=6, NEPTUNE=7, PLUTO=8,
    LUNA=9,
    SUN=10
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
    
    Vector  getHeliocentricVector();
    
    void    update();
    
private:
    Vector m_heliocentricLoc;
    
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
