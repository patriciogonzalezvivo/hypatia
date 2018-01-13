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
    
    BodyId getBodyId() const { return m_bodyId; }

    // Ecliptic (GeoCentric)
    virtual double getEclipticLon() const { return MathOps::toDegrees( m_eclipticLon ); }
    virtual double getEclipticLonRadians() const { return m_eclipticLon; }
    virtual double getEclipticLat() const { return MathOps::toDegrees( m_eclipticLat); }
    virtual double getEclipticLatRadians() const { return m_eclipticLat; }
    virtual double getRadius() const { return m_r; }
    
    // Calculate the data for a given planet, jd, and location
    // This function must be called (directly or via c'tor) before calling
    // any of the other fns!
    //
    virtual void compute( Observer& _obs );
    
protected:
    virtual void computeElipcticAngles( Observer& _obs );

    double  m_jcentury;
    double  m_r, m_eclipticLon, m_eclipticLat;
    
    BodyId  m_bodyId;
};
