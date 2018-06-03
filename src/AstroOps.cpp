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

#include "primitives/Vector.h"

#include "models/VSOP87.h"

#include <math.h>

#define USE_SOLAR_LON 0

const double AstroOps::SPEED_OF_LIGHT = 299792.458;

const double AstroOps::KM_TO_AU = 0.000000006684587;
const double AstroOps::AU_TO_KM = 1.495978707e+8;
const double AstroOps::AU_TO_M = (AstroOps::AU_TO_KM * 1000.);
const double AstroOps::AU_PER_DAY = (86400. * AstroOps::SPEED_OF_LIGHT / AstroOps::AU_TO_KM);

const double AstroOps::EARTH_FLATTENING = 1.0 / 298.26;
const double AstroOps::EARTH_POLAR_RADIUS_KM = 6356.76;
const double AstroOps::EARTH_EQUATORIAL_RADIUS_KM = 6378.135;
const double AstroOps::EARTH_ROTATION_PER_SIDERAL_DAY = 1.00273790934;
const double AstroOps::EARTH_GRAVITATIONAL_CONSTANT = 398600.8;

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
    return MathOps::toRadians(MathOps::normalize( Lm, DEGS ));
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
    
    double T = TimeOps::toJC(jd);
    double lon = VSOP87::calcLoc( T, SUN, VSOP87::ECLIPTIC_LON );
    double rad = VSOP87::calcLoc( T, SUN, VSOP87::RADIUS );
    
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
    
    double T = TimeOps::toJC(jd);
    
    // calculate mean solar longitude
    double Lm_r = meanSolarLongitude(T/10.); // Julian Millenia
    
    // mean anomaly of the sun
    double M_r = meanSolarAnomaly(T);
    
    // eccentricity of earth's orbit
    double e = earthEccentricity(T);
    
    // double dPsiSec, dEpsSec;
    // nutation( jd, 0 /*&dPsiSec*/, &dEpsSec );
    double dEpsSec;
    nutation( jd, 0 , &dEpsSec );
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
    double T = TimeOps::toJC(jd);
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
 */
double AstroOps::meanObliquity( double _jcentury ){
//
//    https://github.com/Bill-Gray/lunar/blob/master/obliquit.cpp
//
    double u, u0;
    double t0 = 30000.;
    double rval = 0.;
    
    const double obliquit_minus_100_cen = MathOps::toRadians(24.232841111);
    const double obliquit_plus_100_cen = MathOps::toRadians(22.611485556);
    
    static const double j2000_obliquit = 23. * MathOps::SECONDS_PER_DEGREE +
                                         26. * MathOps::MINUTES_PER_DEGREE +
                                         21.448;
    static const int OBLIQ_COEFFS = 10;
    static const double coeffs[ OBLIQ_COEFFS ] = {
        -468093.,  -155.,  199925.,  -5138.,  -24967.,
        -3905.,    712.,   2787.,    579.,    245.
    };
    
    if( _jcentury == 0.)      /* common J2000 case;  don't do any math */
        return MathOps::toRadians( j2000_obliquit / MathOps::SECONDS_PER_DEGREE );
#ifndef CLIP_OBLIQUITY
    else if( _jcentury > 100.)      /* Diverges outside +/- 10 millennia,  */
        return( obliquit_plus_100_cen);
    else if( _jcentury < -100.)  /* so we might as well clip to that  */
        return( obliquit_minus_100_cen);
#endif
    else if ( _jcentury != t0) {
        t0 = _jcentury;
        u = u0 = _jcentury / 100.;     // u is in julian 10000's of years
        rval = j2000_obliquit;
        
        for ( int i=0; i<OBLIQ_COEFFS; i++ ) {
            rval += u * coeffs[i] / 100.;
            u *= u0;
        }
        // convert from seconds to radians
        rval = MathOps::toRadians( rval / MathOps::SECONDS_PER_DEGREE );
    }
    return rval;
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
    
    double t  = TimeOps::toJC(jd);
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

/**
 * eclipticToEquatorial() - ecliptic to equatorial coordinates
 *                          (Meeus, Ch. 93)
 *
 *   https://github.com/slowe/VirtualSky/blob/gh-pages/0.7.0/virtualsky.js#L1736
 *
 * @param Observer
 * @param lng - of ecliptic position (IN)
 * @param lat - of ecliptic position (IN)
 * @param ra - of equatorial position (OUT)
 * @param dec - of equatorial position (OUT)
 *
 */
void AstroOps::eclipticToEquatorial ( double _obliq, double _lng, double _lat, double &_ra, double &_dec ) {
    double sin_obliq = sin(_obliq);
    double cos_obliq = cos(_obliq);

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

/**
 * eclipticToEquatorial() - ecliptic to equatorial coordinates
 *                          (Meeus, Ch. 93)
 *
 * @param Observer
 * @param Ecliptic position
 *
 * @return Equatorial position
 */
Equatorial AstroOps::toEquatorial ( const Observer& _obs, const Ecliptic &_ecliptic ) {
    double lng = _ecliptic.getLongitude(RADS);
    double lat = _ecliptic.getLatitude(RADS);
    double ra, dec;
    AstroOps::eclipticToEquatorial(_obs.getObliquity(), lng, lat, ra, dec);
    return Equatorial(ra, dec, RADS);
}

/**
 * eclipticToEquatorial() - equatorial to horizontal coordinates
 *                          (Meeus, Ch. 93)
 *
 *   https://github.com/slowe/VirtualSky/blob/gh-pages/0.7.0/virtualsky.js#L1736
 *
 * @param Observer's latitud
 * @param hour angle (IN)
 * @param dec - of equatorial position (IN)
 * @param alt - of ecliptic position (OUT)
 * @param az - of ecliptic position (OUT)
 *
 */
void AstroOps::equatorialToHorizontal ( double _lat, double _ha, double _dec, double &_alt, double &_az ) {
    double sd = sin(_dec);
    double sl = sin(_lat);
    double cl = cos(_lat);

    // compute altitude in radians
    _alt = asin(sd*sl + cos(_dec)*cl*cos(_ha));

    // compute azimuth in radians
    // divide by zero error at poles or if alt = 90 deg (so we should've already limited to 89.9999)
    _az = acos((sd - sin(_alt)*sl)/(cos(_alt)*cl));

    // choose hemisphere
    if (sin(_ha) > 0.0)
        _az = 2.*MathOps::PI - _az;
}

/**
 * eclipticToEquatorial() - calcuate hour angle
 *                          (Meeus, Ch. 92)
 *
 * @param lst - observer's local sideral time
 * @param ra - right acention position (radians)
 * @param dec - of equatorial position (radians)
 *
 * @return hour angle
 */
double AstroOps::toHourAngle( double _lst, double _ra ) {
    return _lst - _ra;
}

double AstroOps::toHourAngle( const Observer& _obs, double _ra ) {
    return _obs.getLST() - _ra;
}

double AstroOps::toHourAngle( const Observer& _obs, const Equatorial &_equatorial ) {
    return _obs.getLST() - _equatorial.getRightAscension(RADS);
}

/**
 * eclipticToEquatorial() - equatorial to horizontal coordinates
 *                          (Meeus, Ch. 93)
 *
 * @param Observer
 * @param equatorial coordinate
 *
 * @return horizontal position
 */
Horizontal AstroOps::toHorizontal( const Observer& _obs, const Equatorial& _equatorial) {
    double dec = _equatorial.getDeclination(RADS);
    double ha = toHourAngle(_obs, _equatorial);
    double alt, az;
    AstroOps::equatorialToHorizontal(_obs.getLocation().getLatitude(RADS), ha, dec, alt, az);
    return Horizontal(alt, az, RADS);
}

/**
 * toGeocentric() - ecliptic transformation from heliocentric to geocentric
 *
 * @param Observer
 * @param Ecliptic heliocentric
 *
 * @return Ecliptic geocentric
 */
Ecliptic AstroOps::toGeocentric( Observer& _obs, const Ecliptic& _heliocentric ) {
    Vector heliocentric = _heliocentric.getVector(AU);
    return Ecliptic(heliocentric - _obs.getHeliocentricVector(AU), AU);
}

/**
 * toHeliocentric() - ecliptic transformation from geocentric to heliocentric
 *
 * @param Observer
 * @param Ecliptic geocentric
 *
 * @return Ecliptic heliocentric
 */
Ecliptic AstroOps::toHeliocentric(Observer& _obs, const Ecliptic& _geocentric ){
    
    // Compute Sun's coords
    double sun_eclipticLon, sun_eclipticLat, sun_radius;
    VSOP87::calcAllLocs( sun_eclipticLon, sun_eclipticLat, sun_radius, _obs.getJC(), EARTH);
    
    // Get HelioCentric values
    Ecliptic toEarth = Ecliptic(sun_eclipticLon, sun_eclipticLat, sun_radius, RADS, AU);
    Vector Sun2Earth = toEarth.getVector(AU);
    Vector Earth2Moon = _geocentric.getVector(AU);
    Vector Sun2Moon = Sun2Earth + Earth2Moon;
    
    return Ecliptic(Sun2Moon, AU);
}

Geodetic AstroOps::toGeodetic(const ECI& _eci) {
    const double theta = MathOps::actan(_eci.getPosition(KM).y, _eci.getPosition(KM).x);
    
    double lon = theta - TimeOps::toGreenwichSiderealTime(_eci.getJD());
    //WrapNegPosPI
    lon = MathOps::mod(lon + MathOps::PI, MathOps::TAU) - MathOps::PI;
    
    const double r = sqrt((_eci.getPosition(KM).x * _eci.getPosition(KM).x)
                          + (_eci.getPosition(KM).y * _eci.getPosition(KM).y));
    
    static const double e2 = AstroOps::EARTH_FLATTENING * (2.0 - AstroOps::EARTH_FLATTENING);
    
    double lat = MathOps::actan(_eci.getPosition(KM).z, r);
    double phi = 0.0;
    double c = 0.0;
    int cnt = 0;
    
    do {
        phi = lat;
        const double sinphi = sin(phi);
        c = 1.0 / sqrt(1.0 - e2 * sinphi * sinphi);
        lat = MathOps::actan(_eci.getPosition(KM).z + AstroOps::EARTH_EQUATORIAL_RADIUS_KM * c * e2 * sinphi, r);
        cnt++;
    }
    while (fabs(lat - phi) >= 1e-10 && cnt < 10);
    
    const double alt = r / cos(lat) - AstroOps::EARTH_EQUATORIAL_RADIUS_KM * c;
    
    return Geodetic(lat, lon, alt, RADS, KM);
}

ECI AstroOps::toECI(double _jd, const Geodetic& _geod) {
    static const double mfactor = MathOps::TAU * (AstroOps::EARTH_ROTATION_PER_SIDERAL_DAY / TimeOps::SECONDS_PER_DAY);
    
    /*
     * Calculate Local Mean Sidereal Time for observers longitude
     */
    const double theta = TimeOps::toLocalSideralTime(_jd, _geod.getLongitude(RADS), RADS);
    
    /*
     * take into account earth flattening
     */
    const double c = 1.0 / sqrt(1.0 + AstroOps::EARTH_FLATTENING * (AstroOps::EARTH_FLATTENING - 2.0) * pow(sin(_geod.getLatitude(RADS)), 2.0));
    const double s = pow(1.0 - AstroOps::EARTH_FLATTENING, 2.0) * c;
    const double achcp = (AstroOps::EARTH_EQUATORIAL_RADIUS_KM * c + _geod.getLatitude(RADS)) * cos(_geod.getLatitude(RADS));
    
    Vector position;
    position.x = achcp * cos(theta); // km
    position.y = achcp * sin(theta); // km
    position.z = (AstroOps::EARTH_EQUATORIAL_RADIUS_KM * s + _geod.getLatitude(RADS)) * sin(_geod.getLatitude(RADS)); // km
    
    Vector velocity; 
    velocity.x = -mfactor * position.y; // km/s
    velocity.y = mfactor * position.x;  // km/s
    velocity.z = 0.0;                   // km/s
    
    return ECI(_jd, position, velocity);
}

Horizontal AstroOps::toHorizontal(const Observer& _obs, const ECI& _eci) {
    ECI obs = ECI(_eci.getJD(), _obs.getLocation());
    
    /*
     * calculate differences
     */
    Vector range_rate = _eci.getVelocity(KM) - obs.getVelocity(KM);
    Vector range = _eci.getPosition(KM) - obs.getPosition(KM);
    
    /*
     * Calculate Local Mean Sidereal Time for observers longitude
     */
    double theta = TimeOps::toLocalSideralTime(_eci.getJD(), _obs.getLocation().getLongitude(RADS), RADS);
    
    double sin_lat = sin(_obs.getLocation().getLatitude(RADS));
    double cos_lat = cos(_obs.getLocation().getLatitude(RADS));
    double sin_theta = sin(theta);
    double cos_theta = cos(theta);
    
    double top_s = sin_lat * cos_theta * range.x + sin_lat * sin_theta * range.y - cos_lat * range.z;
    double top_e = -sin_theta * range.x + cos_theta * range.y;
    double top_z = cos_lat * cos_theta * range.x + cos_lat * sin_theta * range.y + sin_lat * range.z;
    double az = atan(-top_e / top_s);
    
    if (top_s > 0.0) {
        az += MathOps::PI;
    }
    
    if (az < 0.0) {
        az += MathOps::TAU;
    }
    
    // range in km
    double mag = range.getMagnitud();
    double el = asin(top_z / mag);
    
    // range rate in km/s
    // double rate = range.dot(range_rate) / mag;
    
    return Horizontal(az, el, RADS);
}

/**
 * parallaticAngle() - compute parallactic angle given latitude, object dec and alt.
 *                      all angles in rads.
 *                     (Meeus, Ch. 93)
 *
 *  https://github.com/brandon-rhodes/pyephem/blob/592ecff661adb9c5cbed7437a23d705555d7ce57/libastro-3.7.7/parallactic.c#L15
 * N.B. always return >= 0, caller must determine sign and degenerate cases at
 *   pole or zenith.
 *
 * @param Observer
 * @param alt - of ecliptic position
 * @param az - of ecliptic position
 *
 * @return parallatic angle
 */
double AstroOps::parallaticAngle( double _lat, double _alt, double _dec ) {
    double ca = sin(_lat);
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

/**
 * positionAngle() - compute the position angle of direction d w.r.t the position r
 *                   Astronomy on the Personal Computer p. 134
 *
 * @param por - position of observed body
 * @param dir - direction of which the position angle is to be calculated
 *
 * @return position angle in rad: range [0, TAU]
 *
 * Note: pos and dir must refer to the same coordinate system and equinox
 */
double AstroOps::positionAngle( const Vector& _pos, const Vector& _dir ) {
    Vector e_1, e_2, e_3;
    double c, s, phi;
    
    e_1 = _pos / _pos.getNormalized();
    e_2 = Vector(0., 0., 1.) * _pos;
    e_2 = e_2 / e_2.getNormalized();
    e_3 = e_1 * e_2;
    
    c = _dir.dot(e_3);
    s = _dir.dot(e_2);
    phi = atan2(s, c);
    return MathOps::mod(phi, MathOps::TAU);
}
