/*****************************************************************************\
 * Body.cpp
 *
 * Handles planetary motion calculations and conversions
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#include "Body.h"

#include "AstroOps.h"

#include "Lunar.h"
#include "Pluto.h"
#include "Vsop.h"

Body::Body() : m_jcentury(-1.), m_r(-1.), m_eclipticLon(-1.), m_eclipticLat(-1.), m_az(-1.), m_alt(-1.), m_dec(-1.), m_ra(-1.), m_bodyId( NAB ) {

};

Body::Body( BodyId _body, Observer& _obs ) : m_jcentury(-1.), m_bodyId(_body) {
    update(_obs);
}

// calculate the data for a given planet, jd, and location
//
void Body::update(Observer& _obs) {
    // There's a lot of calculating here, but the one we need most often
    // is the last one (AltAzLoc), which depends on all the previous
    // calculations
    //
    if (m_jcentury != _obs.getJulianDate()) {
        m_jcentury = _obs.getJulianCentury();

        // choose appropriate method, based on planet
        //
        if (LUNA == m_bodyId) {       /* not VSOP */   

            static Lunar luna;
            luna.update(_obs);
            m_eclipticLon = luna.getEclipticLonRadians();
            m_eclipticLat = luna.getEclipticLatRadians();
            m_r = luna.getRadius();
            m_ra = luna.getRightAscensionRadians();
            m_dec = luna.getDeclinationRadians();
            m_alt = luna.getAltitudRadians();
            m_az = luna.getAzimuthRadians();

        } else {
            if (PLUTO == m_bodyId) {    /* not VSOP */

                Pluto::calcAllLocs(m_eclipticLon, m_eclipticLat, m_r, m_jcentury);
                AstroOps::heliocentricToGeocentric(_obs, m_eclipticLon, m_eclipticLat, m_r);

            }
            else if (SUN == m_bodyId) {
                
                Vsop::calcAllLocs(m_eclipticLon, m_eclipticLat, m_r, m_jcentury, EARTH);
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
                Vsop::calcAllLocs(m_eclipticLon, m_eclipticLat, m_r, m_jcentury, m_bodyId);
                AstroOps::heliocentricToGeocentric(_obs, m_eclipticLon, m_eclipticLat, m_r);
            }
            
            AstroOps::eclipticToEquatorial(_obs, m_eclipticLon, m_eclipticLat, m_ra, m_dec);
            AstroOps::equatorialToHorizontal(_obs, m_ra, m_dec, m_alt, m_az);
        }
    }
}
