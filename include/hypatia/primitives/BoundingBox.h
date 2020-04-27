#pragma once

#include <algorithm>
#include <math.h>
#include <iomanip>

struct BoundingBox {

    double minX;
    double minY;
    double maxX;
    double maxY;

    double  getWidth() const { return fabs(maxX - minX); }
    double  getHeight() const { return fabs(maxY - minY); }
    
    double  getCenterX() const { return 0.5 * (minX + maxX); }
    double  getCenterY() const { return 0.5 * (minY + maxY); }
    
    bool    containsX(double x) const { return x >= minX && x <= maxX; }
    bool    containsY(double y) const { return y >= minY && y <= maxY; }
    bool    contains(double x, double y) const { return containsX(x) && containsY(y); }
    
    void    expand(double x, double y) {
        minX = std::min(minX, x);
        minY = std::min(minY, y);
        maxX = std::max(maxX, x);
        maxY = std::max(maxY, y);
    }
};

inline std::ostream& operator<<(std::ostream& strm, const BoundingBox& bb) {
    strm << std::setprecision(3);
    strm << "min: " << std::setw(8) << bb.minX;
    strm << "," << std::setw(8) << bb.minY;
    strm << " max: " << std::setw(8) << bb.maxX;
    strm << "," << std::setw(8) << bb.maxY;
    return strm;
}
