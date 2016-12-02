/*****************************************************************************\
 * PlanetData.h
 *
 * Handles planetary motion calculations and conversions
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#pragma once

#include "ObsInf.h"

class PlanetData {
public:
    PlanetData();
    
    PlanetData( Planet _planet, ObsInfo& _obs );
    
    Planet getPlanet() const { return m_planet; }
    double getJulianDay() const { return ( NAP == m_planet ) ? -1. : m_jd; }

    double getEclipticLon() const { return ( NAP == m_planet ) ? -1. : m_eclipticLon; }
    double getEclipticLat() const { return ( NAP == m_planet ) ? -1. : m_eclipticLat; }
    double getRadius() const { return ( NAP == m_planet ) ? -1. : m_r; }
    
    double getAzimuth() const { return ( NAP == m_planet ) ? -1. : m_az; }
    double getAltitud() const { return ( NAP == m_planet ) ? -1. : m_alt; }
    
    double getDeclination() const { return ( NAP == m_planet ) ? -1. : m_dec; }
    double getRightAscension() const { return ( NAP == m_planet ) ? -1. : m_ra; }
    
    // Calculate the data for a given planet, jd, and location
    // This function must be called (directly or via c'tor) before calling
    // any of the other fns!
    //
    void update( ObsInfo& _obs );
    
protected:
    double m_jd;
    double m_r;
    double m_eclipticLon;
    double m_eclipticLat;
    
    double m_az;
    double m_alt;
    double m_dec;
    double m_ra;
    
    Planet m_planet;
};
