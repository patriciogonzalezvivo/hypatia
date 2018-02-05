/*****************************************************************************\
 * AstroOps  is a 'catch-all' class that performs odd handy calculations
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#pragma once

#include "EcPoint.h"
#include "EqPoint.h"

class AstroOps {
public:
    
    static const double SPEED_OF_LIGHT;
    
    static const double KM_TO_AU;
    static const double AU_TO_KM;
    static const double AU_TO_M;
    static const double AU_PER_DAY;
    
    static const double EARTH_POLAR_RADIUS_KM;
    static const double EARTH_EQUATORIAL_RADIUS_KM;
    
    static const double SUN_DIAMETER_KM;
    
    enum SolExType { SPRING, SUMMER, AUTUMN, WINTER };
    
    /**
     * meanSolarLongitude() - calculate the mean solar latitude on the given T
     *                  (Meeus, Ch. 25)
     *
     * @param _jm - day to use in Julian Millenia
     *
     * @return - the longitude in radians
     */
    static double meanSolarLongitude( double _jm );
    
    /**
     * solarLongitude() - calculate the solar latitude on the given JD
     *                   (Meeus, Ch. 25)
     *
     * @param _jd - day to use
     *
     * @return - the longitude in radians
     */
    static double solarLongitude( double _jd );
    
    /**
     * meanSolarAnomaly() - calculate the mean solar anomaly on the given T
     *                     (Meeus, Ch. 25)
     *
     * @param _jc - day to use in Julian Centuries
     *
     * @return - the anomaly in radians
     */
    static double meanSolarAnomaly( double _jc );
    
    /**
     * earthEccentricity() - calculate the eccentricity of the Earth's orbit on
     *                       the given T (Meeus, Ch. 25)
     *
     * @param _jc - day to use in Julian Centuries
     *
     * @return - the eccentricity
     */
    static double earthEccentricity( double _jc );
    
    /**
     * equationOfTime() - calculate the equation of time on the given JD
     *                   (Meeus, Ch. 28)
     *
     * @param jd - day to use
     *
     * @return - the time in minutes between apparent time and mean time
     */
    static double equationOfTime( double _jd );
    
    /**
     * equinoxSolstice() - calculate the time of the Equinoxes and Solstices
     *
     * @param _year - year to calculate quarter for
     * @param _type - which solar quarter (SPRING, etc.)
     * @param _local - true for local time, else UTC
     *
     * @return - the relatively exact jd time of the spec'd event
     */
    static double equinoxSolstice( int _year, SolExType _type, bool _local = false );
    
    /**
     * meanObliquity()  - calculate the mean obliquity for a given jd
     *
     * @param jd - julian day (valid for years -8000 to +12000)
     *
     * @return Mean obliquity (epsilon sub 0) in radians.
     *
     * The obliquity formula (and all the magic numbers below) come from Meeus,
     * Astro Algorithms.
     *
     */
    static double meanObliquity( double _jcentury );
    
    /**
     * nutation() - calculate delta phi and/or delta epsilon for the given jd
     *
     * @param - julian day
     * @param _pDPhi - [out] nutation (delta phi) in arcseconds
     * @param _pDEpsilon - [out] nutation (delta epsilon) in arcseconds
     *
     * The nutation formula (and all the magic numbers below) come from
     * p 132-5, Meeus,  Astro Algorithms
     *
     * Either pointer can be NULL,  in which case that value is
     * not computed.  (we added this because sometimes,  you want
     * only pDPhi or pDEpsilon;  in such cases,  computing _both_
     * is a waste of perfectly good CPU time)
     */
    static void   nutation( double _jd, double* _pDPhi, double* _pDEpsilon );
    
    // Transform celestial coordinates ( https://en.wikipedia.org/wiki/Celestial_coordinate_system )
    // ********************************

    // -------------------------------------------------- HelioCentric (Ecliptic)
    
    static EcPoint heliocentricToGeocentric( Observer &_obs, const EcPoint &_heliocentric );

    // -------------------------------------------------- GeoCentric
    
    /**
     * eclipticToEquatorial() - ecliptic to equatorial coordinates
     *                          (Meeus, Ch. 93)
     *
     * @param Observer
     * @param Ecliptic position
     *
     * @return Equatorial position
     */
    static EqPoint eclipticToEquatorial ( Observer &_obs, const EcPoint &_ecliptic );
    
    /**
     * eclipticToEquatorial() - ecliptic to equatorial coordinates
     *                          (Meeus, Ch. 93)
     *
     * @param Observer
     * @param lng - of ecliptic position (IN)
     * @param lat - of ecliptic position (IN)
     * @param ra - of equatorial position (OUT)
     * @param dec - of equatorial position (OUT)
     *
     */
    static void eclipticToEquatorial( Observer &_obs, double _lng, double _lat, double &_ra, double &_dec);
    
    // -------------------------------------------------- TopoCentric
    
    /**
     * eclipticToEquatorial() - equatorial to horizontal coordinates
     *                          (Meeus, Ch. 93)
     *
     * @param Observer
     * @param ra - of equatorial position (IN)
     * @param ra - of equatorial position (IN)
     * @param alt - of ecliptic position (OUT)
     * @param az - of ecliptic position (OUT)
     *
     */
    static void equatorialToHorizontal( Observer &_obs, double _ra, double _dec, double &_alt, double &_az);

    /**
     * parallaticAngle() - compute parallactic angle given latitude, object dec and alt.
     *                      all angles in rads.
     *                     (Meeus, Ch. 93)
     *
     * N.B. always return >= 0, caller must determine sign and degenerate cases at
     *   pole or zenith.
     *
     * @param Observer
     * @param alt - of ecliptic position
     * @param az - of ecliptic position
     *
     * @return parallatic angle
     */
    static double parallaticAngle(  Observer &_obs, double _alt, double _decv);
    
};

