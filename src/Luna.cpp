/*****************************************************************************\
 * Lunar.cpp
 *
 * Luna is a class that can calculate lunar fundmentals for any reasonable
 *   time.
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#include "Luna.h"

#include "TimeOps.h"
#include "AstroOps.h"

#include "Body.h"
#include "models/VSOP87.h"

#include <stdlib.h>
#include <math.h>

// a list of terms needed to do lunar calcs data extracted from vsop.bin file
//----------------------------------------------------------------------------

const double Luna::SYNODIC_MONTH = 29.530588861;
const double Luna::SYNODIC_WEEK = 29.530588861 * .25;
const double Luna::PERIGEE_KM = 356355.0;
const double Luna::APOGEE_KM = 406725.0;
const double Luna::DIAMETER_KM = 3476.0;

// terms for longitude & radius
//
static const int N_LTERM1 = 60;
struct LunarTerms1 {
    short d, m, mp, f;
    long sl, sr;
};

// terms for latitude
//
static const int N_LTERM2 = 60;
struct LunarTerms2 {
    short d, m, mp, f;
    long sb;
};

// Luna Fundimentals

const double LunarFundimentals_Lp[5] = {
    218.3164591,
    481267.88134236,
    -0.0013268,
    1.85583502e-006,
    -1.53494298e-008
};
const double LunarFundimentals_D[5] = {
    297.8502042,
    445267.1115168,
    -0.00163,
    1.83194472e-006,
    -8.84447e-009
};
const double LunarFundimentals_M[5] = {
    357.5291092,
    35999.0502909,
    -0.0001536,
    4.08329931e-008,
    0.0
};
const double LunarFundimentals_Mp[5] = {
    134.9634114,
    477198.8676313,
    0.008997,
    1.43474081e-005,
    -6.79717238e-008
};
const double LunarFundimentals_F[5] = {
    93.2720993,
    483202.0175273,
    -0.0034029,
    -2.83607487e-007,
    1.15833246e-009
};


// Luna longitude & radius terms

const LunarTerms1 LunarLonRad[60] = {
    { 0,  0,  1,  0,  6288774, -20905335 },
    { 2,  0, -1,  0,  1274027,  -3699111 },
    { 2,  0,  0,  0,   658314,  -2955968 },
    { 0,  0,  2,  0,   213618,   -569925 },
    { 0,  1,  0,  0,  -185116,     48888 },
    { 0,  0,  0,  2,  -114332,     -3149 },
    { 2,  0, -2,  0,    58793,    246158 },
    { 2, -1, -1,  0,    57066,   -152138 },
    { 2,  0,  1,  0,    53322,   -170733 },
    { 2, -1,  0,  0,    45758,   -204586 },
    { 0,  1, -1,  0,   -40923,   -129620 },
    { 1,  0,  0,  0,   -34720,    108743 },
    { 0,  1,  1,  0,   -30383,    104755 },
    { 2,  0,  0, -2,    15327,     10321 },
    { 0,  0,  1,  2,   -12528,         0 },
    { 0,  0,  1, -2,    10980,     79661 },
    { 4,  0, -1,  0,    10675,    -34782 },
    { 0,  0,  3,  0,    10034,    -23210 },
    { 4,  0, -2,  0,     8548,    -21636 },
    { 2,  1, -1,  0,    -7888,     24208 },
    { 2,  1,  0,  0,    -6766,     30824 },
    { 1,  0, -1,  0,    -5163,     -8379 },
    { 1,  1,  0,  0,     4987,    -16675 },
    { 2, -1,  1,  0,     4036,    -12831 },
    { 2,  0,  2,  0,     3994,    -10445 },
    { 4,  0,  0,  0,     3861,    -11650 },
    { 2,  0, -3,  0,     3665,     14403 },
    { 0,  1, -2,  0,    -2689,     -7003 },
    { 2,  0, -1,  2,    -2602,         0 },
    { 2, -1, -2,  0,     2390,     10056 },
    { 1,  0,  1,  0,    -2348,      6322 },
    { 2, -2,  0,  0,     2236,     -9884 },
    { 0,  1,  2,  0,    -2120,      5751 },
    { 0,  2,  0,  0,    -2069,         0 },
    { 2, -2, -1,  0,     2048,     -4950 },
    { 2,  0,  1, -2,    -1773,      4130 },
    { 2,  0,  0,  2,    -1595,         0 },
    { 4, -1, -1,  0,     1215,     -3958 },
    { 0,  0,  2,  2,    -1110,         0 },
    { 3,  0, -1,  0,     -892,      3258 },
    { 2,  1,  1,  0,     -810,      2616 },
    { 4, -1, -2,  0,      759,     -1897 },
    { 0,  2, -1,  0,     -713,     -2117 },
    { 2,  2, -1,  0,     -700,      2354 },
    { 2,  1, -2,  0,      691,         0 },
    { 2, -1,  0, -2,      596,         0 },
    { 4,  0,  1,  0,      549,     -1423 },
    { 0,  0,  4,  0,      537,     -1117 },
    { 4, -1,  0,  0,      520,     -1571 },
    { 1,  0, -2,  0,     -487,     -1739 },
    { 2,  1,  0, -2,     -399,         0 },
    { 0,  0,  2, -2,     -381,     -4421 },
    { 1,  1,  1,  0,      351,         0 },
    { 3,  0, -2,  0,     -340,         0 },
    { 4,  0, -3,  0,      330,         0 },
    { 2, -1,  2,  0,      327,         0 },
    { 0,  2,  1,  0,     -323,      1165 },
    { 1,  1, -1,  0,      299,         0 },
    { 2,  0,  3,  0,      294,         0 },
    { 2,  0, -1, -2,        0,      8752 }
};

// Luna latitude terms

const LunarTerms2 LunarLat[60] = {
    { 0,  0,  0,  1, 5128122 },
    { 0,  0,  1,  1,  280602 },
    { 0,  0,  1, -1,  277693 },
    { 2,  0,  0, -1,  173237 },
    { 2,  0, -1,  1,   55413 },
    { 2,  0, -1, -1,   46271 },
    { 2,  0,  0,  1,   32573 },
    { 0,  0,  2,  1,   17198 },
    { 2,  0,  1, -1,    9266 },
    { 0,  0,  2, -1,    8822 },
    { 2, -1,  0, -1,    8216 },
    { 2,  0, -2, -1,    4324 },
    { 2,  0,  1,  1,    4200 },
    { 2,  1,  0, -1,   -3359 },
    { 2, -1, -1,  1,    2463 },
    { 2, -1,  0,  1,    2211 },
    { 2, -1, -1, -1,    2065 },
    { 0,  1, -1, -1,   -1870 },
    { 4,  0, -1, -1,    1828 },
    { 0,  1,  0,  1,   -1794 },
    { 0,  0,  0,  3,   -1749 },
    { 0,  1, -1,  1,   -1565 },
    { 1,  0,  0,  1,   -1491 },
    { 0,  1,  1,  1,   -1475 },
    { 0,  1,  1, -1,   -1410 },
    { 0,  1,  0, -1,   -1344 },
    { 1,  0,  0, -1,   -1335 },
    { 0,  0,  3,  1,    1107 },
    { 4,  0,  0, -1,    1021 },
    { 4,  0, -1,  1,     833 },
    { 0,  0,  1, -3,     777 },
    { 4,  0, -2,  1,     671 },
    { 2,  0,  0, -3,     607 },
    { 2,  0,  2, -1,     596 },
    { 2, -1,  1, -1,     491 },
    { 2,  0, -2,  1,    -451 },
    { 0,  0,  3, -1,     439 },
    { 2,  0,  2,  1,     422 },
    { 2,  0, -3, -1,     421 },
    { 2,  1, -1,  1,    -366 },
    { 2,  1,  0,  1,    -351 },
    { 4,  0,  0,  1,     331 },
    { 2, -1,  1,  1,     315 },
    { 2, -2,  0, -1,     302 },
    { 0,  0,  1,  3,    -283 },
    { 2,  1,  1, -1,    -229 },
    { 1,  1,  0, -1,     223 },
    { 1,  1,  0,  1,     223 },
    { 0,  1, -2, -1,    -220 },
    { 2,  1, -2, -1,    -220 },
    { 1,  0,  1,  1,    -185 },
    { 2, -1, -2, -1,     181 },
    { 0,  1,  2,  1,    -177 },
    { 4,  0, -2, -1,     176 },
    { 4, -1, -1, -1,     166 },
    { 1,  0,  1, -1,    -164 },
    { 4,  0,  1, -1,     132 },
    { 1,  0, -1, -1,    -119 },
    { 4, -1,  0, -1,     115 },
    { 2, -2,  0,  1,     107 }
};

Luna::Luna(): m_age(0.0), m_posAngle(0.0) {
    m_bodyId = LUNA;
}

//-------------------------------------------------------------------------
/**
 * calculate current phase angle in radians (Meeus' easy lower precision method)
 */
double Luna::getPhaseAngle(ANGLE_TYPE _type) {
    double phase = MathOps::normalize(
        180 - MathOps::toDegrees(m_f.D)
          - 6.289 * sin( m_f.Mp )
          + 2.110 * sin( m_f.M )
          - 1.274 * sin( (2 * m_f.D) - m_f.Mp )
          - 0.658 * sin( 2 * m_f.D )
          - 0.214 * sin( 2 * m_f.Mp )
          - 0.110 * sin( m_f.D ),
          DEGS);
    if ( _type == DEGS ) {
        return phase;
    }
    else {
        return MathOps::toRadians( phase );
    }
}

//-------------------------------------------------------------------------
double Luna::getPhase() {
    return (1. + cos( getPhaseAngle(RADS) )) / 2.;
}

//----------------------------------------------------------------------------
// calculate an individual fundimental
//  tptr - points to array of doubles
//  _jcentury - time in decimal Julian centuries
//
double getFund( const double* tptr, double _jcentury ) {
    double d = *tptr++;
    double tpow = _jcentury;
    for (int i=4; i!=0; i--) {
        d += tpow * (*tptr++);
        tpow *= _jcentury;
    }
    return MathOps::toRadians( MathOps::normalize( d, DEGS ) );
}

//----------------------------------------------------------------------------
// calculate the fundamanentals given the vsop.bin data and a time
//   ad has vsop.bin data
//   t = decimal julian centuries
//
void Luna::compute( Observer &_obs ) {
    if (m_jcentury != _obs.getJC()) {
        m_jcentury = _obs.getJC();

        m_f.Lp = getFund( LunarFundimentals_Lp, m_jcentury );
        m_f.D = getFund( LunarFundimentals_D, m_jcentury );
        m_f.M = getFund( LunarFundimentals_M, m_jcentury );
        m_f.Mp = getFund( LunarFundimentals_Mp, m_jcentury );
        m_f.F = getFund( LunarFundimentals_F, m_jcentury );

        m_f.A1 = MathOps::toRadians( MathOps::normalize( 119.75 + 131.849 * m_jcentury, DEGS ));
        m_f.A2 = MathOps::toRadians( MathOps::normalize( 53.09 + 479264.290 * m_jcentury, DEGS ));
        m_f.A3 = MathOps::toRadians( MathOps::normalize( 313.45 + 481266.484 * m_jcentury, DEGS ));
        m_f.T  = MathOps::toRadians( MathOps::normalize( m_jcentury, DEGS ));

        double lng, lat, rad = 0.0;
        {
            // Compute Ecliptic Geocentric Latitud
            const LunarTerms2* tptr = LunarLat;
            double rval = 0.;

            const double e = 1. - .002516 * m_f.T - .0000074 * m_f.T * m_f.T;

            for( int i=N_LTERM2; i!=0; i-- ) {

                if( labs( tptr->sb ) > 0. ) {
                    double arg;

                    switch( tptr->d ) {
                        case  1:   arg = m_f.D;          break;
                        case -1:   arg =-m_f.D;          break;
                        case  2:   arg = m_f.D+m_f.D;    break;
                        case -2:   arg =-m_f.D-m_f.D;    break;
                        case  0:   arg = 0.;             break;
                        default:   arg = (double)(tptr->d) * m_f.D;  break;
                    }

                    switch( tptr->m ) {
                        case  1:   arg += m_f.M;         break;
                        case -1:   arg -= m_f.M;         break;
                        case  2:   arg += m_f.M+m_f.M;   break;
                        case -2:   arg -= m_f.M+m_f.M;   break;
                        case  0:           ;             break;
                        default:   arg += (double)(tptr->m) * m_f.M;  break;
                    }

                    switch( tptr->mp ) {
                        case  1:   arg += m_f.Mp;        break;
                        case -1:   arg -= m_f.Mp;        break;
                        case  2:   arg += m_f.Mp+m_f.Mp; break;
                        case -2:   arg -= m_f.Mp+m_f.Mp; break;
                        case  0:           ;             break;
                        default:   arg += (double)(tptr->mp) * m_f.Mp;  break;
                    }

                    switch( tptr->f ) {
                        case  1:   arg += m_f.F;         break;
                        case -1:   arg -= m_f.F;         break;
                        case  2:   arg += m_f.F+m_f.F;   break;
                        case -2:   arg -= m_f.F+m_f.F;   break;
                        case  0:           ;             break;
                        default:   arg += (double)(tptr->f) * m_f.F;  break;
                    }

                    double term = (double)(tptr->sb) * sin( arg );
                    for( int j = abs(tptr->m); j!=0; j-- )
                        term *= e;

                    rval += term;
                }
                rval +=   -2235. * sin( m_f.Lp ) +
                           382.  * sin( m_f.A3 ) +
                           175.  * sin( m_f.A1 - m_f.F ) +
                           175.  * sin( m_f.A1 + m_f.F ) +
                           127.  * sin( m_f.Lp - m_f.Mp ) -
                           115.  * sin( m_f.Lp + m_f.Mp );

                tptr++;
            }
            lat = MathOps::toRadians(rval * 1.e-6);
        }

        {
            // Compute Ecliptic Geocentric Longitude and radius
            const LunarTerms1* tptr = LunarLonRad;

            double sl = 0., sr = 0.;
            const double e = 1. - .002516 * m_f.T - .0000074 * m_f.T * m_f.T;

            for( int i=N_LTERM1; i!=0; i-- ) {
                if( labs( tptr->sl ) > 0 || labs( tptr->sr ) > 0 ) {
                    double arg;

                    switch( tptr->d ){
                        case  1:   arg = m_f.D;          break;
                        case -1:   arg =-m_f.D;          break;
                        case  2:   arg = m_f.D+m_f.D;    break;
                        case -2:   arg =-m_f.D-m_f.D;    break;
                        case  0:   arg = 0.;             break;
                        default:   arg = (double)(tptr->d) * m_f.D;  break;
                    }

                    switch( tptr->m ){
                        case  1:   arg += m_f.M;         break;
                        case -1:   arg -= m_f.M;         break;
                        case  2:   arg += m_f.M+m_f.M;   break;
                        case -2:   arg -= m_f.M+m_f.M;   break;
                        case  0:           ;             break;
                        default:   arg += (double)(tptr->m) * m_f.M;  break;
                    }

                    switch( tptr->mp ){
                        case  1:   arg += m_f.Mp;        break;
                        case -1:   arg -= m_f.Mp;        break;
                        case  2:   arg += m_f.Mp+m_f.Mp; break;
                        case -2:   arg -= m_f.Mp+m_f.Mp; break;
                        case  0:           ;             break;
                        default:   arg += (double)(tptr->mp) * m_f.Mp;  break;
                    }

                    switch( tptr->f ){
                        case  1:   arg += m_f.F;         break;
                        case -1:   arg -= m_f.F;         break;
                        case  2:   arg += m_f.F+m_f.F;   break;
                        case -2:   arg -= m_f.F+m_f.F;   break;
                        case  0:           ;             break;
                        default:   arg += (double)(tptr->f) * m_f.F;  break;
                    }

                    if ( tptr->sl ){
                        double term = (double)(tptr->sl) * sin(arg);
                        for( int j=abs(tptr->m); j!=0; j-- )
                            term *= e;
                        sl += term;
                    }

                    if ( tptr->sr ){
                        double term = (double)(tptr->sr) * cos(arg);
                        for( int j=abs(tptr->m); j!=0; j-- )
                            term *= e;
                        sr += term;
                    }
                }
                tptr++;
            }

            sl += 3958. * sin( m_f.A1 ) +
                  1962. * sin( m_f.Lp - m_f.F ) +
                  318.  * sin( m_f.A2 );

            lng = (m_f.Lp * 180. / MathOps::PI) + sl * 1.e-6;
            lng = MathOps::toRadians(MathOps::normalize( lng, DEGS ));
            
            rad = 385000.56 + sr * 0.001; // Km
        }
    
        m_distance = rad;
        m_geocentric = Ecliptic(lng, lat, rad, RADS, KM);
        
        m_equatorial = AstroOps::toEquatorial( _obs, m_geocentric );
        m_horizontal = AstroOps::toHorizontal( _obs, m_equatorial );

        // Compute Sun's coords
        double sun_eclipticLon, sun_eclipticLat, sun_radius;
        VSOP87::calcAllLocs( sun_eclipticLon, sun_eclipticLat, sun_radius, _obs.getJC(), EARTH);
        
        // Get HelioCentric values
        Ecliptic toEarth = Ecliptic(sun_eclipticLon, sun_eclipticLat, sun_radius, RADS, AU);
        Vector Sun2Earth = toEarth.getVector(AU);
        Vector Earth2Moon = m_geocentric.getVector(AU);
        Vector Sun2Moon = Sun2Earth + Earth2Moon;
        
        m_heliocentric = AstroOps::toHeliocentric(_obs, m_geocentric);
        
        // Distance toSun from the Earth
        sun_eclipticLon += MathOps::PI;
        sun_eclipticLat *= -1.;
        toEarth.invert();
        Equatorial sunEq = AstroOps::toEquatorial( _obs, toEarth);
        Horizontal sunHor = AstroOps::toHorizontal( _obs, sunEq);
        
        // Compute moon age
        double moonAge = MathOps::normalize( MathOps::TAU - (sun_eclipticLon - m_geocentric.getLongitude(RADS)), RADS );

        // convert radians to Synodic day
        m_age = SYNODIC_MONTH * (moonAge / MathOps::TAU);

        // Position Angle
        double delta_az = m_horizontal.getAzimuth(RADS) - sunHor.getAzimuth(RADS);
        m_posAngle = atan2( cos(sunHor.getAltitud(RADS)) * sin(delta_az),
                            sin(sunHor.getAltitud(RADS)) * cos(m_horizontal.getAzimuth(RADS)) - cos(sunHor.getAltitud(RADS)) * sin(m_horizontal.getAzimuth(RADS)) * cos(delta_az));

        // double hour_angle = MathOps::toRadians(localSiderealTime(obs.getJulianDay(), obs.getLongitud())) - m_ra;
    }
}

double Luna::getPositionAngle(ANGLE_TYPE _type) const {
    if (_type == DEGS) {
        return MathOps::toDegrees( m_posAngle );
    }
    else {
        return m_posAngle;
    }
}
