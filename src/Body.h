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
    Body( BodyId _body, Observer& _obs );
    virtual ~Body();
    
    BodyId getBodyId() const { return m_bodyId; }

    // Ecliptic (GeoCentric)
    double getEclipticLon() const { return MathOps::toDegrees( m_eclipticLon ); }
    double getEclipticLonRadians() const { return m_eclipticLon; }
    double getEclipticLat() const { return MathOps::toDegrees( m_eclipticLat); }
    double getEclipticLatRadians() const { return m_eclipticLat; }
    double getRadius() const { return m_r; }
    
    // Ecuatorial (Geocentric)
    double getDeclination() const { return MathOps::toDegrees( m_dec ); }
    double getDeclinationRadians() const { return m_dec; }
    double getRightAscension() const { return MathOps::toDegrees( m_ra ); }
    double getRightAscensionRadians() const { return m_ra; }

    // Horizontal (Topocentric)
    double getAzimuth() const { return MathOps::toDegrees( m_az ); }
    double getAzimuthRadians() const { return m_az; }
    double getAltitud() const { return MathOps::toDegrees( m_alt ); }
    double getAltitudRadians() const { return m_alt; }
    
    // Calculate the data for a given planet, jd, and location
    // This function must be called (directly or via c'tor) before calling
    // any of the other fns!
    //
    virtual void update( Observer& _obs );
    
protected:
    virtual void updateGeoTopoCentric( Observer& _obs );

    double  m_jcentury;

    double  m_r;
    double  m_eclipticLon;
    double  m_eclipticLat;
    
    double  m_dec;
    double  m_ra;

    double  m_az;
    double  m_alt;
    
    BodyId  m_bodyId;
};
