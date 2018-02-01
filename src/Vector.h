#pragma once

class Vector {
public:
    Vector();
    Vector(double _lng, double _lat, double _radius = 1., bool _radiant = false);
    ~Vector();
    
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
    
    double magnitud() const;
    double dot(const Vector& _vec) const;
    
    double getLongitude() const;
    double getLongitudeRadians() const;
    
    double getLatitude() const;
    double getLatitudeRadians() const;
    
    double getRadius() const;

    double x, y, z;
};
