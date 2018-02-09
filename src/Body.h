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
#include "HorPoint.h"

#include "Observer.h"

enum BodyId {
    NAB=-1, // NotABody
    SUN=0,
    MERCURY=1, VENUS=2, EARTH=3, MARS=4, JUPITER=5, SATURN=6, URANUS=7, NEPTUNE=8, PLUTO=9,
    LUNA=10
};

class Body {
public:
    Body();
    Body( BodyId _body );
    virtual ~Body();
    
    virtual BodyId  getBodyId() const { return m_bodyId; }
    virtual char*   getBodyName() const;
    virtual char*   getZodiacSign() const;

    // Heliocentric
    virtual EcPoint getEclipticHeliocentric() const { return m_heliocentric; }

    // Geocentric
    virtual EcPoint getEclipticGeocentric() const { return m_geocentric; }
    
    virtual EqPoint getEquatorial() const { return m_equatorial; }
    virtual Vector  getEquatorialVector() const { return m_equatorial.getVector() * m_geocentric.getRadius(); }
    
    virtual HorPoint getHorizontal() const { return m_horizontal; }
    virtual Vector  getHorizontalVector() const { return m_horizontal.getVector() * m_geocentric.getRadius(); };
    
    // Calculate the data for a given planet, jd, and location
    // This function must be called (directly or via c'tor) before calling
    // any of the other fns!
    //
    virtual void    compute( Observer& _obs );
    
protected:

    double  m_jcentury;
    EcPoint m_heliocentric;
    EcPoint m_geocentric;
    
    EqPoint m_equatorial;
    HorPoint m_horizontal;
    
    BodyId  m_bodyId;
};

inline std::ostream& operator<<(std::ostream& strm, const Body& b) {
    strm << b.getBodyName() << ", ";
    strm << b.getEclipticHeliocentric() << ", ";
    strm << b.getEquatorial() << ", ";
    strm << b.getHorizontal();
}
