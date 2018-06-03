#include "Geodetic.h"

#include "../AstroOps.h"
#include <math.h>

Geodetic::Geodetic () : m_alt(0.0) {
}

Geodetic::Geodetic ( double _lng, double _lat, double _alt, ANGLE_TYPE _a_type, UNIT_TYPE _r_type) {
    if ( _a_type == RADS ) {
        m_phi = _lng;
        m_theta = _lat;
    }
    else {
        m_phi = MathOps::toRadians( _lng );
        m_theta = MathOps::toRadians( _lat );
    }
    
    if ( _r_type == KM ) {
        m_alt = _alt;
    }
    else {
        m_alt = _alt * AstroOps::AU_TO_KM;
    }
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

double Geodetic::getRadius ( UNIT_TYPE _type ) const {
    double pct = abs(sin(m_theta));
    double rad = AstroOps::EARTH_EQUATORIAL_RADIUS_KM * (1.-pct) + AstroOps::EARTH_POLAR_RADIUS_KM * pct;
    rad += m_alt * 0.001; // radious is in KM
    
    if ( _type == KM ) {
        return rad;
    }
    else {
        return rad * AstroOps::KM_TO_AU;
    }
}

double Geodetic::getAltitude (UNIT_TYPE _type) const {
    if (_type == KM) {
        return m_alt;
    }
    else {
        return m_alt * AstroOps::KM_TO_AU;
    }
};

Vector Geodetic::getVector (UNIT_TYPE _type) const {
    if (_type == KM) {
        return Vector(*this);
    }
    else {
        return Vector(*this) * AstroOps::KM_TO_AU;
    }
}
