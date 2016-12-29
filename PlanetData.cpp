/*****************************************************************************\
 * PlanetData.cpp
 *
 * Handles planetary motion calculations and conversions
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#include "PlanetData.h"

#include "AstroOps.h"

#include "Lunar.h"
#include "Pluto.h"
#include "Vsop.h"

PlanetData::PlanetData() :  m_jd(0), m_planet( NAP ) {
}

PlanetData::PlanetData( Planet _planet, ObsInfo& _obs ) : m_jd(0), m_planet(_planet){
    update(_obs);
}

// calculate the data for a given planet, jd, and location
//
void PlanetData::update(ObsInfo& _obs) {
    // There's a lot of calculating here, but the one we need most often
    // is the last one (AltAzLoc), which depends on all the previous
    // calculations
    //
    if (m_jd != _obs.getJulianDate()) {
        // choose appropriate method, based on planet
        //
        if (LUNA == m_planet) {          /* not VSOP */
            static Lunar luna;
            luna.calcAllLocs(m_eclipticLon, m_eclipticLat, m_r, _obs.getCenturyDate());
        }
        else if (PLUTO == m_planet) {    /* not VSOP */
            Pluto::calcAllLocs(m_eclipticLon, m_eclipticLat, m_r, _obs.getCenturyDate());
            AstroOps::heliocentricToGeocentric(_obs, m_eclipticLon, m_eclipticLat, m_r);
        }
        else if (SUN == m_planet) {
            Vsop::calcAllLocs(m_eclipticLon, m_eclipticLat, m_r, _obs.getCenturyDate(), EARTH);
            /*
             * What we _really_ want is the location of the sun as seen from
             * the earth (geocentric view).  VSOP gives us the opposite
             * (heliocentric) view, i.e., the earth as seen from the sun.
             * To work around this, we add PI to the longitude (rotate 180 degrees)
             * and negate the latitude.
             */
            m_eclipticLon += MathOps::HD_PI;
            m_eclipticLat *= -1.;
        }
        else {
            Vsop::calcAllLocs(m_eclipticLon, m_eclipticLat, m_r, _obs.getCenturyDate(), m_planet);
            AstroOps::heliocentricToGeocentric(_obs, m_eclipticLon, m_eclipticLat, m_r);
        }
        
        AstroOps::eclipticToEquatorial(_obs, m_eclipticLon, m_eclipticLat, m_ra, m_dec);
        AstroOps::equatorialToHorizontal(_obs, m_ra, m_dec, m_alt, m_az);
    }
}
