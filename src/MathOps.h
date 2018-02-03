/*****************************************************************************\
 * MathOps  defines misc. handy constants & mathematical fns
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
 \*****************************************************************************/

#pragma once
#undef PI

struct MathOps {
    static const double PI;
    static const double TAU;
    static const double PI_OVER_TWO;
    static const double TWO_OVER_PI;
    static const double DEG_PER_CIRCLE;
    static const double MINUTES_PER_DEGREE;
    static const double SECONDS_PER_DEGREE;
    
    static const double RADS_TO_DEGS;
    static const double DEGS_TO_RADS;
    static const double DEGS_TO_HRS;

    static const double RADS_TO_ARCS;
    static const double ARCS_TO_RADS;

    static const double RADIAN;

    static double rangeDegrees( double deg );
    static double rangeRadians( double rad );
    
    // convert radians to degrees
    static double toDegrees( double rad ) { return rad * RADS_TO_DEGS; };

    // convert degrees to radians
    static double toRadians( double deg ) { return deg * DEGS_TO_RADS; };
    
    static double toHrs( double deg ) { return deg * DEGS_TO_HRS; };

    static void toDMS ( double deg, int &_deg, int &_min, double &_sec );
    static void toHMS ( double deg, int &_hrs, int &_min, double &_sec );
};
