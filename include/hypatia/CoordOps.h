/*****************************************************************************\
 * CoordOps  is a 'catch-all' class that performs odd handy calculations
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#pragma once

#include "Observer.h"

#include "coordinates/Galactic.h"
#include "coordinates/Ecliptic.h"
#include "coordinates/Equatorial.h"
#include "coordinates/Horizontal.h"
#include "coordinates/ECI.h"
#include "coordinates/Geodetic.h"
#include "coordinates/Tile.h"

#include "coordinates/PrecessionMatrix.h"

class CoordOps {
public:
    
    static const double SPEED_OF_LIGHT;
    static const double AU_PER_DAY;
    
    static const double KM_TO_AU;
    static const double AU_TO_KM;
    static const double AU_TO_M;
    static const double AU_TO_LY;
    static const double LY_TO_AU;
    static const double LY_TO_PC;
    static const double PC_TO_LY;

    
    static const double EARTH_FLATTENING;
    static const double EARTH_POLAR_RADIUS_KM;
    static const double EARTH_EQUATORIAL_RADIUS_M;
    static const double EARTH_EQUATORIAL_RADIUS_KM;
    static const double EARTH_EQUATORIAL_CIRCUMFERENCE_M;
    static const double EARTH_EQUATORIAL_HALF_CIRCUMFERENCE_M;
    static const double EARTH_ROTATION_PER_SIDERAL_DAY;
    static const double EARTH_GRAVITATIONAL_CONSTANT;
    
    static const double SUN_DIAMETER_KM;
    
    // Transform celestial coordinates ( https://en.wikipedia.org/wiki/Celestial_coordinate_system )
    // ********************************

    // -------------------------------------------------- to HelioCentric (Ecliptic)
    
    /**
     * toHeliocentric() - ecliptic transformation from geocentric to heliocentric
     *
     * @param Observer
     * @param Ecliptic geocentric
     *
     * @return Ecliptic heliocentric
     */
    static Ecliptic toHeliocentric(Observer& _obs, const Ecliptic& _geocentric);
    
    // -------------------------------------------------- to GeoCentric (Ecliptic)
    
    /**
     * eclipticToEquatorial() - ecliptic transformation from heliocentric to geocentric
     *
     * @param Observer
     * @param Ecliptic heliocentric
     *
     * @return Ecliptic geocentric
     */
    static Ecliptic toGeocentric(Observer& _obs, const Ecliptic& _heliocentric );

    /**
     * toGeocentric() - ) Obliquity, right Ascention and declination to Ecliptic Geocentric
     *                          (Meeus, Ch. 93)
     *
     * @param mean obliquity angle (radians)
     * @param right ascention (radians)
     * @param declination (radians)
     * @param distance (AU)
     *
     * @return Equatorial position
     */
    static Ecliptic toGeocentric (double _obliquity, double _ra, double _dec, double _dist);
    
    /**
     * toGeocentric() - ECI to ecliptic geocentric
     *                          (Meeus, Ch. 93)
     *
     * @param Earth Center Inertial
     *
     * @return Equatorial position
     */
    static Ecliptic toGeocentric (Observer& _obs, const ECI& _eci);
    
    // -------------------------------------------------- to Equatorial

    /**
     * toEquatorial() - Galactic to equatorial coordinates
     *
     * @param Observer
     * @param Galactic position
     *
     * @return Equatorial position
     */
    static Equatorial toEquatorial (const Galactic& _galactic );
    
    /**
     * eclipticToEquatorial() - ecliptic to equatorial coordinates
     *                          (Meeus, Ch. 93)
     *
     * @param mean obliquity angle (radians)
     * @param lng - of ecliptic position (radians)
     * @param lat - of ecliptic position (radians)
     *
     * @return Equatorial position
     */
    static Equatorial toEquatorial( double _obliq, double _lng, double _lat);

    /**
     * toEquatorial() - ecliptic to equatorial coordinates
     *                          (Meeus, Ch. 93)
     *
     * @param Observer
     * @param Ecliptic position
     *
     * @return Equatorial position
     */
    static Equatorial toEquatorial (const Observer& _obs, const Ecliptic& _ecliptic );
    
    // -------------------------------------------------- to Earth Center Innertial (equatorial)
    
    /**
     * toECI() - Geodetic to Earth Center Innertial coordinatws
     *
     * @param julian days
     * @param geodetic coordinate
     *
     * @return Earth Center Innertial
     */
    static ECI toECI(double _jd, const Geodetic& _geod);
    
    // -------------------------------------------------- to Geodetic
    
    /**
     * toGeodetic() - Earth Center Innertial to Geodetic coordinates
     *
     * @param Earth Center Innertial
     *
     * @return Geodetic Coordinate
     */
    static Geodetic toGeodetic(const ECI& _eci);

    // /**
    //  * toTile() - Geodetic location to Tile coordinates 
    //  *
    //  * @param geodetic location (lat/lng)
    //  * @param zoom level
    //  *
    //  * @return tile coords on a mercator tile grid
    //  */
    static Tile toTile(const Geodetic& _geo, int _zoomLevel);
    
    // -------------------------------------------------- to Hour Angle
    /**
     * toHourAngle() - calcuate hour angle (Meeus, Ch. 92)
     *
     * @param lst - observer's local sideral time
     * @param ra - right acention position (radians)
     *
     * @return hour angle
     */
    static double toHourAngle( double _lst, double _ra );
    
    /**
     * toHourAngle() - calcuate hour angle (Meeus, Ch. 92)
     *
     * @param obs - observer
     * @param ra - right acention position (radians)
     *
     * @return hour angle
     */
    static double toHourAngle( const Observer& _obs, double _ra );
    
    /**
     * toHourAngle() - calcuate hour angle (Meeus, Ch. 92)
     *
     * @param obs - observer
     * @param Equatorial coordinate
     *
     * @return hour angle
     */
    static double toHourAngle( const Observer& _obs, const Equatorial& _equatorial );
    
    // -------------------------------------------------- to TopoCentric
    /**
     * eclipticToEquatorial() - equatorial to horizontal coordinates
     *                          (Meeus, Ch. 93)
     *
     * @param lat - Observer's latitud (radians)
     * @param ha - hour angle (radians)
     * @param dec - of equatorial position (radians)
     *
     * @return horizontal position
     */
    static Horizontal toHorizontal( double _lat, double _ha, double _dec);
    
    /**
     * toHorizontal() - to horizontal coordinates
     *                          (Meeus, Ch. 93)
     *
     * @param Observer
     * @param equatorial coordinate
     *
     * @return horizontal position
     */
    static Horizontal toHorizontal( const Observer& _obs, const Equatorial& _equatorial);
    
    /**
     * toECI() - Earth Center Innertial to Horizontal coordinates
     *
     * @param observer
     * @param Earth Center Innertial
     *
     * @return Horizontal coordinates
     */
    static Horizontal toHorizontal(const Observer& _obs, const ECI& _Eci);
    
    // -------------------------------------------------- Other coordinates functions
    
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
     * meanObliquity()  - calculate the mean obliquity for a given jd
     *
     * @param jd - julian day (valid for years -8000 to +12000)
     *
     * @return Mean obliquity (epsilon sub 0) in radians.
     *
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
    
    
    /**
     * anomaly() - calculate delta phi and/or delta epsilon for the given jd
     *
     * @param - mean anomaly of elliptical motion (rad),
     * @param - eccentricity of elliptical motion (rad),
     * @param - return true anomaly (rad)
     * @param - return eccentrict anomaly (rad),
     */
    static void anomaly (double _mean_anomaly, double _eccentricity, double* _true_anomaly, double* _eccentrict_anomaly);
    
    /**
     * parallaticAngle() - compute parallactic angle given latitude, object dec and alt.
     *                      all angles in rads.
     *                     (Meeus, Ch. 98)
     *
     * N.B. always return >= 0, caller must determine sign and degenerate cases at
     *   pole or zenith.
     *
     * @param lat - Observer latitud (radians)
     * @param alt - of ecliptic position (radians)
     * @param az - of ecliptic position (radians)
     *
     * @return parallatic angle
     */
    static double parallaticAngle( double _lat, double _alt, double _dec);
    
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
    static double positionAngle( const Vector3& _pos, const Vector3& _dir );

    /**
     * precess() - precess the given equatorial coordinates
     *
     * @param _matrix - precession matrix
     * @param _equatorial - equatorial coordinates
     *
     * @return precessed equatorial coordinates
     */
    static Equatorial precess(const PrecessionMatrix& _matrix, const Equatorial& _equatorial);


    /**
     * eclipticToEquatorial() - ecliptic to equatorial coordinates
     *                         (Meeus, Ch. 93)
     * *
     * @param mean obliquity angle (radians)
     * * @return transformation matrix
     * 
     */
    static Matrix3x3 eclipticToEquatorial(const double _obliquity);

    /**
     * equatorialToEcliptic() - equatorial to ecliptic coordinates
     *                         (Meeus, Ch. 93)
     * *
     * @param mean obliquity angle (radians)
     * * @return transformation matrix
     * 
     */
    static Matrix3x3 equatorialToEcliptic(const double _obliquity);

    /**
     * eclipticPrecessionFromJ2000() - ecliptic precession matrix from J2000
     *                                 (Meeus, Ch. 93)
     *
     * @param year - year to precess to
     *
     * @return transformation matrix
     */
    static Matrix3x3 eclipticPrecessionFromJ2000(const double _year);
};

