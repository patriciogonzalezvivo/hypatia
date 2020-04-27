// Horizontal Position s(Topocentric)
//

#pragma once

#include "../primitives/Vector3.h"

class Horizontal : public Polar {
public:
    Horizontal();
    Horizontal( const Vector3& _parent );
    Horizontal(const double _alt, const double _az, ANGLE_UNIT _type);
    virtual ~Horizontal();
    
    virtual double  getAltitud(ANGLE_UNIT _type) const;
    virtual double  getAzimuth(ANGLE_UNIT _type) const;
    
    virtual Vector3 getVector() const;
};

inline std::ostream& operator<<(std::ostream& strm, const Horizontal& p) {
    strm << std::setprecision(3);
    strm << "alt: " << std::setw(8) << MathOps::formatAngle(p.getAltitud(DEGS), DEGS, Dd);
    strm << ", az: " << std::setw(8) << MathOps::formatAngle(p.getAzimuth(DEGS), DEGS, Dd);
    return strm;
};
