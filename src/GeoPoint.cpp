#include "GeoPoint.h"

#include <math.h>

GeoPoint::GeoPoint() : m_lng(0.0), m_lat(0.0), m_alt(0.0) {
}

GeoPoint::GeoPoint( double _lng, double _lat, double _alt, bool _radians) {
    if (_radians)
    {
        m_lng = _lng;
        m_lat = _lat;
    }
    else
    {
        m_lng = MathOps::toRadians( _lng );
        m_lat = MathOps::toRadians( _lat );
    }
    m_alt = _alt;
}

GeoPoint::~GeoPoint(){
}

Vector GeoPoint::getVector() const {
    return Vector(Polar(m_lng, m_lat, true)) * m_alt;
}


