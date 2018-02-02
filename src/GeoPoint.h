#pragma once

class GeoPoint {
public:
    GeoPoint();
    GeoPoint( double _lng, double _lat, double _alt, bool _radians = false);
    virtual ~GeoPoint();
    
    double  longitude, altitude, latitude;
};
