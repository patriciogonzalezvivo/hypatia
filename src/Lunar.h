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
    // default c'tor
    Lunar() : m_lon(-1.), m_lat(-1.), m_r(-1.), m_initialized( false ) {}
    Lunar( double _jcentury ) { calcFundamentals( _jcentury ); }
    Lunar( Observer &_obs ) { calcFundamentals( _obs.getJulianCentury()); }

    static const double SYNODIC_MONTH;

    double getAge();
    static double getAge( Observer &_obs );
    static double getAge( double _jcentury );

    // Illuminated Fraction of the Moon's disk.
    double getPhase();

    // Phase Angle of the Moon's.
    double getPhaseAngle();
    double getPhaseAngleRadians();

    // Position Angle of bright limb
    double getPositionAngle( Observer &_obs );
    double getPositionAngleRadians( Observer &_obs );
    
    double getLatitude();         // returns -1 if not initialized
    double getLatitudeRadians() { return ( m_initialized ) ? MathOps::toRadians( getLatitude() ) : -1.; }

    double getLongitude() {  if ( m_lon < 0. ) calcLonRad(); return m_lon; }
    double getLongitudeRadians() { return ( m_initialized ) ? MathOps::toRadians( getLongitude() ) : -1.; }

    double getRadius() { if ( m_r < 0. ) calcLonRad(); return m_r; }

    // calculate all three location elements of the spec'd body at the given time
    //
    void calcAllLocs(
        double& _lon,           // returned longitude
        double& _lat,           // returned latitude
        double& _rad,           // returned radius vector
        double _jcentury) {     // time in decimal centuries
        calcFundamentals( _jcentury );
        _lon = getLongitudeRadians();
        _lat = getLatitudeRadians();
        _rad = getRadius();
    }

private:
    // calculates the fundamanentals given the time
    void calcFundamentals( double _jcentury ); 
    void calcLonRad();

    // calculate an individual fundimental
    //  tptr - points to array of doubles
    double getFund( const double* _tptr, double _jcentury );

    LunarFundamentals m_f;      // our calculated fundmentals
    double m_lon;
    double m_lat;
    double m_r;

    double m_jcentury;

    bool m_initialized;         // true if calcFundamentals has been called    
};

