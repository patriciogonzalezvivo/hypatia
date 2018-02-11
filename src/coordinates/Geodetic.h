#pragma once

#include "../MathOps.h"
#include "../primitives/Vector.h"

class Geodetic : public Polar {
public:
    Geodetic();
    Geodetic( double _lng, double _lat, double _alt = 0.0, bool _radians = false);
    virtual ~Geodetic();
    
    virtual double  getLongitude() const { return MathOps::toDegrees( m_phi ); };
    virtual double  getLongitudeRadians() const { return m_phi; };
    
    virtual double  getLatitude() const { return MathOps::toDegrees( m_theta ); };
    virtual double  getLatitudeRadians() const { return m_theta; };
    
    virtual double  getAltitude() const { return m_alt; };
    
    virtual double  getRadius() const;
    
    virtual Vector  getVector() const;
    
protected:
    double m_alt;
};

inline std::ostream& operator<<(std::ostream& strm, const Geodetic& p) {
    strm << std::setprecision(3);
    strm << "lng: " << std::setw(12) << MathOps::formatDegrees(p.getLongitude(), D_M_Ss);
    strm << ", lat: " << std::setw(12) << MathOps::formatDegrees(p.getLatitude(), D_M_Ss);
    strm << ", alt: " << std::setw(8) << p.getAltitude();

    return strm;
}
