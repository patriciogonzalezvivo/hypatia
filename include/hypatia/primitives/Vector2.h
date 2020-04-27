#pragma once

#include "Polar.h"

class Vector2 {
public:
    Vector2();
    Vector2(const Polar& _polar);
    Vector2(const double _x, const double _y);
    
    virtual ~Vector2();
    
    double x, y;

    virtual double  getMagnitud() const;
    virtual Vector2 getNormalized() const;
    
    virtual double  dot(const Vector2& _vec) const;
    
    virtual Vector2& normalize();
    
    virtual double& operator[]( int _n );
    virtual double  operator[]( int _n ) const;
    
    virtual Vector2 operator+ (const Vector2& _vec) const;
    virtual Vector2 operator- (const Vector2& _vec) const;
    virtual Vector2 operator* (const Vector2& _vec) const;
    virtual Vector2 operator/ (const Vector2& _vec) const;
    
    virtual Vector2 operator+ (double _d) const;
    virtual Vector2 operator- (double _d) const;
    virtual Vector2 operator* (double _d) const;
    virtual Vector2 operator/ (double _d) const;
    
    virtual Vector2& operator+= (const Vector2& _vec);
    virtual Vector2& operator-= (const Vector2& _vec);
    virtual Vector2& operator*= (const Vector2& _vec);
    virtual Vector2& operator/= (const Vector2& _vec);
    
    virtual Vector2& operator+= (double _d);
    virtual Vector2& operator-= (double _d);
    virtual Vector2& operator*= (double _d);
    virtual Vector2& operator/= (double _d);
    
protected:
    virtual double * getPtr();
    virtual const double * getPtr() const;
};

inline std::ostream& operator<<(std::ostream& strm, const Vector2& v) {
    // strm << std::setprecision(3);
    strm << " " << std::setw(8) << v.x;
    strm << "," << std::setw(8) << v.y;
    return strm;
}
