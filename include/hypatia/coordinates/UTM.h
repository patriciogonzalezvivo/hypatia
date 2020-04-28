#pragma once

#include <iomanip>

class UTM {
public:
    UTM() {};
    virtual ~UTM() {};

    int     zoneX;
    char    zoneY;
    
    double  easting;
    double  northing;
};


inline std::ostream& operator<<(std::ostream& strm, const UTM& _utm) {
    strm << std::setprecision(3);
    strm << "zone: " << _utm.zoneX << _utm.zoneY;
    strm << ", easting: " << _utm.easting;
    strm << ", northing: " << _utm.northing;
    return strm;
}
