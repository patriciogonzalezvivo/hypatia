// Ecliptic Position (Helio/Geocentric)
//

#pragma once

#include "MathOps.h"
#include "Vector.h"

#include <string>

class EcPoint {
public:
    EcPoint();
    EcPoint(const Vector& _parent);
    EcPoint(double _lng, double _lat, double _radius = 1., bool _radiant = false);
    virtual ~EcPoint();
    
    EcPoint& operator= (const Vector& _vec);
    
    virtual void    setLongitude( double _lng, bool _radians = false );
    virtual void    setLatitude( double _lat, bool _radians = false );
    virtual void    setRadius( double _radius );
    
    virtual double  getLongitude() const { return MathOps::toDegrees( m_lng ); };
    virtual double  getLongitudeRadians() const { return m_lng; };
    
    virtual double  getLatitude() const { return MathOps::toDegrees( m_lat ); };
    virtual double  getLatitudeRadians() const { return m_lat; };
    
    virtual double  getRadius() const { return m_radius; };
    
    virtual Vector  getEclipticVector() const;
    
    virtual std::string getString() const;
    
protected:
    double m_lng, m_lat, m_radius;
};
