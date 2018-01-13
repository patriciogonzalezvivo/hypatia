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

    // Others
    static double meanObliquity( double _jcentury );

    /* corrects (on place) ra and dec, both in radians, for precession from epoch 1 to epoch 2.
     * the epochs are given by their modified JDs, mjd1 and mjd2, respectively.
     * N.B. ra and dec are modifed IN PLACE.
     */
    static void precession( double mjd1, double mjd2, double &_ra, double &_dec );
};

