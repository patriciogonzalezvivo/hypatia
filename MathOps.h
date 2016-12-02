/*****************************************************************************\
 * Astro     defines misc. handy constants & fns
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
 \*****************************************************************************/

#pragma once

// * * * * * Handy Type Definitions * * * * *
typedef double AstroVector[3];

// * * * * * Generic Handy Constants and Conversions * * * * *

struct MathOps {
    static const double HD_PI;
    static const double TAU;
    static const double PI_OVER_TWO;
    static const double TWO_OVER_PI;
    static const double DEG_PER_CIRCLE;
    static const double MINUTES_PER_DEGREE;
    static const double SECONDS_PER_DEGREE;
    
    static const double TO_RADS;
    
    // convert degrees to radians
    static double toRadians( double deg ) { return deg * TO_RADS; }
    
    // convert radians to degrees
    static double toDegrees( double rad ) { return rad / TO_RADS; }
    
    static double normalizeDegrees( double d );
    static double normalizeRadians( double d );
    
    static void rotateVector( AstroVector& _v, double _radiant_angle, int _axis );
    static void polar3ToCartesian( AstroVector& _v, double _radiant_lon, double _radiant_lat );
    static void polar3ToCartesian( AstroVector& _v, double _radiant_lon, double _radiant_lat, double _radius );
};
