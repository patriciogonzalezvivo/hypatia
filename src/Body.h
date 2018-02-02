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

#include "EcPoint.h"
#include "EqPoint.h"

class Body : public EqPoint {
public:
    Body();
    Body( BodyId _body );
    virtual ~Body();
    
    virtual BodyId  getBodyId() const { return m_bodyId; }
    virtual char*   getBodyName() const;
    virtual char*   getZodiacSign() const;

    // Heliocentric
    virtual EcPoint getHeliocentricEcliptic() const { return m_heliocentric; }
    virtual Vector  getHeliocentricVector() const { return m_heliocentric.getEclipticVector(); }

    // Geocentric
    virtual EcPoint getGeocentricEcliptic() const { return m_geocentric; }
    virtual Vector  getGeocentricVector() const { return m_geocentric.getEclipticVector(); }
    
    virtual Vector  getEquatorialVector() const { return EqPoint::getEquatorialVector() * m_geocentric.getRadius(); }
    virtual Vector  getHorizontalVector() const { return HorPoint::getHorizontalVector() * m_geocentric.getRadius(); };
    
    // Calculate the data for a given planet, jd, and location
    // This function must be called (directly or via c'tor) before calling
    // any of the other fns!
    //
    virtual void    compute( Observer& _obs );
    
protected:
    virtual void computeElipcticAngles( Observer& _obs );

    double  m_jcentury;
    EcPoint m_heliocentric;
    EcPoint m_geocentric;
    
    BodyId  m_bodyId;
};
