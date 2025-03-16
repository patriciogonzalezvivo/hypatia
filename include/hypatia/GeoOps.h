#pragma once

#include "hypatia/coordinates/Geodetic.h"
#include "hypatia/coordinates/UTM.h"
#include "hypatia/coordinates/Tile.h"
#include "hypatia/primitives/Vector2.h"
#include <iostream>

class GeoOps {
    public:

    static const double EARTH_FLATTENING;
    static const double EARTH_POLAR_RADIUS_KM;
    static const double EARTH_EQUATORIAL_RADIUS_M;
    static const double EARTH_EQUATORIAL_RADIUS_KM;
    static const double EARTH_EQUATORIAL_CIRCUMFERENCE_M;
    static const double EARTH_EQUATORIAL_HALF_CIRCUMFERENCE_M;
    static const double EARTH_ROTATION_PER_SIDERAL_DAY;
    static const double EARTH_GRAVITATIONAL_CONSTANT;

    static const int TIMEZONES_TOTAL;
    static const int COUNTRIES_TOTAL;
    static const int CITIES_TOTAL;

    // Timezones

    /**
     * tzNameToIndex(): convert a time zone string to an index
     * 
     * @param _tz = time zone name string
     * 
     * @return index of time zone
     */
    static size_t tzNameToIndex ( const char* _tz );


    /**
     * tzIndexToName(): convert a time zone index to a string
     * 
     * @param _tzIndex = time zone index
     * 
     * @return time zone name string
     */
    static const char* tzIndexToName ( size_t _tzIndex );

    /**
     * tzOffsetInDays(): calculate time zone offset from Universal Time
     *                   in days for the spec'd time_t (includes DST)
     *
     * @param _tzIndex = time zone index
     *
     * @return Offset (-0.5 ... +0.5 )
     */

    static double tzOffsetInDaysST ( size_t _tzIndex );


    /**
     * tzOffsetInDaysDST(): calculate time zone offset from Universal Time
     *                   in days for the spec'd time_t (includes DST)
     * 
     * @param _tzIndex = time zone index
     * 
     * @return Offset (-0.5 ... +0.5 )
     */

    static double tzOffsetInDaysDST ( size_t _tzIndex );

    /**
     * tzIsDST(): determine if the given date is in Daylight Savings Time
     *
     * @param _lat = latitude of location
     * @param _month = month of year
     * @param _day = day of month
     * @param _year = year
     *
     * @return true if in DST, false otherwise
     */
    static bool tzIsDST ( double _lat, int _month, int _day );

    /**
     * tzOffsetInDays(): calculate time zone offset from Universal Time
     *                   in days for the spec'd time_t (includes DST)
     *
     * @param _lat = latitude of location
     * @param _month = month of year
     * @param _day = day of month
     * @param _tzIndex = time zone index
     *
     * @return Offset (-0.5 ... +0.5 )
     */
    static double tzOffsetInDays ( double _lat, int _month, int _day, size_t _tzIndex );


    // Distance
    static double distance(double _lng1, double _lat1, double _lng2, double _lat2);
    static double distance(const Geodetic& _g1, const Geodetic& _g2);

    // Find closest city Index
    static size_t findClosestCity(double _lng, double _lat);
    static size_t findClosestCity(const Geodetic& _g);

    // return City data
    static std::string getCityName(size_t _index);
    static size_t      getCityCountryIndex(size_t _index);
    static std::string getCityCountry(size_t _index);

    static std::string getCountryName(size_t _index);
    static std::string getCountry2LetterCode(size_t _index);
    
    static size_t getCityTimezoneIndex(size_t _index);
    static std::string getCityTimezone(size_t _index);
    
    static double getCityLongitude(size_t _index);
    static double getCityLatitude(size_t _index);
    static int getCityAltitudeM(size_t _index);
    static Geodetic getCityLocation(size_t _index);

    static UTM toUTM(double _lng, double _lat);
    static UTM toUTM(const Geodetic& _g);

    static Geodetic toGeodetic(const UTM& _utm);
    static Geodetic toGeodetic(const Vector2& _mercator);
    static Geodetic toGeodetic(const Tile& _tile);
    static Geodetic toGeodetic(const Tile& _tile, const Vector2& _uv);

    static Vector2 toMercator( double _lng, double _lat);
    static Vector2 toMercator( const Geodetic& _coord );

    // /**
    //  * toTile() - Geodetic location to Tile coordinates 
    //  *
    //  * @param geodetic location (lat/lng)
    //  * @param zoom level
    //  *
    //  * @return tile coords on a mercator tile grid
    //  */
    static Tile toTile(const Geodetic& _geo, int _zoomLevel);

    static double getMetersPerTileAt(int _zoom);
};