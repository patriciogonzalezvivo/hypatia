#include "Geodetic.h"

#include <math.h>

Geodetic::Geodetic() : m_alt(0.0) {
}

Geodetic::Geodetic( double _lng, double _lat, double _alt, ANGLE_TYPE _type) {
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

Geodetic::~Geodetic(){
}

double Geodetic::getLongitude(ANGLE_TYPE _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_phi );
    }
    else {
        return m_phi;
    }
}

double Geodetic::getLatitude(ANGLE_TYPE _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_theta );
    }
    else {
        return m_theta;
    }
}

double Geodetic::getRadius() const {
    // TODO:
    //      - interpolate between polar and equatorial earth radius
    return m_alt;
}

Vector Geodetic::getVector() const {
    return Vector(*this) * getRadius();
}


