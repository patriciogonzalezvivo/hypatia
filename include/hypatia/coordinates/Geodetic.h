#pragma once

#include "../primitives/Vector.h"

class Geodetic : public Polar {
public:
    Geodetic();
    Geodetic(double _lng, double _lat, double _alt_m, ANGLE_UNIT _a_type, DISTANCE_UNIT _r_type);
    virtual ~Geodetic();
    
    virtual double  getLongitude(ANGLE_UNIT _type) const;
    virtual double  getLatitude(ANGLE_UNIT _type) const;
    virtual double  getAltitude(DISTANCE_UNIT _type) const;
    virtual double  getRadius(DISTANCE_UNIT _type) const;
    
    virtual Vector  getVector(DISTANCE_UNIT _type) const;
    
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
