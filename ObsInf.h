/*****************************************************************************\
 * ObsInf
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
 \*****************************************************************************/

#pragma once

#include "MathOps.h"

// bodies of interest
//
enum Planet {
    NAP=-1, // NotAPlanet
    SUN=0,
    MERCURY=1, VENUS=2, EARTH=3, MARS=4, JUPITER=5, SATURN=6, URANUS=7, NEPTUNE=8, PLUTO=9,
    LUNA=10
};

// * * * * * Observer's Location Info * * * * *
class ObsInfo {
public:
    // c'tor: lon & lat are passed in DEGREES
    ObsInfo(double _lng_deg = 0, double _lat_deg = 0, Planet _planet = EARTH, unsigned long _sec = 0);
    
    void    setLatitude(double _deg);
    void    setLongitude(double _deg);
    void    setTime(unsigned long _sec = 0);
    void    setJuliaDay(double _jd);
    
    double  getLongitude();
    double  getDegLatitude();
    
    double  getLatitude();
    double  getDegLongitude();
    
    unsigned long  getTime();
    double  getJulianDate();
    double  getCenturyDate();
    double  getObliquity();
    double  getLST();
    
    AstroVector    getEclipticHelioLocation();
    
    void    update();
    
private:
    AstroVector m_eclipticHelioLocation;
    
    double m_longitude;   // in radians, N positive
    double m_latitude;    // in radians, E positive
    
    unsigned long m_sec;
    double m_jd;
    double m_century;
    double m_obliquity;
    double m_lst;
    
    Planet m_planet;
    
    bool m_change;
};
