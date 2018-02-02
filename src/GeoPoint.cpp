#include "GeoPoint.h"
#include "MathOps.h"

GeoPoint::GeoPoint() : longitude(0.0), latitude(0.0), altitude(0.0) {
}

GeoPoint::GeoPoint( double _lng, double _lat, double _alt, bool _radians) {
    if (_radians)
    {
        longitude = _lng;
        latitude = _lat;
    }
    else
    {
        longitude = MathOps::toRadians( _lng );
        latitude = MathOps::toRadians( _lat );
    }
    altitude = _alt;
}

GeoPoint::~GeoPoint(){
    
}
