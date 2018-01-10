/*****************************************************************************\
 * Lunar.h
 *
 * Lunar is a class that can calculate lunar fundmentals for any reasonable
 * time.
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#pragma once

#include "AstroOps.h"

// A struct to hold the fundmental elements
// The member names should be familiar to Meeus fans ;-)
//
struct LunarFundamentals {
    double Lp, D, M, Mp, F, A1, A2, A3, T;
    LunarFundamentals():Lp(0.),D(0.),M(0.),Mp(0.),F(0.),A1(0.),A2(0.),A3(0.),T(0.) {}
};

// our main class -- calculates Lunar fundamentals, lat, lon & distance
//
class Lunar {
public:
    Lunar();
    Lunar( Observer &_obs );

    static const double SYNODIC_MONTH;

    // Calculate age of the moon in days (0.0 to 29.53...)
    double getAge() const { return m_age; };

    // Illuminated Fraction of the Moon's disk.
    double getPhase();

    // Phase Angle of the Moon's.
    double getPhaseAngle();
    double getPhaseAngleRadians();

    // Position Angle of bright limb
    double getPositionAngle() const { return MathOps::toDegrees( m_pangle ); };
    double getPositionAngleRadians() const { return m_pangle; };
    
    // Ecliptic Geocentric Lng/Lat
    double getEclipticLon() const { return MathOps::toDegrees( m_eclipticLon ); };
    double getEclipticLonRadians() const { return m_eclipticLon; }
    double getEclipticLat() const { return MathOps::toDegrees( m_eclipticLat ); }
    double getEclipticLatRadians() const { return m_eclipticLat; } 

    double getRadius() const { return m_r; }

    // Ecuatorial (Geocentric)
    double getDeclination() const { return MathOps::toDegrees( m_dec ); }
    double getDeclinationRadians() const { return m_dec; }
    double getRightAscension() const {  return MathOps::toDegrees( m_ra ); }
    double getRightAscensionRadians() const {  return m_ra; }

    // Horizontal (Topocentric)
    double getAzimuth() const { return MathOps::toDegrees( m_az ); }
    double getAzimuthRadians() const { return m_az; }
    double getAltitud() const { return MathOps::toDegrees( m_alt ); }
    double getAltitudRadians() const { return m_alt; }

    // calculate all three location elements of the spec'd body at the given time
    void update( Observer &_obs );

private:
    LunarFundamentals m_f;      // our calculated fundmentals

    double m_jcentury;

    double m_r;
    double m_eclipticLon;
    double m_eclipticLat;
    
    double m_az;
    double m_alt;
    double m_dec;
    double m_ra;

    double m_age;
    double m_pangle;

    bool m_initialized;         // true if calcFundamentals has been called    
};

