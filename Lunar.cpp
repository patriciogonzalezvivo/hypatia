/*****************************************************************************\
 * Lunar.cpp
 *
 * Lunar is a class that can calculate lunar fundmentals for any reasonable
 *   time.
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#include "Lunar.h"

#include "TimeOps.h"
#include "AstroOps.h"
#include "Vsop.h"
#include "PlanetData.h"

#include <stdlib.h>
#include <math.h>

// a list of terms needed to do lunar calcs data extracted from vsop.bin file
//----------------------------------------------------------------------------

const double Lunar::SYNODIC_MONTH = 29.530588861;

// Lunar Fundimentals

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


// Lunar longitude & radius terms

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

// Lunar latitude terms

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


//-------------------------------------------------------------------------
/**
 * calculate current phase angle in radians (Meeus' easy lower precision method)
 */
double Lunar::phaseAngle() {
    if ( !m_initialized )
        return -1.;

    return normalize(
        180 - MathOps::toDegrees(m_f.D)
          - 6.289 * sin( m_f.Mp )
          + 2.110 * sin( m_f.M )
          - 1.274 * sin( (2 * m_f.D) - m_f.Mp )
          - 0.658 * sin( 2 * m_f.D )
          - 0.214 * sin( 2 * m_f.Mp )
          - 0.110 * sin( m_f.D )
          );
}

//-------------------------------------------------------------------------
double Lunar::illuminatedFraction() {
    if ( !m_initialized )
        return -1.;

    return (1. + cos( phaseAngle() )) / 2.;
}

//-------------------------------------------------------------------------
/**
  * Calculate age of the moon in days (0.0 to 29.53...)
  * @param jd - Julian day for which lunar age is required
  */
double Lunar::ageOfMoonInDays( double jd ) {
    double jCenturies = TimeOps::toMillenia( jd ); // convert jd to jm ref. J2000

    // first calculate solar ecliptic longitude (in RAD)
    //
    double earthLon = Vsop::calcLoc( jCenturies, EARTH, Vsop::ECLIPTIC_LON );
    /*
    * What we _really_ want is the location of the sun as seen from
    * the earth (geocentric view).  VSOP gives us the opposite
    * (heliocentric) view, i.e., the earth as seen from the sun.
    * To work around this, we add PI to the longitude (rotate 180 degrees)
    */
    double sunLon = earthLon + MathOps::HD_PI;

    // next calculate lunar ecliptic longitude (in RAD)
    //
    Lunar luna(jCenturies);
    double moonLon = luna.longitudeRadians();

    // age of moon in radians = difference
    double moonAge = MathOps::normalizeRadians( MathOps::TAU - (sunLon - moonLon) );

    // convert radians to Synodic day
    double sday = SYNODIC_MONTH * (moonAge / MathOps::TAU);
    return sday;
}

//----------------------------------------------------------------------------
// calculate an individual fundimental
//  tptr - points to array of doubles
//  t - time in decimal Julian centuries
//
double Lunar::getFund( const double* tptr, double t ) {
    double d = *tptr++;
    double tpow = t;
    for (int i=4; i!=0; i--) {
        d += tpow * (*tptr++);
        tpow *= t;
    }
    return normalize( d );
}

//----------------------------------------------------------------------------
// calculate the fundamanentals given the vsop.bin data and a time
//   ad has vsop.bin data
//   t = decimal julian centuries
//
void Lunar::calcFundamentals( double t ) {
    m_f.Lp = getFund( LunarFundimentals_Lp, t );
    m_f.D = getFund( LunarFundimentals_D, t );
    m_f.M = getFund( LunarFundimentals_M, t );
    m_f.Mp = getFund( LunarFundimentals_Mp, t );
    m_f.F = getFund( LunarFundimentals_F, t );

    m_f.A1 = normalize( 119.75 + 131.849 * t );
    m_f.A2 = normalize( 53.09 + 479264.290 * t );
    m_f.A3 = normalize( 313.45 + 481266.484 * t );
    m_f.T  = normalize( t );

    // indicate values need to be recalculated
    m_lat = m_lon = m_r = -1.;

    // set init'd flag to true
    m_initialized = true;
}

//----------------------------------------------------------------------------
// calculate longitude and radius
//
// NOTE: calcFundamentals() must have been called first
//
void Lunar::calcLonRad() {
    if ( !m_initialized ) {
        m_r = m_lon = -1.;
    }
    else {
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

        m_lon = (m_f.Lp * 180. / MathOps::HD_PI) + sl * 1.e-6;

        // reduce signed angle to ( 0 < m_lon < 360 )
        m_lon = MathOps::normalizeDegrees( m_lon );
        m_r = 385000.56 + sr / 1000.;
    }
}

//----------------------------------------------------------------------------
// calculate (or return prev. calculated) latitude
//
// NOTE: calcFundamentals() must have been called first
//
double Lunar::latitude() {
    if ( !m_initialized )
        return -1.;

    if ( m_lat < 0. ) {
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
        m_lat = rval * 1.e-6;
    }
    return m_lat;
}
