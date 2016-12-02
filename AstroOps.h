/*****************************************************************************\
 * AstroOps  is a 'catch-all' class that performs odd handy calculations
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#pragma once

#include "ObsInf.h"

// * * * * * Handy functions that didn't fit anywhere else * * * * *

class AstroOps {
public:
    // Transform celestial coordinates ( https://en.wikipedia.org/wiki/Celestial_coordinate_system )
    static void   eclipticToEquatorial( double _jd,
                                        double _lng, double _lat,       // IN
                                        double &_ra, double &_dec);     // OUT
    
    static void   equatorialToHorizontal( ObsInfo _loc,
                                          double _ra, double _dec,
                                          double &_alt, double &_az);
    
    static void   heliocentricToGeocentric( ObsInfo _loc,
                                            double planet_eclipticLon, double planet_eclipticLat, double planet_rad,
                                            double &_ra, double &_dec);
    
    static double meanObliquity( double _jd);
    static void   nutation( double t, double* d_lon, double* d_obliq );
};

