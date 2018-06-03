#pragma once

#include "../primitives/Vector.h"

class Geodetic : public Polar {
public:
    Geodetic();
    Geodetic(double _lng, double _lat, double _alt_m, ANGLE_TYPE _a_type, UNIT_TYPE _r_type);
    virtual ~Geodetic();
    
    virtual double  getLongitude(ANGLE_TYPE _type) const;
    virtual double  getLatitude(ANGLE_TYPE _type) const;
    virtual double  getAltitude(UNIT_TYPE _type) const;
    virtual double  getRadius(UNIT_TYPE _type) const;
    
    virtual Vector  getVector(UNIT_TYPE _type) const;
    
protected:
    double m_alt;   // km
};

inline std::ostream& operator<<(std::ostream& strm, const Geodetic& p) {
    strm << std::setprecision(3);
    strm << "lng: " << std::setw(12) << MathOps::formatAngle(p.getLongitude(DEGS), DEGS, D_M_Ss);
    strm << ", lat: " << std::setw(12) << MathOps::formatAngle(p.getLatitude(DEGS), DEGS, D_M_Ss);
    strm << ", alt: " << std::setw(8) << p.getAltitude(KM);

    return strm;
};
