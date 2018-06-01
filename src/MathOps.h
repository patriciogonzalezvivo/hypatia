/*****************************************************************************\
 * MathOps  defines misc. handy constants & mathematical fns
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
 \*****************************************************************************/

#pragma once
#undef PI

enum UNIT_TYPE {
    KM, AU
};

enum ANGLE_TYPE {
    DEGS, RADS
};

enum ANGLE_FMT {
    Dd,        // 16.2°
    Ddd,       // 16.29°
    Dddd,      // 16.294°
    D_Mm,      // 12° 36.1'
    D_M_Ss,    // 12° 36' 59.1"
    Hs,        // 15.1h
    Hss,       // 15.12h
    Hsss,      // 15.123h
    H_Mm,      // 15h 15,3m
    H_M_Ss     // 15h 15m 32.9s
};

struct MathOps {
    static const double PI;
    static const double TAU;
    static const double PI_OVER_TWO;
    static const double TWO_OVER_PI;
    static const double DEG_PER_CIRCLE;
    static const double RAD_PER_CIRCLE;
    
    static const double MINUTES_PER_DEGREE;
    static const double SECONDS_PER_DEGREE;
    static const double RADS_PER_DEGREE;
    
    static const double RADS_TO_DEGS;
    static const double DEGS_TO_RADS;
    static const double DEGS_TO_HRS;

    static const double RADS_TO_ARCS;
    static const double ARCS_TO_RADS;

    static const double RADIAN;
    
    static const double ROUND;
    static const double INVALID;
    
    static const double TWO_THIRD;

    /**
     * toDegrees(): convert radians to degrees
     *
     * @param an angle in radians
     *
     * @return an angle in degrees
     */
    static double toDegrees( double _rad );

    /**
     * toDegrees(): convert radians to degrees
     *
     * @param an degrees
     * @param an minutes
     * @param an seconds
     *
     * @return an angle in degrees
     */
    static double toDegrees( int _deg, int _min, double _sec );
    
    /**
     * toRadians(): convert degrees to radians
     *
     * @param an angle in degrees
     *
     * @return an angle in radians
     */
    static double toRadians( double _deg );
    
    /**
     * secToRadians(): convert arcseconds to radians
     *
     * @param an angle in arcseconds
     *
     * @return an angle in radians
     */
    static double secToRadians( double _sec );
    
    /**
     * toHrs(): convert arcseconds to radians
     *
     * @param degrees
     *
     * @return hours double
     */
    static double toHrs( double _angle, ANGLE_TYPE _type );
    
    /**
     * toDMS(): convert angle to degrees, minutes and seconds
     *
     * @param angle in
     * @param angle type
     *
     * @param degrees out
     * @param minutes out
     * @param seconds out
     *
     */
    static void toDMS ( double _angles, ANGLE_TYPE _type, int &_deg, int &_min, double &_sec );
    
    /**
     * toHMS(): convert angles to degrees, minutes and seconds
     *
     * @param angle in
     * @param angle type
     *
     * @param hours out
     * @param minutes out
     * @param seconds out
     *
     */
    static void toHMS ( double _deg, ANGLE_TYPE _type, int &_hrs, int &_min, double &_sec );
    
    /**
     * formatAngle(): format angle into a string
     *
     * @param angle value
     * @param angle type
     * @param format type
     *
     * @return formated string
     *
     */
    static char* formatAngle ( double _angle, ANGLE_TYPE _type, ANGLE_FMT _format );
      
    /**
     * normalize(): reduce an angle to the range (0 <= d < 360 or 0 <= d << TAO)
     *
     * @param angle
     * @param angle type
     *
     *
     * @return a
     */
    static double normalize( double _angle, ANGLE_TYPE _type );
    
    /**
     *  quadrant(): returns the quadrant ( 0, 1, 2, or 3 ) of the spec'd angle
     *
     * @param angle - in radians
     *
     * @return quadrant of angle
     */
    static int quadrant( double _rad );
    
    /**
     * acose(): "safe" acos which prevents overflow errors
     *
     * @param angle
     *
     * @return acos (0 ... PI)
     */
    static double acose( double _rad );
    
    /**
     * asine(): "safe" asine which prevents overflow errors
     *
     * @param angle
     *
     * @return asin (PI/2 ... -PI/2)
     */
    static double asine( double _rad );

    /**
     * actan(): 
     *
     * @param sinx
     * @param cosx
     *
     * @return actan
     */
    static double actan(const double sinx, const double cosx);
    
    /**
     * fract(): gives the fractional part of a number
     *
     * @param number
     *
     * @return fractional part of it 
     */
    static double fract (double _x);

    /**
     * mod(): calculates x modulus y
     *
     * @param base
     *
     * @return modulus of
     */
    static long mod ( long x, long y );
    static double mod ( double _x, double _y );
};
