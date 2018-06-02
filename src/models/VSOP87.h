/*****************************************************************************\
 * Vsop.h
 *
 * The Vsop class wraps the VSOP87 data and provides VSOP support fns
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#pragma once

#include "Body.h"  // for enum Body

// * * * * * simple support structs * * * * *

// One VSOP term

struct VSOP87Set {
    double A;
    double B;
    double C;
};

// A set of VSOP terms
struct VSOP87Terms {
    unsigned rows;          // number of term sets
    const VSOP87Set* pTerms;  // pointer to start of data

    VSOP87Terms() : rows(0), pTerms(0) {}
    VSOP87Terms( unsigned r, const VSOP87Set* p ) : rows(r), pTerms(p) {}
};

// A complete collection of VSOP terms (6 Lat, 6 Lon, 6 Rad )

typedef const VSOP87Terms AstroTerms[3*6];

// The main VSOP support class

class VSOP87 {
public:
    // location elements (longitude, latitide, distance)
    //
    enum LocType { ECLIPTIC_LON = 0, ECLIPTIC_LAT = 1, RADIUS = 2 };

    // calculate the spec'd location element of the spec'd body at the given time
    //
    static double calcLoc(
                          double cen,             // time in decimal centuries
                          BodyId planet,          // must be in the range SUN...NEPTUNE
                          LocType value);         // 0=ecliptic lon, 1=ecliptic lat, 2=radius

    // calculate all three location elements of the spec'd body at the given time
    //
    static void calcAllLocs(
                            double& lon,            // returned longitude
                            double& lat,            // returned latitude
                            double& rad,            // returned radius vector
                            double cen,             // time in decimal centuries
                            BodyId planet)          // must be in the range SUN...NEPTUNE
    {
        lon = calcLoc( cen, planet, ECLIPTIC_LON );
        lat = calcLoc( cen, planet, ECLIPTIC_LAT );
        rad = calcLoc( cen, planet, RADIUS );
    }
};  // end class Vsop
