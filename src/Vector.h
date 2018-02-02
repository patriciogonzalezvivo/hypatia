#pragma once

class Vector {
public:
    Vector();
    virtual ~Vector();

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

    double x, y, z;
};
