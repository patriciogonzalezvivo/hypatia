// Horizontal Position s(Topocentric)
//

#pragma once

#include "Vector.h"
#include "MathOps.h"

#include <string>

class HorPoint {
public:
    HorPoint();
    HorPoint(const Vector& _parent);
    HorPoint( double _alt, double _az, bool _radians = false );
    virtual ~HorPoint();
    
    virtual double  getAzimuth() const { return MathOps::toDegrees( m_az ); }
    virtual double  getAzimuthRadians() const { return m_az; }
    
    virtual double  getAltitud() const { return MathOps::toDegrees( m_alt ); }
    virtual double  getAltitudRadians() const { return m_alt; }
    
    virtual Vector  getHorizontalVector() const;
    
    virtual std::string getString() const;
    
protected:
    virtual void    setHorizontalAngles( double _alt, double _az, bool _radians = false );
    
    double  m_alt,  m_az;
};
