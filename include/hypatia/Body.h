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
    LUNA=10,
    SATELLITE=11
};

class Body {
public:
    Body();
    Body( BodyId _body );
    virtual ~Body();
    
    virtual BodyId      getId() const { return m_bodyId; }
    virtual char*       getName() const;
    virtual char*       getZodiacSign() const;
    virtual double      getPeriod(TIME_UNIT _unit) const;
    virtual double      getHourAngle(ANGLE_UNIT _type) const;
    
    // Heliocentric
    virtual Ecliptic    getEclipticHeliocentric() const { return m_heliocentric; }

    // Geocentric
    virtual Ecliptic    getEclipticGeocentric() const { return m_geocentric; }
    
    virtual Equatorial  getEquatorial() const { return m_equatorial; }
    virtual Vector3     getEquatorialVector(DISTANCE_UNIT _type) const { return m_equatorial.getVector() * m_geocentric.getRadius(_type); }
    
    virtual bool        haveHorizontal() const { return m_bHorizontal; }
    virtual Horizontal  getHorizontal() const { return m_horizontal; }
    virtual Vector3     getHorizontalVector(DISTANCE_UNIT _type) const { return m_horizontal.getVector() * m_geocentric.getRadius(_type); };
    
    //  Calculate the data for a given planet given an observer
    //  - This function must be called (directly or via c'tor) before calling any of the other fns!
    //  - If the observer have no location Horizontal coordinates will not be calculate and remain 0.0, 0.0
    //
    virtual void        compute( Observer& _obs );
    
protected:
    Ecliptic    m_heliocentric;
    Ecliptic    m_geocentric;
    
    Equatorial  m_equatorial;
    Horizontal  m_horizontal;
    
    double      m_jcentury;
    double      m_ha;
    BodyId      m_bodyId;
    
    bool        m_bHorizontal;
};

inline std::ostream& operator<<(std::ostream& strm, const Body& b) {
    strm << b.getName() << ", ";
    strm << b.getEclipticHeliocentric() << ", ";
    strm << b.getEquatorial();
    
    if ( b.haveHorizontal() ) {
        strm << ", " << b.getHorizontal();
    }
    
    return strm;
}
