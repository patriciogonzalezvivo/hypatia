#pragma once

#include "MathOps.h"
#include "Vector.h"

class GeoPoint {
public:
    GeoPoint();
    GeoPoint( double _lng, double _lat, double _alt = 0.0, bool _radians = false);
    virtual ~GeoPoint();
    
    virtual double  getLongitude() const { return MathOps::toDegrees( m_lng ); };
    virtual double  getLongitudeRadians() const { return m_lng; };
    
    virtual double  getLatitude() const { return MathOps::toDegrees( m_lat ); };
    virtual double  getLatitudeRadians() const { return m_lat; };
    
    virtual double  getAltitude() const { return m_alt; };
    
    virtual Vector  getVector() const;
    
protected:
    double  m_lng, m_lat, m_alt;
};

inline std::ostream& operator<<(std::ostream& strm, const GeoPoint& p) {
    strm << std::setprecision(3);
    strm << "lng: " << std::setw(12) << MathOps::formatDegrees(p.getLongitude(), D_M_Ss);
    strm << ", lat: " << std::setw(12) << MathOps::formatDegrees(p.getLatitude(), D_M_Ss);
    strm << ", alt: " << std::setw(8) << p.getAltitude();
}

