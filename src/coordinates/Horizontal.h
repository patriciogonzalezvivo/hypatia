// Horizontal Position s(Topocentric)
//

#pragma once

#include "../primitives/Vector.h"

class Horizontal : public Polar {
public:
    Horizontal();
    Horizontal( const Vector& _parent );
    Horizontal(const double _alt, const double _az, ANGLE_TYPE _type);
    virtual ~Horizontal();
    
    virtual double  getAltitud(ANGLE_TYPE _type) const;
    virtual double  getAzimuth(ANGLE_TYPE _type) const;
    
    virtual Vector  getVector() const;
};

inline std::ostream& operator<<(std::ostream& strm, const Horizontal& p) {
    strm << std::setprecision(3);
    strm << "alt: " << std::setw(8) << MathOps::formatAngle(p.getAltitud(DEGS), DEGS, Dd);
    strm << ", az: " << std::setw(8) << MathOps::formatAngle(p.getAzimuth(DEGS), DEGS, Dd);
    return strm;
};
