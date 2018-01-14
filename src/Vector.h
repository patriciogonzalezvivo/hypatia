#pragma once

class Vector {
public:
    Vector();
    Vector(double _radiant_lon, double _radiant_lat, double _radius = 1.);
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
    
    double getLongitude() const;
    double getLongitudeRadians() const;
    double getLatitude() const;
    double getLatitudeRadians() const;
    double getRadius() const;

    double x, y, z;
};
