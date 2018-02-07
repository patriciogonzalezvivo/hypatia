#pragma once

#include "MathOps.h"
#include "Vector.h"

#include <string>

enum BodyId {
    NAB=-1, // NotABody
    SUN=0,
    MERCURY=1, VENUS=2, EARTH=3, MARS=4, JUPITER=5, SATURN=6, URANUS=7, NEPTUNE=8, PLUTO=9,
    LUNA=10
};

class GeoPoint {
public:
    GeoPoint();
    GeoPoint( double _lng, double _lat, double _alt = 0.0, bool _radians = false, BodyId _body = EARTH );
    virtual ~GeoPoint();
    
    virtual double  getLongitude() const { return MathOps::toDegrees( m_lng ); };
    virtual double  getLongitudeRadians() const { return m_lng; };
    
    virtual double  getLatitude() const { return MathOps::toDegrees( m_lat ); };
    virtual double  getLatitudeRadians() const { return m_lat; };
    
    virtual double  getAltitude() const { return m_alt; };
    
    virtual BodyId  getBody() const { return m_body; };
    
    virtual Vector  getGeoVector() const;
    
    virtual std::string getString() const;
    
protected:
    double  m_lng, m_lat, m_alt;
    BodyId  m_body;
};
