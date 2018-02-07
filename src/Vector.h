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

    virtual double  getMagnitud() const;
    virtual Vector  getNormalized() const;

    virtual double  getLongitude() const;
    virtual double  getLongitudeRadians() const;
    
    virtual double  getLatitude() const;
    virtual double  getLatitudeRadians() const;
    
    virtual double  dot(const Vector& _vec) const;
    virtual void    normalize();
    
    virtual void    rotate(double _lng, double _lat, bool _radians = false);
    virtual void    rotate(double _angle, int _axis, bool _radians = false);
    virtual void    rotate(double _angle, const Vector& _axis, bool _radians = false);
    
    virtual double& operator[]( int n );
    virtual double  operator[]( int n ) const;
    
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
};
