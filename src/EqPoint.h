// Equatorial Position (Geocentric)
//

#pragma once

#include "Vector.h"
#include "MathOps.h"

class EqPoint {
public:
    EqPoint();
    EqPoint(const Vector& _parent);
    EqPoint( double _ra, double _dec, bool _radians = false );
    virtual ~EqPoint();

    virtual double  getDeclination() const { return MathOps::toDegrees( m_dec ); }
    virtual double  getDeclinationRadians() const { return m_dec; }
    
    virtual double  getRightAscension() const { return MathOps::toDegrees( m_ra ); }
    virtual double  getRightAscensionRadians() const { return m_ra; }
    
    virtual Vector  getVector() const;

protected:
    double  m_ra, m_dec;
};

inline std::ostream& operator<<(std::ostream& strm, const EqPoint& p) {
    strm << std::setprecision(3);
    strm << "ra: " << std::setw(12) << MathOps::formatDegrees(p.getRightAscension(), H_M_Ss);
    strm << ", dec: " << std::setw(12) << MathOps::formatDegrees(p.getDeclination(), D_M_Ss);
}
