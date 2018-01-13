#pragma once

#include "MathOps.h"
#include "Observer.h"

class EqPoint {
public:
    EqPoint();
    EqPoint( double _ra, double _dec );
    virtual ~EqPoint();

    // Ecuatoriall (Geocentric)
    virtual double  getDeclination() const { return MathOps::toDegrees( m_dec ); }
    virtual double  getDeclinationRadians() const { return m_dec; }
    virtual double  getRightAscension() const { return MathOps::toDegrees( m_ra ); }
    virtual double  getRightAscensionRadians() const { return m_ra; }

    // Horizontal (Topocentric)
    virtual double  getAzimuth() const { return MathOps::toDegrees( m_az ); }
    virtual double  getAzimuthRadians() const { return m_az; }
    virtual double  getAltitud() const { return MathOps::toDegrees( m_alt ); }
    virtual double  getAltitudRadians() const { return m_alt; }

    virtual void    compute( Observer &_obs );

protected:
    virtual void    setEqAngles( double _ra, double _dec );
    virtual void    setEqAnglesRadians( double _ra, double _dec);

    double  m_ra,   m_dec; // Ecuatoriall (Geocentric)
    double  m_alt,  m_az; // Horizontal (Topocentric)
};
