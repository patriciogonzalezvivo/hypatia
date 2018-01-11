#pragma once

#include "Observer.h"

class Star {
public:
    Star();
    Star( int _id );

    int getId() const { return m_id; };

    // Ecuatorial Centroid (Geocentric)
    double getDeclination() const { return ( m_id == -1. ) ? -1. : MathOps::toDegrees( m_dec ); }
    double getDeclinationRadians() const { return ( m_id == -1. ) ? -1. : m_dec; }
    double getRightAscension() const { return ( m_id == -1. ) ? -1. : MathOps::toDegrees( m_ra ); }
    double getRightAscensionRadians() const { return ( m_id == -1. ) ? -1. : m_ra; }

    // Horizontal (Topocentric)
    double getAzimuth() const { return ( m_id == -1. ) ? -1. : MathOps::toDegrees( m_az ); }
    double getAzimuthRadians() const { return ( m_id == -1. ) ? -1. : m_az; }
    double getAltitud() const { return ( m_id == -1. ) ? -1. : MathOps::toDegrees( m_alt ); }
    double getAltitudRadians() const { return ( m_id == -1. ) ? -1. : m_alt; }

    void update( Observer &_obs );
protected:
    double m_mag;

    double m_ra;
    double m_dec;

    double m_alt;
    double m_az;
    
    int m_id;
};
