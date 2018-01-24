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

#include "EqPoint.h"

class Body : public EqPoint {
public:
    Body();
    Body( BodyId _body );
    virtual ~Body();
    
    virtual BodyId  getBodyId() const { return m_bodyId; }
    virtual char*   getZodiacSign() const;

    // Heliocentric
    virtual Vector  getHeliocentricVector() const;

    // Geocentric
    virtual Vector  getGeocentricVector() const;
    virtual Vector  getEquatorialVector() const { return Vector(m_ra, m_dec, m_gEclipticRad, true); }
    virtual Vector  getHorizontalVector() const { return Vector(m_alt, m_az, m_gEclipticRad, true); }
    
    // The Radius is in AU
    virtual double  getRadius() const { return m_gEclipticRad; }
    virtual double  getEclipticLon() const { return MathOps::toDegrees(m_gEclipticLon); }
    virtual double  getEclipticLonRadians() const { return m_gEclipticLon; }
    virtual double  getEclipticLat() const { return MathOps::toDegrees(m_gEclipticLat); }
    virtual double  getEclipticLatRadians() const { return m_gEclipticLat; }
    
    // Calculate the data for a given planet, jd, and location
    // This function must be called (directly or via c'tor) before calling
    // any of the other fns!
    //
    virtual void    compute( Observer& _obs );
    
protected:
    virtual void computeElipcticAngles( Observer& _obs );
    Vector  m_heliocentricLoc;

    double  m_jcentury;
    double  m_hEclipticRad, m_hEclipticLon, m_hEclipticLat;
    double  m_gEclipticRad, m_gEclipticLon, m_gEclipticLat;
    
    BodyId  m_bodyId;
};
