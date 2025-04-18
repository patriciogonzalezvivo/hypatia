#include "hypatia/coordinates/Geodetic.h"
#include "hypatia/CoordOps.h"
#include "hypatia/GeoOps.h"

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

    m_phi = std::max( std::min( m_phi, MathOps::toRadians( 180.0 ) ), MathOps::toRadians( -180.0 ) );
    m_theta = std::max( std::min( m_theta, MathOps::toRadians( 90.0 ) ), MathOps::toRadians( -90.0 ) );
    
    if ( _alt_unit == KM ) {
        m_alt = _alt;
    }
    else if ( _alt_unit == AU ) {
        m_alt = _alt * CoordOps::AU_TO_KM;
    }
    else if ( _alt_unit == LY ) {
        m_alt = _alt * CoordOps::AU_TO_KM * CoordOps::LY_TO_AU;
    }
    else if ( _alt_unit == PC ) {
        m_alt = _alt * CoordOps::AU_TO_KM * CoordOps::LY_TO_AU * CoordOps::LY_TO_PC;
    }
    else {
        m_alt = _alt;
    }
}

Geodetic::~Geodetic () {
}

void Geodetic::setLongitude( double _lng, ANGLE_UNIT _type ) {
    if ( _type == DEGS ) {
        m_phi = MathOps::toRadians( _lng );
    }
    else {
        m_phi = _lng;
    }
}

void Geodetic::setLatitude( double _lat, ANGLE_UNIT _type ) {
    if ( _type == DEGS ) {
        m_theta = MathOps::toRadians( _lat );
    }
    else {
        m_theta = _lat;
    }
}

void Geodetic::setAltitude( double _alt, DISTANCE_UNIT _type ) {
    if ( _type == KM ) {
        m_alt = _alt;
    }
    else if ( _type == AU ) {
        m_alt = _alt * CoordOps::AU_TO_KM;
    }
    else if ( _type == LY ) {
        m_alt = _alt * CoordOps::AU_TO_KM * CoordOps::LY_TO_AU;
    }
    else if ( _type == PC ) {
        m_alt = _alt * CoordOps::AU_TO_KM * CoordOps::LY_TO_AU * CoordOps::LY_TO_PC;
    }
    else {
        m_alt = _alt;
    }
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
    double rad = GeoOps::EARTH_EQUATORIAL_RADIUS_KM * (1.-pct) + GeoOps::EARTH_POLAR_RADIUS_KM * pct;
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

Vector3 Geodetic::getVector (DISTANCE_UNIT _type) const {
    if (_type == KM) {
        return Vector3(*this);
    }
    else if ( _type == AU ) {
        return Vector3(*this) * CoordOps::KM_TO_AU;
    }
    else if ( _type == LY ) {
        return Vector3(*this) * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY;
    }
    else if ( _type == PC ) {
        return Vector3(*this) * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY * CoordOps::LY_TO_PC;
    }
    else {
        return Vector3(*this);
    }
}
