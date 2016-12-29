/*****************************************************************************\
 * Astro     defines misc. handy constants & fns
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
 \*****************************************************************************/

#pragma once

// * * * * * Handy Type Definitions * * * * *
struct AstroVector {
    AstroVector();
    AstroVector(double _radiant_lon, double _radiant_lat, double _radius = 1.);
    
    double getLongitud();
    double getLatitud();
    double getRadius();

    double x;
    double y;
    double z;
};

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
    static void toDegreesMinSec ( double rad, int &_deg, int &_min, int &_sec );
    
    static double normalizeDegrees( double d );
    static double normalizeRadians( double d );
};
