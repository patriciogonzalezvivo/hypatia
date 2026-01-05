/*****************************************************************************\
 * CoordOps.cpp
 *
 * CoordOps is a 'catch-all' class that performs misc. handy calculations
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#include "hypatia/CoordOps.h"
#include "hypatia/ProjOps.h"
#include "hypatia/GeoOps.h"

#include "hypatia/MathOps.h"
#include "hypatia/TimeOps.h"

#include "hypatia/primitives/Vector3.h"
#include "hypatia/primitives/Matrix3x3.h"

#include "hypatia/models/VSOP87.h"

#include <math.h>

#define USE_SOLAR_LON 0

const double CoordOps::SPEED_OF_LIGHT = 299792.458;

const double CoordOps::KM_TO_AU = 0.000000006684587;
const double CoordOps::AU_TO_KM = 1.495978707e+8;
const double CoordOps::AU_TO_M = (CoordOps::AU_TO_KM * 1000.);
const double CoordOps::AU_PER_DAY = (86400.0 * CoordOps::SPEED_OF_LIGHT / CoordOps::AU_TO_KM);

const double CoordOps::LY_TO_AU = 63241.077;
const double CoordOps::AU_TO_LY = 1.0/LY_TO_AU;
const double CoordOps::PC_TO_LY = 3.26156;
const double CoordOps::LY_TO_PC = 0.306601;

const double CoordOps::SUN_DIAMETER_KM = 1392000;


//---------------------------------------------------------------------------- to Heliocentric

/**
 * toHeliocentric() - ecliptic transformation from geocentric to heliocentric
 *
 * @param Observer
 * @param Ecliptic geocentric
 *
 * @return Ecliptic heliocentric
 */
Ecliptic CoordOps::toHeliocentric(Observer& _obs, const Ecliptic& _geocentric ){
    
    // Compute Sun's coords
    double sun_eclipticLon, sun_eclipticLat, sun_radius;
    VSOP87::calcAllLocs( sun_eclipticLon, sun_eclipticLat, sun_radius, _obs.getJC(), EARTH);
    
    // Get HelioCentric values
    Ecliptic toEarth = Ecliptic(sun_eclipticLon, sun_eclipticLat, sun_radius, RADS, AU);
    Vector3 Sun2Earth = toEarth.getVector(AU);
    Vector3 Earth2Moon = _geocentric.getVector(AU);
    Vector3 Sun2Moon = Sun2Earth + Earth2Moon;
    
    return Ecliptic(Sun2Moon, AU);
}

//---------------------------------------------------------------------------- to Geocentric

/**
 * toGeocentric() - ecliptic transformation from heliocentric to geocentric
 *
 * @param Observer
 * @param Ecliptic heliocentric
 *
 * @return Ecliptic geocentric
 */
Ecliptic CoordOps::toGeocentric( Observer& _obs, const Ecliptic& _heliocentric ) {
    Vector3 heliocentric = _heliocentric.getVector(AU);
    return Ecliptic(heliocentric - _obs.getHeliocentricVector(AU), AU);
}

/**
 * toGeocentric() - ) Obliquity, right Ascention and declination to Ecliptic Geocentric
 *                          (Meeus, Ch. 93)
 *
 * @param obliquity (radians)
 * @param right ascention (radians)
 * @param declination (radians)
 * @param distance (AU)
 *
 * @return Equatorial position
 */
Ecliptic CoordOps::toGeocentric (double _obliquity, double _ra, double _dec, double _dist) {
    double sin_ra = sin(_ra);
    double cos_ra = cos(_ra);
    
    double sin_dec = sin(_dec);
    double cos_dec = cos(_dec);
    double tan_dec = tan(_dec);
    
    double cos_obliq = cos(_obliquity);
    double sin_obliq = sin(_obliquity);
    
    double lng = atan2( (sin_ra * cos_obliq + tan_dec * sin_obliq), cos_ra );
    double lat = asin( sin_dec * cos_obliq - cos_dec * sin_obliq * sin_ra );

    return Ecliptic(lng, lat, _dist, RADS, AU);
}

/**
 * toGeocentric() - Earth Centered Innertial (Equatorial) to Ecliptic Geocentric
 *                          (Meeus, Ch. 93)
 *
 * @param Earth Center Inertial
 *
 * @return Equatorial position
 */
Ecliptic CoordOps::toGeocentric (Observer& _obs, const ECI& _eci) {
    double distance = _eci.getPosition(AU).getMagnitud();
    Equatorial equat = Equatorial(_eci.getPosition(AU));
    return toGeocentric(_obs.getObliquity(), equat.getRightAscension(RADS), equat.getDeclination(RADS) , distance);
}

//---------------------------------------------------------------------------- to Earth Center Innertial

/**
 * toECI() - Geodetic to Earth Center Innertial coordinatws
 *
 * @param julian days
 * @param geodetic coordinate
 *
 * @return Earth Center Innertial
 */
ECI CoordOps::toECI(double _jd, const Geodetic& _geod) {
    static const double mfactor = MathOps::TAU * (GeoOps::EARTH_ROTATION_PER_SIDERAL_DAY / TimeOps::SECONDS_PER_DAY);
    
    /*
     * Calculate Local Mean Sidereal Time for observers longitude
     */
    const double theta = TimeOps::toLocalSideralTime(_jd, _geod.getLongitude(RADS), RADS);
    
    /*
     * take into account earth flattening
     */
    const double c = 1.0 / sqrt(1.0 + GeoOps::EARTH_FLATTENING * (GeoOps::EARTH_FLATTENING - 2.0) * pow(sin(_geod.getLatitude(RADS)), 2.0));
    const double s = pow(1.0 - GeoOps::EARTH_FLATTENING, 2.0) * c;
    const double achcp = (GeoOps::EARTH_EQUATORIAL_RADIUS_KM * c + _geod.getLatitude(RADS)) * cos(_geod.getLatitude(RADS));
    
    Vector3 position;
    position.x = achcp * cos(theta); // km
    position.y = achcp * sin(theta); // km
    position.z = (GeoOps::EARTH_EQUATORIAL_RADIUS_KM * s + _geod.getLatitude(RADS)) * sin(_geod.getLatitude(RADS)); // km
    
    Vector3 velocity;
    velocity.x = -mfactor * position.y; // km/s
    velocity.y = mfactor * position.x;  // km/s
    velocity.z = 0.0;                   // km/s
    
    return ECI(_jd, position, velocity);
}

//---------------------------------------------------------------------------- to Equatorial

/**
 * toEquatorial() - Galactic to equatorial coordinates
 *
 * @param Observer
 * @param Galactic position
 *
 * @return Equatorial position
 */
Equatorial CoordOps::toEquatorial (const Galactic& _galactic ) {
    double lng = _galactic.getLongitude(RADS);
    double lat = _galactic.getLatitude(RADS);
    double ra = 0.0;
    double dec = 0.0;

// // https://github.com/DarkEnergySurvey/despyastro/blob/master/python/despyastro/coords.py

// #define GAL_E1950
#ifdef GAL_E1950
        double psi      =  4.9261918136;
        double stheta   = -0.88781538514;
        double ctheta   =  0.46019978478;
        double phi      =  0.57595865315;
#else
        double psi      = 4.9368292465;
        double stheta   = -0.88998808748;
        double ctheta   = 0.45598377618;
        double phi      = 0.57477043300;
#endif

    lng -= phi;
    lat = _galactic.getLatitude(RADS);
    
    double slat = sin(lat);
    double clat = cos(lat);
    double clat_slng = clat * sin(lng);
    lat = -stheta * clat_slng + ctheta * slat;
    dec = asin(lat);

    lng = atan2( ctheta * clat_slng + stheta * slat, clat * cos(lng) );
    ra = MathOps::normalize(lng + psi, RADS);
    
    return Equatorial(ra, dec, RADS);
}


/**
 * eclipticToEquatorial() - ecliptic to equatorial coordinates
 *                          (Meeus, Ch. 93)
 *
 *   https://github.com/slowe/VirtualSky/blob/gh-pages/0.7.0/virtualsky.js#L1736
 *
 * @param Observer
 * @param lng - of ecliptic position (IN)
 * @param lat - of ecliptic position (IN)
 *
 * @return Equatorial position
 */
Equatorial CoordOps::toEquatorial ( double _obliq, double _lng, double _lat ) {
    double sin_obliq = sin(_obliq);
    double cos_obliq = cos(_obliq);

    double sl = sin(_lng);
    double cl = cos(_lng);
    double sb = sin(_lat);
    double cb = cos(_lat);
    double tb = tan(_lat);

    double ra = atan2((sl * cos_obliq - tb * sin_obliq),(cl));
    double dec = asin(sb * cos_obliq + cb * sin_obliq * sl);

    // Make sure RA is positive
    if (ra < 0)
        ra += MathOps::TAU;
    
    return Equatorial(ra, dec, RADS);
}

/**
 * toEquatorial() - ecliptic to equatorial coordinates
 *                          (Meeus, Ch. 93)
 *
 * @param Observer
 * @param Ecliptic position
 *
 * @return Equatorial position
 */
Equatorial CoordOps::toEquatorial ( const Observer& _obs, const Ecliptic &_ecliptic ) {
    double lng = _ecliptic.getLongitude(RADS);
    double lat = _ecliptic.getLatitude(RADS);
    return toEquatorial(_obs.getObliquity(), lng, lat);
}

//---------------------------------------------------------------------------- to Hour Angle
/**
 * toHourAngle() - calcuate hour angle
 *                          (Meeus, Ch. 92)
 *
 * @param lst - observer's local sideral time
 * @param ra - right acention position (radians)
 *
 * @return hour angle
 */
double CoordOps::toHourAngle( double _lst, double _ra ) {
    return _lst - _ra;
}

/**
 * toHourAngle() - calcuate hour angle
 *                          (Meeus, Ch. 92)
 *
 * @param Observer
 * @param ra - right acention position (radians)
 *
 * @return hour angle
 */
double CoordOps::toHourAngle( const Observer& _obs, double _ra ) {
    return _obs.getLST() - _ra;
}

/**
 * toHourAngle() - calcuate hour angle
 *                          (Meeus, Ch. 92)
 *
 * @param Observer
 * @param equatorial coordinate
 *
 * @return hour angle
 */
double CoordOps::toHourAngle( const Observer& _obs, const Equatorial &_equatorial ) {
    return _obs.getLST() - _equatorial.getRightAscension(RADS);
}

//---------------------------------------------------------------------------- to Geodetic

Geodetic CoordOps::toGeodetic(const ECI& _eci) {
    const double theta = MathOps::actan(_eci.getPosition(KM).y, _eci.getPosition(KM).x);
    
    double lon = theta - TimeOps::toGreenwichSiderealTime(_eci.getJD());
    //WrapNegPosPI
    lon = MathOps::mod(lon + MathOps::PI, MathOps::TAU) - MathOps::PI;
    
    const double r = sqrt((_eci.getPosition(KM).x * _eci.getPosition(KM).x)
                          + (_eci.getPosition(KM).y * _eci.getPosition(KM).y));
    
    static const double e2 = GeoOps::EARTH_FLATTENING * (2.0 - GeoOps::EARTH_FLATTENING);
    
    double lat = MathOps::actan(_eci.getPosition(KM).z, r);
    double phi = 0.0;
    double c = 0.0;
    int cnt = 0;
    
    do {
        phi = lat;
        const double sinphi = sin(phi);
        c = 1.0 / sqrt(1.0 - e2 * sinphi * sinphi);
        lat = MathOps::actan(_eci.getPosition(KM).z + GeoOps::EARTH_EQUATORIAL_RADIUS_KM * c * e2 * sinphi, r);
        cnt++;
    }
    while (fabs(lat - phi) >= 1e-10 && cnt < 10);
    
    const double alt = r / cos(lat) - GeoOps::EARTH_EQUATORIAL_RADIUS_KM * c;
    
    return Geodetic(lat, lon, alt, RADS, KM);
}


//---------------------------------------------------------------------------- to Horizontal
/**
 * eclipticToEquatorial() - equatorial to horizontal coordinates
 *                          (Meeus, Ch. 93)
 *
 *   https://github.com/slowe/VirtualSky/blob/gh-pages/0.7.0/virtualsky.js#L1736
 *
 * @param Observer's latitud
 * @param hour angle (radians)
 * @param dec - of equatorial position (radians)
 *
 * @return horizontal position
 */
Horizontal CoordOps::toHorizontal ( double _lat, double _ha, double _dec) {
    double sd = sin(_dec);
    double sl = sin(_lat);
    double cl = cos(_lat);
    
    // compute altitude in radians
    double alt = asin(sd*sl + cos(_dec)*cl*cos(_ha));
    
    // compute azimuth in radians
    // divide by zero error at poles or if alt = 90 deg (so we should've already limited to 89.9999)
    double az = acos((sd - sin(alt)*sl)/(cos(alt)*cl));
    
    // choose hemisphere
    if (sin(_ha) > 0.0)
        az = MathOps::TAU - az;
    
    return Horizontal(alt, az, RADS);
}

/**
 * toHorizontal() - equatorial to horizontal coordinates
 *                          (Meeus, Ch. 93)
 *
 * @param Observer
 * @param equatorial coordinate
 *
 * @return horizontal position
 */
Horizontal CoordOps::toHorizontal( const Observer& _obs, const Equatorial& _equatorial) {
    double dec = _equatorial.getDeclination(RADS);
    double ha = toHourAngle(_obs, _equatorial);
    return toHorizontal(_obs.getLocation().getLatitude(RADS), ha, dec);
}

/**
 * toEquatorial() - Eart Centered Innertial to horizontal coordinates
 *                          (Meeus, Ch. 93)
 *
 * @param Observer
 * @param equatorial coordinate
 *
 * @return horizontal position
 */
Horizontal CoordOps::toHorizontal(const Observer& _obs, const ECI& _eci) {
    ECI obs = ECI(_eci.getJD(), _obs.getLocation());
    
    /*
     * calculate differences
     */
    Vector3 range_rate = _eci.getVelocity(KM) - obs.getVelocity(KM);
    Vector3 range = _eci.getPosition(KM) - obs.getPosition(KM);
    
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


//----------------------------------------------------------------------------
/**
 * meanSolarLongitude() - calculate the mean solar latitude on the given T
 *                  (Meeus, Ch. 25)
 *
 * @param t - day to use in Julian Millenia
 *
 * @return - the longitude in radians
 */
double CoordOps::meanSolarLongitude( double t ) {
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
double CoordOps::solarLongitude( double jd ) {
    
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
double CoordOps::meanSolarAnomaly( double _jc ) {
    return MathOps::toRadians(357.52911 + 35999.05029 * _jc - 0.0001537 * _jc * _jc);
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
double CoordOps::earthEccentricity( double _jc ) {
    return 0.016708634 - 0.000042037*_jc - 0.0000001267*_jc*_jc;
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
double CoordOps::meanObliquity( double _jcentury ){
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
void CoordOps::nutation( double jd, double* pDPhi, double* pDEpsilon ) {
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

/**
 * anomaly() - calculate delta phi and/or delta epsilon for the given jd
 *
 * @param - mean anomaly of elliptical motion (rad),
 * @param - eccentricity of elliptical motion (rad),
 * @param - return true anomaly (rad)
 * @param - return eccentrict anomaly (rad),
 */
void CoordOps::anomaly (double _mean_anomaly, double _eccentricity, double* _true_anomaly, double* _eccentrict_anomaly) {
    double m, fea, corr;
    
    if (_eccentricity < 1.0) {
        /* elliptical */
        double dla;
        
        m = _mean_anomaly - MathOps::TAU * (long)(_mean_anomaly / MathOps::TAU);
        
        if (m > MathOps::PI) {
            m -= MathOps::TAU;
        }
        
        if (m < -MathOps::PI) {
            m += MathOps::TAU;
        }
        
        fea = m;
        
        for (;;) {
            dla = fea - ( _eccentricity * sin(fea) ) - m;
            if (fabs(dla) < (1e-8)) {
                break;
            }
            /* avoid runnaway corrections for e>.97 and M near 0*/
            corr = 1.0 - ( _eccentricity * cos(fea));
            if (corr < 0.1) {
                corr = 0.1;
            }
            dla /= corr;
            fea -= dla;
        }
        *_true_anomaly = 2.0 * atan(sqrt((1.0 + _eccentricity) / (1.0 - _eccentricity)) * tan(fea * 0.5));
    } else {
        /* hyperbolic */
        double fea1;
        
        m = fabs(_mean_anomaly);
        fea = m / (_eccentricity - 1.);
        fea1 = pow(6.0 * m / (_eccentricity * _eccentricity), 1./3.);
        
        /* whichever is smaller is the better initial guess */
        if (fea1 < fea) {
            fea = fea1;
        }
        
        corr = 1;
        while (fabs(corr) > (1e-8)) {
            corr = (m - _eccentricity * sinh(fea) + fea) / (_eccentricity * cosh(fea) - 1.0);
            fea += corr;
        }
        if (_mean_anomaly < 0.) {
            fea = -fea;
        }
        *_true_anomaly = 2.0 * atan(sqrt(( _eccentricity + 1.0) /( _eccentricity - 1.0)) * tanh(fea * 0.5));
    }
    *_eccentrict_anomaly = fea;
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
double CoordOps::parallaticAngle( double _lat, double _alt, double _dec ) {
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
double CoordOps::positionAngle( const Vector3& _pos, const Vector3& _dir ) {
    Vector3 e_1, e_2, e_3;
    double c, s, phi;
    
    e_1 = _pos / _pos.getNormalized();
    e_2 = Vector3(0., 0., 1.) * _pos;
    e_2 = e_2 / e_2.getNormalized();
    e_3 = e_1 * e_2;
    
    c = _dir.dot(e_3);
    s = _dir.dot(e_2);
    phi = atan2(s, c);
    return MathOps::mod(phi, MathOps::TAU);
}

/*
    * precess() - precess equatorial coordinates
    *                          (Meeus, Ch. 93)
    *
    * @param PrecessionMatrix
    * @param Equatorial position
    *
    * @return Equatorial position
*/

Equatorial CoordOps::precess(const PrecessionMatrix& _matrix, const Equatorial& _equatorial) {
    
    const double old_ra = _equatorial.getRightAscension(RADS);
    
    Vector3 v1 = _equatorial.getVector();
    
    double ra, dec;
    
    Vector3 v2;
    if( _matrix.isBackward())
        v2 = _matrix.deprecess(v1);
    else
        v2 = _matrix.precess(v1);

    if (v2.y != 0.0 || v2.x != 0.0)
        ra = atan2( v2.y, v2.x);
    else
        ra = 0.0;

    dec = MathOps::asine(v2.z);
    
    while (ra - old_ra > MathOps::PI)
        ra -= MathOps::TAU;
    while (ra - old_ra < - MathOps::PI)
        ra += MathOps::TAU;
    
    return Equatorial(ra, dec, RADS);
}

Matrix3x3 CoordOps::equatorialToEcliptic(const double _obliquity) {
    double sin_obliq = sin(_obliquity);
    double cos_obliq = cos(_obliquity);
    
    return Matrix3x3(
        1.0, 0.0, 0.0,
        0.0, cos_obliq, -sin_obliq,
        0.0, sin_obliq, cos_obliq
    );
}

Matrix3x3 CoordOps::eclipticToEquatorial(const double _obliquity) {
    return Matrix3x3::transpose( equatorialToEcliptic(_obliquity) );
}

// Based on https://github.com/Bill-Gray/lunar/blob/master/precess.cpp#L109
//
Matrix3x3 CoordOps::eclipticPrecessionFromJ2000(const double _year) {
    double t = (_year - 2000.0) / 100.0; // t in Julian centuries from J2000
    const double eta = t * (47.0029 * MathOps::ARCS_TO_RADS + (-.03302 * MathOps::ARCS_TO_RADS + 6.e-5 * MathOps::ARCS_TO_RADS * t) * t);
    const double pie = 174.876384 * MathOps::DEGS_TO_RADS - t * (869.8089 * MathOps::ARCS_TO_RADS - .03536 * MathOps::ARCS_TO_RADS * t);
    const double p = t * (5029.0966 * MathOps::ARCS_TO_RADS + (1.11113 * MathOps::ARCS_TO_RADS - 6.e-5 * MathOps::ARCS_TO_RADS * t) * t);

    Matrix3x3 m = Matrix3x3();
    m *= Matrix3x3::rotationZ(-pie);
    m *= Matrix3x3::rotationY(-eta);
    m *= Matrix3x3::rotationZ(p);
    m *= Matrix3x3::rotationZ(pie);
    return m;
}