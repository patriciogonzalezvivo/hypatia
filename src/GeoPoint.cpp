#include "GeoPoint.h"

#include <math.h>
#include <sstream>
#include <iomanip>

static char* bodyNames[] = { (char*)"Sun", (char*)"Mer", (char*)"Ven", (char*)"Earth", (char*)"Mar", (char*)"Jup", (char*)"Sat", (char*)"Ur", (char*)"Nep", (char*)"Pl", (char*)"Moon" };


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
    v.setPolar(m_lng, m_lat, true);
    return v * m_alt;
}

std::string GeoPoint::getString() const {
    std::stringstream ss;
    ss << std::right << std::fixed << std::setprecision(3);
    ss << bodyNames[m_body];
    ss << ", Lng: " << std::setw(8) << MathOps::formatDegrees(getLongitude(), D_M_S);
    ss << ", Lat: " << std::setw(8) << MathOps::formatDegrees(getLatitude(), D_M_S);
    ss << ", Alt: " << std::setw(8) << getAltitude();
    return ss.str();
}
