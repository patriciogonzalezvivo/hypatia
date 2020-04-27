#include "hypatia/coordinates/Geodetic.h"
#include "hypatia/CoordOps.h"

#include <math.h>

Geodetic::Geodetic () : m_alt(0.0) {
}

Geodetic::Geodetic ( double _lng, double _lat, double _alt, ANGLE_UNIT _angles_unit, DISTANCE_UNIT _alt_unit) {
    if ( _angles_unit == RADS ) {
        m_phi = _lng;
        m_theta = _lat;
    }
    else {
        m_phi = MathOps::toRadians( _lng );
        m_theta = MathOps::toRadians( _lat );
    }
    
    if ( _alt_unit == KM ) {
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
    else if ( _type == LY ) {
        return rad * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY;
    }
    else if ( _type == PC ) {
        return rad * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY * CoordOps::LY_TO_PC;
    }
    else {
        return rad;
    }
}

double Geodetic::getAltitude (DISTANCE_UNIT _type) const {
    if (_type == KM) {
        return m_alt;
    }
    else if ( _type == AU ) {
        return m_alt * CoordOps::KM_TO_AU;
    }
    else if ( _type == LY ) {
        return m_alt * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY;
    }
    else if ( _type == PC ) {
        return m_alt * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY * CoordOps::LY_TO_PC;
    }
    else {
        return m_alt;
    }
};

Vector Geodetic::getVector (DISTANCE_UNIT _type) const {
    if (_type == KM) {
        return Vector(*this);
    }
    else if ( _type == AU ) {
        return Vector(*this) * CoordOps::KM_TO_AU;
    }
    else if ( _type == LY ) {
        return Vector(*this) * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY;
    }
    else if ( _type == PC ) {
        return Vector(*this) * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY * CoordOps::LY_TO_PC;
    }
    else {
        return Vector(*this);
    }
}
