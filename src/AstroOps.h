/*****************************************************************************\
 * AstroOps  is a 'catch-all' class that performs odd handy calculations
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#pragma once

#include "EcPoint.h"
#include "EqPoint.h"

class AstroOps {
public:
    
    static const double SPEED_OF_LIGHT;
    
    static const double KM_TO_AU;
    static const double AU_TO_KM;
    static const double AU_TO_M;
    static const double AU_PER_DAY;
    
    // Transform celestial coordinates ( https://en.wikipedia.org/wiki/Celestial_coordinate_system )

    // HelioCentric (Ecliptic)
//    static void heliocentricToGeocentric( Observer &_obs,
//                                            double &_body_eclipticLon, double &_body_eclipticLat, double &_body_rad); // OUT
//    
    static EcPoint heliocentricToGeocentric( Observer &_obs, const EcPoint &_heliocentric );

    // GeoCentric
    static EqPoint eclipticToEquatorial ( Observer &_obs, const EcPoint &_ecliptic );
    static void eclipticToEquatorial( Observer &_obs,
                                        double _lng, double _lat,       // IN
                                        double &_ra, double &_dec);     // OUT
    
    // TopoCentric
    static void equatorialToHorizontal( Observer &_obs,
                                          double _ra, double _dec,      // IN
                                          double &_alt, double &_az);   // OUT

    static double parallaticAngle(  Observer &_obs,
                                    double _alt, double _dec);

    // Others
    static double meanObliquity( double _jcentury );
};

