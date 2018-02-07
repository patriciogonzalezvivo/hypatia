#include "GeoPoint.h"

#include <math.h>

GeoPoint::GeoPoint() : m_lng(0.0), m_lat(0.0), m_alt(0.0), m_body(NAB) {
}

GeoPoint::GeoPoint( double _lng, double _lat, double _alt, bool _radians, BodyId _body) {
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
    m_body = _body;
}

GeoPoint::~GeoPoint(){
}

Vector GeoPoint::getGeoVector() const {
    Vector v;
    v.setPolar(m_lng, m_lat);
    return v * m_alt;
}
