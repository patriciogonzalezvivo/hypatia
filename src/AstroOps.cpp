/*****************************************************************************\
 * AstroOps.cpp
 *
 * AstroOps is a 'catch-all' class that performs misc. handy calculations
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#include "AstroOps.h"
#include "MathOps.h"
#include "TimeOps.h"

#include "Vector.h"

#include "Vsop.h"

#include <math.h>

#define USE_SOLAR_LON 0

const double AstroOps::SPEED_OF_LIGHT = 299792.458;

const double AstroOps::KM_TO_AU = 0.000000006684587;
const double AstroOps::AU_TO_KM = 1.495978707e+8;
const double AstroOps::AU_TO_M = (AstroOps::AU_TO_KM * 1000.);
const double AstroOps::AU_PER_DAY = (86400. * AstroOps::SPEED_OF_LIGHT / AstroOps::AU_TO_KM);

const double AstroOps::EARTH_POLAR_RADIUS_KM = 6356.76;
const double AstroOps::EARTH_EQUATORIAL_RADIUS_KM = 6378.14;

const double AstroOps::SUN_DIAMETER_KM = 1392000;

//----------------------------------------------------------------------------
/**
 * meanSolarLongitude() - calculate the mean solar latitude on the given T
 *                  (Meeus, Ch. 25)
 *
 * @param t - day to use in Julian Millenia
 *
 * @return - the longitude in radians
 */
double AstroOps::meanSolarLongitude( double t ) {
    static const int N_L_TERMS = 6;
    static const double lTerms[N_L_TERMS] = {
        280.4664567, 360007.6982779, 0.03032028,
        1./49931., -1./15300., -1./2000000. };
    
    double tPower = t;
    double Lm = lTerms[0];
    for (int i =1; i<N_L_TERMS; i++) {
        Lm += lTerms[i] * tPower;
        tPower *= t;
    }
    return MathOps::normalizeDeg2Rad( Lm );
}

//----------------------------------------------------------------------------
/**
 * solarLongitude() - calculate the solar longitude on the given JD
 *                   (Meeus, Ch. 25)
 *
 * @param jd - day to use
 *
 * @return - the longitude in radians
 */
double AstroOps::solarLongitude( double jd ) {
    
    double T = TimeOps::toJulianCenturies(jd);
    double lon = Vsop::calcLoc( T, SUN, Vsop::ECLIPTIC_LON );
    double rad = Vsop::calcLoc( T, SUN, Vsop::RADIUS );
    
    double dPhiSec;
    nutation( jd, &dPhiSec, 0 );
    
    // aberration
    double aber = (20.4898/MathOps::SECONDS_PER_DEGREE) / rad;
    
    return lon -
    MathOps::secToRadians(0.09033) -   // FK5 correction
    MathOps::secToRadians( dPhiSec ) - // nutation
    MathOps::toRadians( aber );        // aberration
}

//----------------------------------------------------------------------------
/**
 * meanSolarAnomaly() - calculate the mean solar anomaly on the given T
 *                     (Meeus, Ch. 25)
 *
 * @param _jc - day to use in Julian Centuries
 *
 * @return - the anomaly in radians
 */
double AstroOps::meanSolarAnomaly( double _jc ) {
    return MathOps::toRadians(357.52911 + 35999.05029*_jc - 0.0001537*_jc*_jc);
}

//----------------------------------------------------------------------------
/**
 * earthEccentricity() - calculate the eccentricity of the Earth's orbit on
 *                       the given T (Meeus, Ch. 25)
 *
 * @param _jc - day to use in Julian Centuries
 *
 * @return - the eccentricity
 */
double AstroOps::earthEccentricity( double _jc ) {
    return 0.016708634 - 0.000042037*_jc - 0.0000001267*_jc*_jc;
}


//----------------------------------------------------------------------------
/**
 * equationOfTime() - calculate the equation of time on the given JD
 *                   (Meeus, Ch. 28)
 *
 * @param jd - day to use
 *
 * @return - the time in minutes between apparent time and mean time
 */
double AstroOps::equationOfTime( double jd ) {
    
    double T = TimeOps::toJulianCenturies(jd);
    
    // calculate mean solar longitude
    double Lm_r = meanSolarLongitude(T/10.); // Julian Millenia
    
    // mean anomaly of the sun
    double M_r = meanSolarAnomaly(T);
    
    // eccentricity of earth's orbit
    double e = earthEccentricity(T);
    
    double dPsiSec, dEpsSec;
    nutation( jd, 0 /*&dPsiSec*/, &dEpsSec );
    double Eps = meanObliquity(jd) + MathOps::secToRadians(dEpsSec);
    
    double y = tan(Eps/2.);
    y *= y;    // tan^2
    double E = y * sin(2*Lm_r) -
    2*e*sin(M_r) +
    4*e*y*sin(M_r)*cos(2*Lm_r) -
    y*y*sin(4*Lm_r)/2 -
    5*e*e*sin(2*M_r)/4;
    
    /*
     PlanetData pd (SUN, jd);
     
     printf( "a=%f\n", MathOps::toDegrees(pd.equatorialLoc(0)) );
     
     double E = Lm -
     MathOps::toRadians(0.0057183) -
     pd.equatorialLoc(0) +  // alpha
     MathOps::secToRadians(dPhiSec) * cos(Em);
     */
    return MathOps::toDegrees(E) * 4; // convert rads to minutes
}

//----------------------------------------------------------------------------
/**
 * equinoxSolstice() - calculate the time of the Equinoxes and Solstices
 *                     Meeus, Ch. 27
 *
 * @param year - year to calculate quarters (must be -1000...+3000 A.D.)
 * @param type - quarter type
 * @param local - true for local time, else UTC
 *
 * @return - the relatively exact time of the spec'd event
 */
double AstroOps::equinoxSolstice( int year, SolExType type, bool local ) {
    
    static const int N_Y_TERMS = 5;
    static const double yTermsA[4][N_Y_TERMS] = { // -1000 to 1000 A.D.
        { 1721139.29189, 365242.13740, +0.06134, +0.00111, -0.00071 }, // SPRING
        { 1721233.25401, 365241.72562, -0.05323, +0.00907, +0.00025 }, // SUMMER
        { 1721325.70455, 365242.49558, -0.11677, -0.00297, +0.00074 }, // AUTUMN
        { 1721414.39987, 365242.99257, -0.00769, -0.00933, -0.00006 }  // WINTER
    };
    
    static const double yTermsB[4][N_Y_TERMS] = { // 1000 to 3000 A.D.
        { 2451623.80984, 365242.37404, +0.05169, -0.00411, -0.00057 }, // SPRING
        { 2451716.56767, 365241.62603, +0.00325, +0.00888, -0.00030 }, // SUMMER
        { 2451810.21715, 365242.01767, -0.11575, +0.00337, +0.00078 }, // AUTUMN
        { 2451900.05952, 365242.74049, -0.06223, -0.00823, +0.00032 }  // WINTER
    };
    
    // calculate mean JD of solar quarter
    //
    const double* py;
    double Y;
    if (year < 1000) {
        Y = double(year)/1000.;
        py = yTermsA[type];
    }
    else {
        Y = double(year-2000)/1000.;
        py = yTermsB[type];
    }
    double jd = py[0];
    double yPower = Y;
    for (int i=1; i<N_Y_TERMS; i++ ) {
        jd += (py[i] * yPower);
        yPower *= Y;    // Y, Y^2, Y^3, etc.
    }
    
#if USE_SOLAR_LON
    
    jd += 58 * sin ( int(type)*Astro::PI_OVER_TWO - solarLongitude(jd) );
    
#else
    
    static const int N_S_TERMS = 24;
    static const double sTerms[N_S_TERMS][3] = {
        { 485., 324.96,   1934.136 },
        { 203., 337.23,  32964.467 },
        { 199., 342.08,     20.186 },
        { 182.,  27.85, 445267.112 },
        { 156.,  73.14,  45036.886 },
        { 136., 171.52,  22518.443 },
        {  77., 222.54,  65928.934 },
        {  74., 296.72,   3034.906 },
        {  70., 243.58,   9037.513 },
        {  58., 119.81,  33718.147 },
        {  52., 297.17,    150.678 },
        {  50.,  21.02,   2281.226 },
        {  45., 247.54,  29929.562 },
        {  44., 325.15,  31555.956 },
        {  29.,  60.73,   4443.417 },
        {  18., 155.12,  67555.328 },
        {  17., 288.79,   4562.452 },
        {  16., 198.04,  62894.029 },
        {  14., 199.76,  31436.921 },
        {  12.,  95.39,  14577.848 },
        {  12., 287.11,  31931.756 },
        {  12., 320.81,  34777.259 },
        {   9., 227.73,   1222.114 },
        {   8.,  15.45,  16859.074 }
    };
    
    // refine mean value
    //
    enum { A, B, C };   // to make the terms below like Meeus'
    double T = TimeOps::toJulianCenturies(jd);
    double S = 0.;
    
    for ( int i = 0; i < N_S_TERMS; i++ ) {
        S += sTerms[i][A] *
        cos( MathOps::toRadians( sTerms[i][B] + (sTerms[i][C] * T) ) );
    }
    
    double W = 35999.373 * T - 2.47;
    double dLam = 1. + 0.0334 * cos( MathOps::toRadians(W) )
    + 0.0007 * cos( MathOps::toRadians(W + W) );
    
    jd += (.00001 * S)/dLam;
    
#endif  /* #if USE_SOLAR_LON */
    
    if (local)
        jd += TimeOps::tzOffsetInDays(jd);
    
    return jd + TimeOps::JD_DIFF;
}

/*----------------------------------------------------------------------------
 * The obliquity formula (and all the magic numbers below) come from Meeus,
 * Astro Algorithms p 135.
 *
 * Input _jcentury is time in julian centuries from 2000.
 * Valid range is the years -8000 to +12000 (t = -100 to 100).
 *
 * return value is mean obliquity (epsilon sub 0) in radians.
 *
 * https://github.com/Bill-Gray/lunar/blob/master/obliquit.cpp
 */
double AstroOps::meanObliquity( double _jcentury ){
    double u, u0;
    unsigned i;
    const double obliquit_minus_100_cen = 24.232841111 * MathOps::DEGS_TO_RADS;
    const double obliquit_plus_100_cen =  22.611485556 * MathOps::DEGS_TO_RADS;
    static double j2000_obliquit = 23. * 3600. + 26. * 60. + 21.448;
    static double t0 = 30000., rval;
    static long coeffs[10] = { -468093L, -155L, 199925L, -5138L,
        -24967L, -3905L, 712L, 2787L, 579L, 245L };
    
    if( _jcentury == 0.)      /* common J2000 case;  don't do any math */
        return( j2000_obliquit * MathOps::ARCS_TO_RADS );
#ifndef CLIP_OBLIQUITY
    else if( _jcentury > 100.)      /* Diverges outside +/- 10 millennia,  */
        return( obliquit_plus_100_cen );
    else if( _jcentury < -100.)  /* so we might as well clip to that  */
        return( obliquit_minus_100_cen );
#endif
    
    if( t0 == _jcentury )    /* return previous answer */
        return( rval );
    
    rval = j2000_obliquit;
    t0 = _jcentury;
    u = u0 = _jcentury / 100.;     /* u is in julian 10000's of years */
    for( i = 0; i < 10; i++, u *= u0) {
        rval += u * (double)coeffs[i] / 100.;
    }
    rval *= MathOps::ARCS_TO_RADS;
    return( rval );
}

//----------------------------------------------------------------------------
/**
 * nutation() - calculate delta phi and/or delta epsilon for the given jd
 *
 * @param - julian day
 * @param pDPhi - [out] nutation (delta phi) in arcseconds
 * @param pDEpsilon - [out] nutation (delta epsilon) in arcseconds
 *
 * The nutation formula (and all the magic numbers below) come from
 * p 132-5, Meeus,  Astro Algorithms
 *
 * Either pointer can be NULL,  in which case that value is
 * not computed.  (we added this because sometimes,  you want
 * only pDPhi or pDEpsilon;  in such cases,  computing _both_
 * is a waste of perfectly good CPU time)
 */
void AstroOps::nutation( double jd, double* pDPhi, double* pDEpsilon ) {
    static const double linearPart[5] = {
        445267.111480, 35999.050340, 477198.867398,
        483202.017538, -1934.136261 };
    
    static const double coefficients[5][3] = {
        { 29785036., -19142.,  189474. },
        { 35752772., - 1603., -300000. },
        { 13496298.,  86972., 56250. },
        {  9327191., -36825., 327270. },
        { 12504452.,  20708., 450000. }
    };
    
    static const int N_NUTATION_COEFFS = 62;
    
    static const int args[N_NUTATION_COEFFS][3] = {
        {  324,-13187,5736 },
        { 1574, -2274, 977 },
        { 1564,  2062,-895 },
        { 1687,  1426,  54 },
        { 1587,   712,  -7 },
        {  449,  -517, 224 },
        { 1573,  -386, 200 },
        { 1599,  -301, 129 },
        {  199,   217, -95 },
        {  337,  -158,   0 },
        {  323,   129, -70 },
        { 1549,   123, -53 },
        { 2812,    63,   0 },
        { 1588,    63, -33 },
        { 2799,   -59,  26 },
        { 1538,   -58,  32 },
        { 1598,   -51,  27 },
        {  362,    48,   0 },
        { 1523,    46, -24 },
        { 2824,   -38,  16 },
        { 1624,   -31,  13 },
        { 1612,    29,   0 },
        {  349,    29, -12 },
        { 1572,    26,   0 },
        {  322,   -22,   0 },
        { 1548,    21, -10 },
        { 1812,    17,   0 },
        { 2788,    16,  -8 },
        {  574,   -16,   7 },
        { 1688,   -15,   9 },
        {  338,   -13,   7 },
        { 1438,   -12,   6 },
        { 1602,    11,   0 },
        { 2798,   -10,   5 },
        { 2849,    -8,   3 },
        { 1699,     7,  -3 },
        {  462,    -7,   0 },
        { 1449,    -7,   3 },
        { 2823,    -7,   3 },
        { 2837,     6,   0 },
        {  374,     6,  -3 },
        {  348,     6,  -3 },
        { 2763,    -6,   3 },
        { 2813,    -6,   3 },
        { 1462,     5,   0 },
        {  198,    -5,   3 },
        {  313,    -5,   3 },
        { 1623,    -5,   3 },
        { 1524,    -3,   0 },
        {  363,     4,   0 },
        {  448,     4,   0 },
        { 1474,    -3,   0 },
        { 2674,    -3,   0 },
        { 1649,    -3,   0 },
        { 2699,    -3,   0 },
        {  837,    -3,   0 },
        {  962,    -4,   0 },
        {  437,    -4,   0 },
        { 1577,     4,   0 },
        { 2187,    -4,   0 },
        { 1712,    -3,   0 },
        { 1597,     3,   0 }
    };
    
    static const int timeDependent[16 + 9] = {
        -16, -2, 2, -34, 1, 12, -4, 0, -5, 0, 1, 0, 0, 1, 0, -1,
        -31, -5, 5, -1,  0, -6, 0, -1, 3 };
    
    double terms[5];
    
    double t  = TimeOps::toJulianCenturies(jd);
    double t2 = t * t;
    double t3 = t2 * t;
    
    for( int i=0; i<5; i++ ) {
        terms[i] = linearPart[i] * t + coefficients[i][0] / 100000.;
        terms[i] += t2 * coefficients[i][1] * 1.e-7;
        terms[i] += t3 / coefficients[i][2];
        terms[i] *= MathOps::RADS_PER_DEGREE;
    }
    
    // The largest term in pDPhi won't fit into a short int.
    // Everything else does,  luckily.
    //
    if( pDPhi )
        *pDPhi = (-171996. - 174.2 * t) * sin( terms[4] );
    
    if( pDEpsilon )
        *pDEpsilon = (92025. + 8.9 * t) * cos( terms[4] );
    
    for( int i=0; i<N_NUTATION_COEFFS; i++ ) {
        double totalArg = 0.;
        int mult = args[i][0];
        
        for( int j=4; j>=0; j--) {
            if( mult % 5 != 2)
                totalArg += (double)( mult % 5 - 2) * terms[j];
            mult /= 5;
        }
        
        double coeff = (double)(args[i][1]);
        if( i < 16 && 0 != timeDependent[i] )
            coeff += (double)(timeDependent[i]) * t / 10.;
        else if( 26 == i || 28 == i )
            coeff += (double)(27 - i) * t / 10.;
        
        if( pDPhi )
            *pDPhi += coeff * sin(totalArg);
        
        if( 0 != args[i][2] ) {
            coeff = (double)(args[i][2]);
            
            if( i < 9 && 0 != timeDependent[i + 16] )
                coeff += (double)(timeDependent[i + 16]) * t / 10.;
            
            if( pDEpsilon)
                *pDEpsilon += coeff * cos( totalArg );
        }
    }
    if( pDPhi )
        *pDPhi *= .0001;
    
    if( pDEpsilon )
        *pDEpsilon *= .0001;
}


//----------------------------------------------------------------------------

// https://github.com/slowe/VirtualSky/blob/gh-pages/0.7.0/virtualsky.js#L1736
void AstroOps::eclipticToEquatorial ( Observer &_obs, double _lng, double _lat, double &_ra, double &_dec ) {
    double obliq = _obs.getObliquity();
    double sin_obliq = sin(obliq);
    double cos_obliq = cos(obliq);
    
    double sl = sin(_lng);
    double cl = cos(_lng);
    double sb = sin(_lat);
    double cb = cos(_lat);
    double tb = tan(_lat);

    _ra = atan2((sl * cos_obliq - tb * sin_obliq),(cl));
    _dec = asin(sb * cos_obliq + cb * sin_obliq * sl);
    
    // Make sure RA is positive
    if (_ra < 0)
        _ra += MathOps::TAU;
}

EqPoint AstroOps::eclipticToEquatorial ( Observer &_obs, const EcPoint &_ecliptic ) {
    double lng = _ecliptic.getLongitudeRadians();
    double lat = _ecliptic.getLatitudeRadians();
    double ra, dec;
    eclipticToEquatorial(_obs, lng, lat, ra, dec);
    EqPoint rta = EqPoint(ra, dec, true);
    rta.compute(_obs);
    return rta;
}

void AstroOps::equatorialToHorizontal ( Observer &_obs, double _ra, double _dec, double &_alt, double &_az ) {
    // compute hour angle in degrees
    double ha = (MathOps::PI*_obs.getLST()/12.) - _ra;
    double sd = sin(_dec);
    double sl = sin(_obs.getLocation().getLatitudeRadians());
    double cl = cos(_obs.getLocation().getLatitudeRadians());
    
    // compute altitude in radians
    _alt = asin(sd*sl + cos(_dec)*cl*cos(ha));
    
    // compute azimuth in radians
    // divide by zero error at poles or if alt = 90 deg (so we should've already limited to 89.9999)
    _az = acos((sd - sin(_alt)*sl)/(cos(_alt)*cl));
    
    // choose hemisphere
    if (sin(ha) > 0.0)
        _az = 2.*MathOps::PI - _az;
}

EcPoint AstroOps::heliocentricToGeocentric( Observer &_obs, const EcPoint &_heliocentric ) {
    Vector heliocentric = _heliocentric.getEclipticVector();
    return EcPoint(heliocentric - _obs.getHeliocentricVector());
}

/* compute parallactic angle given latitude, object dec and alt.
 * all angles in rads.
 * N.B. always return >= 0, caller must determine sign and degenerate cases at
 *   pole or zenith.
 */
// https://github.com/brandon-rhodes/pyephem/blob/592ecff661adb9c5cbed7437a23d705555d7ce57/libastro-3.7.7/parallactic.c#L15
double AstroOps::parallaticAngle(   Observer &_obs,
                                    double _alt, double _dec ) {
    double ca = sin(_obs.getLocation().getLatitudeRadians());
    double cb = sin(_dec);
    double sb = cos(_dec);
    double cc = sin(_alt);
    double sc = cos(_alt);
    double cpa;

    /* given three sides find an angle */
    if ( sb == 0.0 || sc == 0.0 ) {
        return (0.0);
    }
    cpa = (ca - cb * cc) / (sb * sc);
    if (cpa < -1.0) {
        cpa = -1.0;
    }
    if (cpa >  1) {
        cpa =  1.0;
    }
    return acos (cpa);
}
