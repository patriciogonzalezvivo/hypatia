// Ecliptic Position (Helio/Geocentric)
//

#pragma once

#include "MathOps.h"
#include "primitives/Vector.h"

class EcPoint {
public:
    EcPoint();
    EcPoint(const Vector& _parent);
    EcPoint(double _lng, double _lat, double _radius = 1., bool _radiant = false);
    virtual ~EcPoint();
    
    EcPoint& operator= (const Vector& _vec);
    
    virtual EcPoint& invert();
    
    virtual void    setLongitude( double _lng, bool _radians = false );
    virtual void    setLatitude( double _lat, bool _radians = false );
    virtual void    setRadius( double _radius );
    
    virtual double  getLongitude() const { return MathOps::toDegrees( m_lng ); };
    virtual double  getLongitudeRadians() const { return m_lng; };
    
    virtual double  getLatitude() const { return MathOps::toDegrees( m_lat ); };
    virtual double  getLatitudeRadians() const { return m_lat; };
    
    virtual double  getRadius() const { return m_radius; };
    
    virtual Vector  getVector() const;
    
protected:
    double m_lng, m_lat, m_radius;
};

inline std::ostream& operator<<(std::ostream& strm, const EcPoint& p) {
    strm << std::setprecision(3);
    strm << "lng: " << std::setw(12) << MathOps::formatDegrees(p.getLongitude(), D_M_Ss);
    strm << ", lat: " << std::setw(12) << MathOps::formatDegrees(p.getLatitude(), D_M_Ss);
    strm << ", rad: " << std::setw(8) << p.getRadius();
    return strm;
}

