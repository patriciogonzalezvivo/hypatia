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

#include "coordinates/Ecliptic.h"
#include "coordinates/Equatorial.h"
#include "coordinates/Horizontal.h"

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
    virtual double  getHourAngle(ANGLE_TYPE _type) const;

    // Heliocentric
    virtual Ecliptic    getEclipticHeliocentric() const { return m_heliocentric; }

    // Geocentric
    virtual Ecliptic    getEclipticGeocentric() const { return m_geocentric; }
    
    virtual Equatorial  getEquatorial() const { return m_equatorial; }
    virtual Vector      getEquatorialVector() const { return m_equatorial.getVector() * m_geocentric.getRadius(); }
    
    virtual Horizontal  getHorizontal() const { return m_horizontal; }
    virtual Vector      getHorizontalVector() const { return m_horizontal.getVector() * m_geocentric.getRadius(); };
    
    // Calculate the data for a given planet, jd, and location
    // This function must be called (directly or via c'tor) before calling
    // any of the other fns!
    //
    virtual void    compute( Observer& _obs );
    
protected:
    Ecliptic    m_heliocentric;
    Ecliptic    m_geocentric;
    
    Equatorial  m_equatorial;
    Horizontal  m_horizontal;
    
    double      m_jcentury;
    double      m_ha;
    BodyId      m_bodyId;
};

inline std::ostream& operator<<(std::ostream& strm, const Body& b) {
    strm << b.getBodyName() << ", ";
    strm << b.getEclipticHeliocentric() << ", ";
    strm << b.getEquatorial() << ", ";
    strm << b.getHorizontal();

    return strm;
}
