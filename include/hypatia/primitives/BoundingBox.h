#pragma once

#include <algorithm>
#include <math.h>
#include <iomanip>

#include "Vector2.h"

struct BoundingBox {

    Vector2 min;
    Vector2 max;

    double  getWidth() const { return fabs(max.x - min.x); }
    double  getHeight() const { return fabs(max.y - min.y); }
    
    Vector2 getCenter() const { return (min + max) * 0.5; }
    
    bool    containsX(double _x) const { return _x >= min.x && _x <= max.x; }
    bool    containsY(double _y) const { return _y >= min.y && _y <= max.y; }
    bool    contains(double _x, double _y) const { return containsX(_x) && containsY(_y); }
    bool    contains(const Vector2& _v) const { return containsX(_v.x) && containsY(_v.y); }

    void    expand(const Vector2& _v) { expand(_v.x, _v.y); }
    void    expand(double _x, double _y) {
        min.x = std::min(min.x, _x);
        min.y = std::min(min.y, _y);
        max.x = std::max(max.x, _x);
        max.y = std::max(max.y, _y);
    }

};

inline std::ostream& operator<<(std::ostream& strm, const BoundingBox& bb) {
    strm << std::setprecision(3);
    strm << "min: " << std::setw(8) << bb.min.x;
    strm << "," << std::setw(8) << bb.min.y;
    strm << " max: " << std::setw(8) << bb.max.x;
    strm << "," << std::setw(8) << bb.max.y;
    return strm;
}
