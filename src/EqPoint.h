#pragma once

#include "Vector.h"
#include "MathOps.h"
#include "Observer.h"

class EqPoint {
public:
    EqPoint();
    EqPoint( double _ra_deg, double _dec_deg );
    virtual ~EqPoint();
    
    virtual double  setInRadians(double _ra_rad, double _dec_rad);

    // Equatorial (Geocentric)
    virtual double  getDeclination() const { return MathOps::toDegrees( m_dec ); }
    virtual double  getDeclinationRadians() const { return m_dec; }
    virtual double  getRightAscension() const { return MathOps::toDegrees( m_ra ); }
    virtual double  getRightAscensionRadians() const { return m_ra; }
    virtual Vector  getEquatorialVector() const { return Vector(m_ra, m_dec, 1.); }

    // Horizontal (Topocentric)
    virtual double  getAzimuth() const { return MathOps::toDegrees( m_az ); }
    virtual double  getAzimuthRadians() const { return m_az; }
    virtual double  getAltitud() const { return MathOps::toDegrees( m_alt ); }
    virtual double  getAltitudRadians() const { return m_alt; }
    virtual Vector  getHorizontalVector() const { return Vector(m_alt, m_az, 1.); }

    virtual void    compute( Observer &_obs );

protected:
    virtual void    setEqAngles( double _ra, double _dec );
    virtual void    setEqAnglesRadians( double _ra, double _dec);

    double  m_ra,   m_dec; // Ecuatoriall (Geocentric)
    double  m_alt,  m_az; // Horizontal (Topocentric)
};
