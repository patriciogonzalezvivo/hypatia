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

#include "Luna.h"
#include "Pluto.h"
#include "Vsop.h"

static char* zodiacSigns[] = { (char*)"Ari", (char*)"Tau", (char*)"Gem", (char*)"Cnc", (char*)"Leo", (char*)"Vir", (char*)"Lib", (char*)"Sco", (char*)"Sgr", (char*)"Cap", (char*)"Aqr", (char*)"Psc" };

Body::Body() : 
m_jcentury(0.0), 
m_hEclipticRad(0.0), m_hEclipticLon(0.0), m_hEclipticLat(0.0),
m_gEclipticRad(0.0), m_gEclipticLon(0.0), m_gEclipticLat(0.0),
m_bodyId( NAB ) 
{};

Body::Body( BodyId _body ) {
    m_jcentury = 0.0;
    m_hEclipticRad = 0.0;
    m_hEclipticLon = 0.0;
    m_hEclipticLat = 0.0;
    m_gEclipticRad = 0.0;
    m_gEclipticLon = 0.0;
    m_gEclipticLat = 0.0;
    m_bodyId = _body;
};

Body::~Body() {
}

// calculate the data for a given planet, jd, and location
//
void Body::compute( Observer& _obs ) {
    // There's a lot of calculating here, but the one we need most often
    // is the last one (AltAzLoc), which depends on all the previous
    // calculations
    //
    if (m_jcentury != _obs.getJulianDate()) {
        m_jcentury = _obs.getJulianCentury();

        // choose appropriate method, based on planet
        //
        if (LUNA == m_bodyId) {       /* not VSOP */   

            static Luna luna;
            luna.compute(_obs);
            m_gEclipticLon = luna.getEclipticLonRadians();
            m_gEclipticLat = luna.getEclipticLatRadians();
            m_gEclipticRad = luna.getRadius();
            
            Vector helioCentric = luna.getHeliocentricVector();
            m_hEclipticLon = helioCentric.getLongitudeRadians();
            m_hEclipticLat = helioCentric.getLatitudeRadians();
            m_hEclipticRad = helioCentric.getRadius();
        } 
        else if (PLUTO == m_bodyId) {    /* not VSOP */

            Pluto::calcAllLocs(m_hEclipticLon, m_hEclipticLat, m_hEclipticRad, m_jcentury);
            m_gEclipticLon = m_hEclipticLon;
            m_gEclipticLat = m_hEclipticLat;
            m_gEclipticRad = m_hEclipticRad;
            AstroOps::heliocentricToGeocentric(_obs, m_gEclipticLon, m_gEclipticLat, m_gEclipticRad);
            

        }
        else if (SUN == m_bodyId) {
            m_hEclipticLon = m_hEclipticLat = m_hEclipticRad = 0.0;
            Vsop::calcAllLocs(m_gEclipticLon, m_gEclipticLat, m_gEclipticRad, m_jcentury, EARTH);
            /*
             * What we _really_ want is the location of the sun as seen from
             * the earth (geocentric view).  VSOP gives us the opposite
             * (heliocentric) view, i.e., the earth as seen from the sun.
             * To work around this, we add PI to the longitude (rotate 180 degrees)
             * and negate the latitude.
             */
            m_gEclipticLon += MathOps::HD_PI;
            m_gEclipticLat *= -1.;
        }
        else {
            Vsop::calcAllLocs(m_hEclipticLon, m_hEclipticLat, m_hEclipticRad, m_jcentury, m_bodyId);
            m_gEclipticLon = m_hEclipticLon;
            m_gEclipticLat = m_hEclipticLat;
            m_gEclipticRad = m_hEclipticRad;
            AstroOps::heliocentricToGeocentric(_obs, m_gEclipticLon, m_gEclipticLat, m_gEclipticRad);
        }
        
        if (m_bodyId == EARTH) {
            m_gEclipticLon = m_gEclipticLat = m_gEclipticRad = 0.0;
        }
        
        computeElipcticAngles( _obs );
    }
}

void Body::computeElipcticAngles( Observer& _obs ) {
    AstroOps::eclipticToEquatorial( _obs, m_gEclipticLon, m_gEclipticLat, m_ra, m_dec );
    AstroOps::equatorialToHorizontal( _obs, m_ra, m_dec, m_alt, m_az );
}

char * Body::getZodiacSign() const {
    return zodiacSigns[ int((getEclipticLonRadians()/MathOps::TAU)*12.)%12 ];
}

Vector Body::getHeliocentricVector() const {
    double dist = m_hEclipticRad;
    return Vector(m_hEclipticLon, m_hEclipticLat, dist, true);
}

Vector Body::getGeocentricVector() const {
    double dist = m_gEclipticRad;
    return Vector(m_gEclipticLon, m_gEclipticLat, dist, true);
}
