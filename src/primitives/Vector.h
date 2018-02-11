#pragma once

#include "Polar.h"

class Vector {
public:
    Vector();
    Vector(const Polar& _polar);
    Vector(const double _x, const double _y, const double _z);
    
    virtual ~Vector();
    
    double x, y, z;

    virtual double  getMagnitud() const;
    virtual Vector  getNormalized() const;

    virtual double  getLongitude() const;
    virtual double  getLongitudeRadians() const;
    
    virtual double  getLatitude() const;
    virtual double  getLatitudeRadians() const;
    
    virtual double  dot(const Vector& _vec) const;
    
    virtual Vector& normalize();
    virtual Vector& rotate(double _lng, double _lat, bool _radians = false);
    virtual Vector& rotate(double _angle, int _axis, bool _radians = false);
    virtual Vector& rotate(double _angle, const Vector& _axis, bool _radians = false);
    
    virtual double& operator[]( int _n );
    virtual double  operator[]( int _n ) const;
    
    virtual Vector operator+ (const Vector& _vec) const;
    virtual Vector operator- (const Vector& _vec) const;
    virtual Vector operator* (const Vector& _vec) const;
    virtual Vector operator/ (const Vector& _vec) const;
    
    virtual Vector operator+ (double _d) const;
    virtual Vector operator- (double _d) const;
    virtual Vector operator* (double _d) const;
    virtual Vector operator/ (double _d) const;
    
    virtual Vector& operator+= (const Vector& _vec);
    virtual Vector& operator-= (const Vector& _vec);
    virtual Vector& operator*= (const Vector& _vec);
    virtual Vector& operator/= (const Vector& _vec);
    
    virtual Vector& operator+= (double _d);
    virtual Vector& operator-= (double _d);
    virtual Vector& operator*= (double _d);
    virtual Vector& operator/= (double _d);
    
private:
    virtual double * getPtr();
    virtual const double * getPtr() const;
};

inline std::ostream& operator<<(std::ostream& strm, const Vector& v) {
    strm << std::setprecision(3);
    strm << "x: " << std::setw(8) << v.x;
    strm << ", y: " << std::setw(8) << v.y;
    strm << ", z: " << std::setw(8) << v.z;

    return strm;
}
