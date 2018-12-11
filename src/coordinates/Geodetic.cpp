#include "Geodetic.h"

#include "../CoordOps.h"
#include <math.h>

Geodetic::Geodetic () : m_alt(0.0) {
}

Geodetic::Geodetic ( double _lng, double _lat, double _alt, ANGLE_UNIT _a_type, DISTANCE_UNIT _r_type) {
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
        m_alt = _alt * CoordOps::AU_TO_KM;
    }
}

Geodetic::~Geodetic () {
}

double Geodetic::getLongitude ( ANGLE_UNIT _type ) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_phi );
    }
    else {
        return m_phi;
    }
}

double Geodetic::getLatitude ( ANGLE_UNIT _type ) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_theta );
    }
    else {
        return m_theta;
    }
}

double Geodetic::getRadius ( DISTANCE_UNIT _type ) const {
    double pct = abs(sin(m_theta));
    double rad = CoordOps::EARTH_EQUATORIAL_RADIUS_KM * (1.-pct) + CoordOps::EARTH_POLAR_RADIUS_KM * pct;
    rad += m_alt * 0.001; // radious is in KM
    
    if ( _type == KM ) {
        return rad;
    }
    else if ( _type == AU ) {
        return rad * CoordOps::KM_TO_AU;
    }
    else {
        // PC and LY are to big of a scale
        return 0.0;
    }
}

double Geodetic::getAltitude (DISTANCE_UNIT _type) const {
    if (_type == KM) {
        return m_alt;
    }
    else if ( _type == AU ) {
        return m_alt * CoordOps::KM_TO_AU;
    }
    else {
        // PC and LY are to big of a scale
        return 0.0;
    }
};

Vector Geodetic::getVector (DISTANCE_UNIT _type) const {
    if (_type == KM) {
        return Vector(*this);
    }
    else if ( _type == AU ) {
        return Vector(*this) * CoordOps::KM_TO_AU;
    }
    else {
        // PC and LY are to big of a scale
        return Vector(*this) * 0.0;
    }
}
