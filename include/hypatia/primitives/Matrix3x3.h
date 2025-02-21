#pragma once

#include "Vector3.h"

class Matrix3x3 {
public:
    Matrix3x3();
    Matrix3x3(const double _m00, const double _m01, const double _m02,
              const double _m10, const double _m11, const double _m12,
              const double _m20, const double _m21, const double _m22);

    Matrix3x3(Vector3& _v0, Vector3& _v1, Vector3& _v2);
    Matrix3x3(const Matrix3x3& _m);
    virtual ~Matrix3x3();

    // product
    Matrix3x3   operator*(const Matrix3x3& _m) const;
    Vector3     operator*(const Vector3& _v) const;
    
    
protected:

    double m_data[3][3];
};
    