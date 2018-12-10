// Equatorial Position (Geocentric)
//

#pragma once

#include "../primitives/Vector.h"

class Equatorial : public Polar {
public:
    Equatorial();
    Equatorial(const Vector& _parent);
    Equatorial(const double _ra, const double _dec, ANGLE_UNIT _type);
    virtual ~Equatorial();

    virtual double  getRightAscension(ANGLE_UNIT _type) const;
    virtual double  getDeclination(ANGLE_UNIT _type) const;
    
    virtual Vector  getVector() const;

    virtual double  getAngularDistance(const Equatorial& _equ, ANGLE_UNIT _type) const;
};

inline std::ostream& operator<<(std::ostream& strm, const Equatorial& p) {
    strm << std::setprecision(3);
    strm << "ra: " << std::setw(12) << MathOps::formatAngle(p.getRightAscension(DEGS), DEGS, H_M_Ss);
    strm << ", dec: " << std::setw(12) << MathOps::formatAngle(p.getDeclination(DEGS), DEGS, D_M_Ss);
    return strm;
}
