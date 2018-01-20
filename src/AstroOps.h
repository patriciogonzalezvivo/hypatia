/*****************************************************************************\
 * AstroOps  is a 'catch-all' class that performs odd handy calculations
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#pragma once

#include "Observer.h"

class AstroOps {
public:
    
    static const double KM_TO_AU;
    static const double AU_TO_KM;
    
    // Transform celestial coordinates ( https://en.wikipedia.org/wiki/Celestial_coordinate_system )

    // to GeoCentric
    static void heliocentricToGeocentric( Observer &_loc, 
                                            double &_body_eclipticLon, double &_body_eclipticLat, double &_body_rad); // OUT

    static void eclipticToEquatorial( Observer &_loc,
                                        double _lng, double _lat,       // IN
                                        double &_ra, double &_dec);     // OUT
    
    // to TopoCentric
    static void equatorialToHorizontal( Observer &_loc,
                                          double _ra, double _dec,      // IN
                                          double &_alt, double &_az);   // OUT


    static double parallaticAngle(  Observer &_loc, 
                                    double _alt, double _dec);

    // Others
    static double meanObliquity( double _jcentury );
};

