// Horizontal Position s(Topocentric)
//

#pragma once

#include "Vector.h"
#include "MathOps.h"

class HorPoint {
public:
    HorPoint();
    HorPoint( const Vector& _parent );
    HorPoint( double _alt, double _az, bool _radians = false );
    virtual ~HorPoint();
    
    virtual double  getAzimuth() const { return MathOps::toDegrees( m_az ); }
    virtual double  getAzimuthRadians() const { return m_az; }
    
    virtual double  getAltitud() const { return MathOps::toDegrees( m_alt ); }
    virtual double  getAltitudRadians() const { return m_alt; }
    
    virtual Vector  getVector() const;

protected:
    double  m_alt,  m_az;
};

inline std::ostream& operator<<(std::ostream& strm, const HorPoint& p) {
    strm << std::setprecision(3);
    strm << "alt: " << std::setw(8) << MathOps::formatDegrees(p.getAltitud(), Dd);
    strm << ", az: " << std::setw(8) << MathOps::formatDegrees(p.getAzimuth(), Dd);
    return strm;
}
