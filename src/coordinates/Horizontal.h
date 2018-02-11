// Horizontal Position s(Topocentric)
//

#pragma once

#include "../MathOps.h"
#include "../primitives/Vector.h"

class Horizontal : public Polar {
public:
    Horizontal();
    Horizontal( const Vector& _parent );
    Horizontal(const double _alt, const double _az, bool _radians = false);
    virtual ~Horizontal();
    
    virtual double  getAltitud() const { return MathOps::toDegrees( m_phi ); }
    virtual double  getAltitudRadians() const { return m_phi; }
    
    virtual double  getAzimuth() const { return MathOps::toDegrees( m_theta ); }
    virtual double  getAzimuthRadians() const { return m_theta; }
    
    virtual Vector  getVector() const;
};

inline std::ostream& operator<<(std::ostream& strm, const Horizontal& p) {
    strm << std::setprecision(3);
    strm << "alt: " << std::setw(8) << MathOps::formatDegrees(p.getAltitud(), Dd);
    strm << ", az: " << std::setw(8) << MathOps::formatDegrees(p.getAzimuth(), Dd);
    return strm;
}
