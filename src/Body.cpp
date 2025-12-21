/*****************************************************************************\
 * Body.cpp
 *
 * Handles planetary motion calculations and conversions
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#include "hypatia/Body.h"

#include "hypatia/CoordOps.h"

#include "hypatia/Luna.h"
#include "hypatia/models/Pluto.h"
#include "hypatia/models/VSOP87.h"

static char* bodyNames[] = { (char*)"Sun", (char*)"Mercury", (char*)"Venus", (char*)"Earth", (char*)"Mars", (char*)"Jupiter", (char*)"Saturn", (char*)"Uranus", (char*)"Neptune", (char*)"Pluto", (char*)"Luna", (char*)"Satellite" };

static char* zodiacSigns[] = { (char*)"Ari", (char*)"Taurus", (char*)"Gemini", (char*)"Cancer", (char*)"Leo", (char*)"Virgo", (char*)"Libra", (char*)"Scorpion", (char*)"Sagittarius", (char*)"Capricorn", (char*)"Aquarius", (char*)"Pisces" };

//                         Sun,  Mercury, Venus, Earth,  Mars, Jupiter, Saturn, Uranus, Neptune, Pluto,   Moon, Sats
static double period[] = { 0.0, 0.240846, 0.615,   1.0, 1.881,   11.86,  29.46,  84.01,   164.8, 248.1, 0.0751,  0.0 };

Body::Body() : m_jcentury(0.0), m_ha(0.0), m_bodyId( NAB ) {
}

Body::Body( BodyId _body ) : m_jcentury(0.0), m_ha(0.0), m_bodyId( _body ) {
}

Body::~Body() {
}

char*  Body::getName() const {
    return bodyNames[m_bodyId];
}

char* Body::getZodiacSign() const {
    int deg = MathOps::normalize( getEclipticGeocentric().getLongitude(DEGS), DEGS );
    int zod = int(deg / 30) % 12;
    return zodiacSigns[zod];
}

double Body::getHourAngle(ANGLE_UNIT _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_ha );;;
    }
    else {
        return m_ha;
    }
}

double Body::getPeriod(TIME_UNIT _unit) const {
    if (m_bodyId > SUN && m_bodyId < SATELLITE) {
        return TimeOps::yearTo(period[m_bodyId], _unit);
    }
    else {
        return 0.0;
    }
}

// calculate the data for a given planet, jd, and location
//
void Body::compute( Observer& _obs ) {
    // There's a lot of calculating here, but the one we need most often
    // is the last one (AltAzLoc), which depends on all the previous
    // calculations
    //
    // if (m_jcentury != _obs.getJC()) 
    {
        m_jcentury = _obs.getJC();

        // choose appropriate method, based on planet
        //
        if (LUNA == m_bodyId) {       /* not VSOP */   
            static Luna luna;
            luna.compute(_obs);
            m_geocentric = luna.getEclipticGeocentric();
            m_heliocentric = luna.getEclipticHeliocentric();
            
        }
        else if (PLUTO == m_bodyId) {    /* not VSOP */
            double hLng, hLat, rad = 0.0;
            Pluto::calcAllLocs(hLng, hLat, rad, m_jcentury);
            m_heliocentric = Ecliptic(hLng, hLat, rad, RADS, AU);
            m_geocentric = CoordOps::toGeocentric(_obs, m_heliocentric);
        }
        else if (SUN == m_bodyId) {
            double hLng, hLat, rad = 0.0;
            VSOP87::calcAllLocs(hLng, hLat, rad, m_jcentury, EARTH);
            m_heliocentric = Ecliptic(hLng, hLat, rad, RADS, AU);
            
            /*
             * What we _really_ want is the location of the sun as seen from
             * the earth (geocentric view).  VSOP gives us the opposite
             * (heliocentric) view, i.e., the earth as seen from the sun.
             * To work around this, we add PI to the longitude (rotate 180 degrees)
             * and negate the latitude.
             */
            m_geocentric = Ecliptic(hLng + MathOps::PI, hLat * -1., rad, RADS, AU);
        }
        else {
            double hLng, hLat, rad = 0.0;
            VSOP87::calcAllLocs(hLng, hLat, rad, m_jcentury, m_bodyId);
            m_heliocentric = Ecliptic(hLng, hLat, rad, RADS, AU);
            m_geocentric = CoordOps::toGeocentric(_obs, m_heliocentric);
        }
        
        if (m_bodyId == EARTH) {
            m_geocentric = Ecliptic(0., 0., 0., RADS, AU);
        }
        
        m_equatorial = CoordOps::toEquatorial( _obs, m_geocentric );
        
        if ( _obs.haveLocation() ) {
            m_ha = MathOps::normalize(CoordOps::toHourAngle( _obs, m_equatorial ), RADS);
            m_horizontal = CoordOps::toHorizontal( _obs, m_equatorial );
            m_bHorizontal = true;
        }
        else {
            m_ha = 0.0;
            m_horizontal[0] = 0.0;
            m_horizontal[1] = 0.0;
            m_bHorizontal = false;
        }
    }
}