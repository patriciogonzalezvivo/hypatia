// Equatorial Position (Geocentric)
//

#pragma once

#include "../MathOps.h"
#include "../primitives/Vector.h"

class Equatorial : public Polar {
public:
    Equatorial();
    Equatorial(const Vector& _parent);
    Equatorial(const double _ra, const double _dec, bool _radians = false);
    virtual ~Equatorial();
    
    virtual double  getRightAscension() const { return MathOps::toDegrees( m_phi ); }
    virtual double  getRightAscensionRadians() const { return m_phi; }

    virtual double  getDeclination() const { return MathOps::toDegrees( m_theta ); }
    virtual double  getDeclinationRadians() const { return m_theta; }
    
    virtual Vector  getVector() const;
};

inline std::ostream& operator<<(std::ostream& strm, const Equatorial& p) {
    strm << std::setprecision(3);
    strm << "ra: " << std::setw(12) << MathOps::formatDegrees(p.getRightAscension(), H_M_Ss);
    strm << ", dec: " << std::setw(12) << MathOps::formatDegrees(p.getDeclination(), D_M_Ss);
    return strm;
}
