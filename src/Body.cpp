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

static char* bodyNames[] = { (char*)"Sun", (char*)"Mer", (char*)"Ven", (char*)"Earth", (char*)"Mar", (char*)"Jup", (char*)"Sat", (char*)"Ur", (char*)"Nep", (char*)"Pl", (char*)"Moon" };

static char* zodiacSigns[] = { (char*)"Ari", (char*)"Tau", (char*)"Gem", (char*)"Cnc", (char*)"Leo", (char*)"Vir", (char*)"Lib", (char*)"Sco", (char*)"Sgr", (char*)"Cap", (char*)"Aqr", (char*)"Psc" };

Body::Body() : m_jcentury(0.0), m_bodyId( NAB ){
    
}

Body::Body( BodyId _body ) {
    m_jcentury = 0.0;
    m_bodyId = _body;
}

Body::~Body() {
}

// calculate the data for a given planet, jd, and location
//
void Body::compute( Observer& _obs ) {
    // There's a lot of calculating here, but the one we need most often
    // is the last one (AltAzLoc), which depends on all the previous
    // calculations
    //
    if (m_jcentury != _obs.getJC()) {
        m_jcentury = _obs.getJC();

        // choose appropriate method, based on planet
        //
        if (LUNA == m_bodyId) {       /* not VSOP */   
            static Luna luna;
            luna.compute(_obs);
            m_geocentric = luna.getGeocentricEcliptic();
            m_heliocentric = luna.getHeliocentricEcliptic();
            
        }
        else if (PLUTO == m_bodyId) {    /* not VSOP */
            double hLng, hLat, rad = 0.0;
            Pluto::calcAllLocs(hLng, hLat, rad, m_jcentury);
            m_heliocentric = EcPoint(hLng, hLat, rad, true);
            m_geocentric = AstroOps::heliocentricToGeocentric(_obs, m_heliocentric);
        }
        else if (SUN == m_bodyId) {
            double hLng, hLat, rad = 0.0;
            Vsop::calcAllLocs(hLng, hLat, rad, m_jcentury, EARTH);
            m_heliocentric = EcPoint(hLng, hLat, rad, true);
            
            /*
             * What we _really_ want is the location of the sun as seen from
             * the earth (geocentric view).  VSOP gives us the opposite
             * (heliocentric) view, i.e., the earth as seen from the sun.
             * To work around this, we add PI to the longitude (rotate 180 degrees)
             * and negate the latitude.
             */
            m_geocentric = EcPoint(hLng + MathOps::PI, hLat * -1., rad, true);
        }
        else {
            double hLng, hLat, rad = 0.0;
            Vsop::calcAllLocs(hLng, hLat, rad, m_jcentury, m_bodyId);
            m_heliocentric = EcPoint(hLng, hLat, rad, true);
            m_geocentric = AstroOps::heliocentricToGeocentric(_obs, m_heliocentric);
        }
        
        if (m_bodyId == EARTH) {
            m_geocentric = EcPoint(0., 0., 0.);
        }
        
        computeElipcticAngles( _obs );
    }
}

void Body::computeElipcticAngles( Observer& _obs ) {
    AstroOps::eclipticToEquatorial( _obs, m_geocentric.getLongitudeRadians(), m_geocentric.getLatitudeRadians(), m_ra, m_dec );
    
    m_hourAngle = _obs.getLST() - m_ra;
    AstroOps::equatorialToHorizontal( _obs, m_ra, m_dec, m_alt, m_az );
}

char*  Body::getBodyName() const {
    return bodyNames[m_bodyId];
}

char * Body::getZodiacSign() const {
    return zodiacSigns[ int((m_geocentric.getLongitudeRadians()/MathOps::TAU)*12.)%12 ];
}
