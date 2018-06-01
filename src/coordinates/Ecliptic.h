// Ecliptic Position (Helio/Geocentric)
//

#pragma once

#include "../primitives/Vector.h"

class Ecliptic : public Polar {
public:
    Ecliptic ();
    Ecliptic (const Vector& _parent, UNIT_TYPE _type);
    Ecliptic (double _lng, double _lat, double _radius, ANGLE_TYPE _a_type, UNIT_TYPE _d_type);
    virtual ~Ecliptic();
    
    virtual Ecliptic& operator= (const Vector& _vec);
    
    virtual double  getLongitude (ANGLE_TYPE _type) const;
    virtual double  getLatitude (ANGLE_TYPE _type) const;
    virtual double  getRadius (UNIT_TYPE _type) const;
    
    virtual Vector  getVector (UNIT_TYPE _type) const;
    
protected:
    double m_radius;
};

inline std::ostream& operator<<(std::ostream& strm, const Ecliptic& p) {
    strm << std::setprecision(3);
    strm << "lng: " << std::setw(12) << MathOps::formatAngle(p.getLongitude(DEGS), DEGS, D_M_Ss);
    strm << ", lat: " << std::setw(12) << MathOps::formatAngle(p.getLatitude(DEGS), DEGS, D_M_Ss);
    strm << ", rad: " << std::setw(8) << p.getRadius(AU);
    return strm;
}

