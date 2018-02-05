#pragma once

class Vector {
public:
    Vector();
    Vector(const double _x, const double _y, const double _z);
    virtual ~Vector();
    
    double x, y, z;

    virtual void setPolar(double _lng, double _lat, bool _radians = false);
    
    virtual double * getPtr();
    virtual const double * getPtr() const;
    virtual double getMagnitud() const;
    
    virtual double  dot(const Vector& _vec) const;
    virtual void    rotate(double _angle, int _axis, bool _radians = false);
    
    double& operator[]( int n );
    double  operator[]( int n ) const;
    
    Vector operator+ (const Vector& _vec) const;
    Vector operator- (const Vector& _vec) const;
    Vector operator* (const Vector& _vec) const;
    Vector operator/ (const Vector& _vec) const;
    
    Vector operator+ (double _d) const;
    Vector operator- (double _d) const;
    Vector operator* (double _d) const;
    Vector operator/ (double _d) const;
    
    Vector& operator+= (const Vector& _vec);
    Vector& operator-= (const Vector& _vec);
    Vector& operator*= (const Vector& _vec);
    Vector& operator/= (const Vector& _vec);
    
    Vector& operator+= (double _d);
    Vector& operator-= (double _d);
    Vector& operator*= (double _d);
    Vector& operator/= (double _d);
};
