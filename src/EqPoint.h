// Equatorial Position (Geocentric)
//

#pragma once

#include "HorPoint.h"
#include "Observer.h"

class EqPoint : public HorPoint {
public:
    EqPoint();
    EqPoint(const Vector& _parent);
    EqPoint( double _ra, double _dec, bool _radians = false );
    virtual ~EqPoint();

    virtual double  getDeclination() const { return MathOps::toDegrees( m_dec ); }
    virtual double  getDeclinationRadians() const { return m_dec; }
    
    virtual double  getRightAscension() const { return MathOps::toDegrees( m_ra ); }
    virtual double  getRightAscensionRadians() const { return m_ra; }
    
    virtual Vector  getEquatorialVector() const;
    
    virtual double  getHourAngle() const { return MathOps::toDegrees( m_hourAngle ); }
    virtual double  getHourAngleRadians() const { return m_hourAngle; }

    virtual void    compute( Observer &_obs );

protected:
    virtual void    setEquatorialAngles( double _ra, double _dec, bool _radians = false );

    double  m_ra, m_dec;
    double  m_hourAngle;
};
