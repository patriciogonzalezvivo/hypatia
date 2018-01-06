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

    // to GeoCentric
    static void   heliocentricToGeocentric( ObsInfo &_loc, 
                                            double &_planet_eclipticLon, double &_planet_eclipticLat, double &_planet_rad); // OUT

    static void   eclipticToEquatorial( ObsInfo &_loc,
                                        double _lng, double _lat,       // IN
                                        double &_ra, double &_dec);     // OUT
    
    // to TopoCentric
    static void   equatorialToHorizontal( ObsInfo &_loc,
                                          double _ra, double _dec,      // IN
                                          double &_alt, double &_az);   // OUT

    // Others
    static double meanObliquity( ObsInfo &_loc );
    static double meanObliquity( double _jd_century );
};

