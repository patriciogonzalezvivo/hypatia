#pragma once

#include "Polar.h"

class Vector3 {
public:
    Vector3();
    Vector3(const Polar& _polar);
    Vector3(const double _x, const double _y, const double _z);
    
    virtual ~Vector3();
    
    double x, y, z;

    virtual double  getMagnitud() const;
    virtual Vector3  getNormalized() const;

    virtual double  getLongitude(ANGLE_UNIT _type) const;
    virtual double  getLatitude(ANGLE_UNIT _type) const;
    
    virtual double  dot(const Vector3& _vec) const;
    
    virtual Vector3& normalize();
    virtual Vector3& rotate(double _lng, double _lat, bool _radians = false);
    virtual Vector3& rotate(double _angle, int _axis, bool _radians = false);
    virtual Vector3& rotate(double _angle, const Vector3& _axis, bool _radians = false);
    
    virtual double& operator[]( int _n );
    virtual double  operator[]( int _n ) const;
    
    virtual Vector3 operator+ (const Vector3& _vec) const;
    virtual Vector3 operator- (const Vector3& _vec) const;
    virtual Vector3 operator* (const Vector3& _vec) const;
    virtual Vector3 operator/ (const Vector3& _vec) const;
    
    virtual Vector3 operator+ (double _d) const;
    virtual Vector3 operator- (double _d) const;
    virtual Vector3 operator* (double _d) const;
    virtual Vector3 operator/ (double _d) const;
    
    virtual Vector3& operator+= (const Vector3& _vec);
    virtual Vector3& operator-= (const Vector3& _vec);
    virtual Vector3& operator*= (const Vector3& _vec);
    virtual Vector3& operator/= (const Vector3& _vec);
    
    virtual Vector3& operator+= (double _d);
    virtual Vector3& operator-= (double _d);
    virtual Vector3& operator*= (double _d);
    virtual Vector3& operator/= (double _d);
    
protected:
    virtual double * getPtr();
    virtual const double * getPtr() const;
};

inline std::ostream& operator<<(std::ostream& strm, const Vector3& v) {
    strm << std::setprecision(3);
    strm << " " << std::setw(8) << v.x;
    strm << "," << std::setw(8) << v.y;
    strm << "," << std::setw(8) << v.z;

    return strm;
}
