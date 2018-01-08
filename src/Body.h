/*****************************************************************************\
 * Body.h
 *
 * Handles planetary motion calculations and conversions
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#pragma once

#include "Observer.h"

class Body {
public:
    Body();
    
    Body( BodyName _body, Observer& _obs );
    
    BodyName getBody() const { return m_bodyName; }

    // Ecliptic (GeoCentric)
    double getEclipticLon() const { return ( NAB == m_bodyName ) ? -1. : MathOps::toDegrees( m_eclipticLon ); }
    double getEclipticLonRadians() const { return ( NAB == m_bodyName ) ? -1. : m_eclipticLon; }
    double getEclipticLat() const { return ( NAB == m_bodyName ) ? -1. : MathOps::toDegrees( m_eclipticLat); }
    double getEclipticLatRadians() const { return ( NAB == m_bodyName ) ? -1. : m_eclipticLat; }
    double getRadius() const { return ( NAB == m_bodyName ) ? -1. : m_r; }
    
    // Ecuatorial (Geocentric)
    double getDeclination() const { return ( NAB == m_bodyName ) ? -1. : MathOps::toDegrees( m_dec ); }
    double getDeclinationRadians() const { return ( NAB == m_bodyName ) ? -1. : m_dec; }
    double getRightAscension() const { return ( NAB == m_bodyName ) ? -1. : MathOps::toDegrees( m_ra ); }
    double getRightAscensionRadians() const { return ( NAB == m_bodyName ) ? -1. : m_ra; }

    // Horizontal (Topocentric)
    double getAzimuth() const { return ( NAB == m_bodyName ) ? -1. : MathOps::toDegrees( m_az ); }
    double getAzimuthRadians() const { return ( NAB == m_bodyName ) ? -1. : m_az; }
    double getAltitud() const { return ( NAB == m_bodyName ) ? -1. : MathOps::toDegrees( m_alt ); }
    double getAltitudRadians() const { return ( NAB == m_bodyName ) ? -1. : m_alt; }
    
    // Calculate the data for a given planet, jd, and location
    // This function must be called (directly or via c'tor) before calling
    // any of the other fns!
    //
    void update( Observer& _obs );
    
protected:
    double m_jd;
    double m_r;
    double m_eclipticLon;
    double m_eclipticLat;
    
    double m_az;
    double m_alt;
    double m_dec;
    double m_ra;
    
    BodyName m_bodyName;
};
