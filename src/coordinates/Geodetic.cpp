#include "Geodetic.h"

#include "../AstroOps.h"
#include <math.h>

Geodetic::Geodetic () : m_alt(0.0) {
}

Geodetic::Geodetic ( double _lng, double _lat, double _alt, ANGLE_TYPE _type) {
    if ( _type == RADS ) {
        m_phi = _lng;
        m_theta = _lat;
    }
    else {
        m_phi = MathOps::toRadians( _lng );
        m_theta = MathOps::toRadians( _lat );
    }
    m_alt = _alt;
}

Geodetic::~Geodetic () {
}

double Geodetic::getLongitude ( ANGLE_TYPE _type ) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_phi );
    }
    else {
        return m_phi;
    }
}

double Geodetic::getLatitude ( ANGLE_TYPE _type ) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_theta );
    }
    else {
        return m_theta;
    }
}

double Geodetic::getRadius () const {
    double pct = abs(sin(m_theta));
    double rad = AstroOps::EARTH_EQUATORIAL_RADIUS_KM * (1.-pct) + AstroOps::EARTH_POLAR_RADIUS_KM * pct;
    rad += m_alt * 0.001; // radious is in KM
    return rad;
}

Vector Geodetic::getVector () const {
    return Vector(*this) * getRadius();
}
