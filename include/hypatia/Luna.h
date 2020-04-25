/*****************************************************************************\
 * Luna.h
 *
 * Luna is a class that can calculate lunar fundmentals for any reasonable
 * time.
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#pragma once

// #include "CoordOps.h"
#include "Body.h"

// A struct to hold the fundmental elements
// The member names should be familiar to Meeus fans ;-)
//
struct LunarFundamentals {
    double Lp, D, M, Mp, F, A1, A2, A3, T;
    LunarFundamentals():Lp(0.),D(0.),M(0.),Mp(0.),F(0.),A1(0.),A2(0.),A3(0.),T(0.) {}
};

// our main class -- calculates Luna fundamentals, lat, lon & distance
//
class Luna : public Body {
public:
    Luna();

    static const double SYNODIC_MONTH;
    static const double SYNODIC_WEEK;
    static const double PERIGEE_KM;
    static const double APOGEE_KM;
    static const double DIAMETER_KM;

    // Calculate age of the moon in days (0.0 to 29.53...)
    virtual double getAge() const { return m_age; };

    // Illuminated Fraction of the Moon's disk.
    virtual double getPhase();

    // Distance to the earht usually better in km
    virtual double getDistance(DISTANCE_UNIT _type) const; 
    
    // Phase Angle of the Moon's.
    virtual double getPhaseAngle(ANGLE_UNIT _type);

    // Position Angle of bright limb
    virtual double getPositionAngle(ANGLE_UNIT _type) const;

    // calculate all three location elements of the spec'd body at the given time
    virtual void compute( Observer &_obs );

private:
    LunarFundamentals m_f;      // our calculated fundmentals
    
    double m_age, m_posAngle, m_distance;
};

