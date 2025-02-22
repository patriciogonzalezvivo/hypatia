#pragma once

#include "Vector3.h"

class Matrix3x3 {
public:
    Matrix3x3();
    Matrix3x3(const double* _m);
    Matrix3x3(const double _m00, const double _m01, const double _m02,
              const double _m10, const double _m11, const double _m12,
              const double _m20, const double _m21, const double _m22);

    Matrix3x3(Vector3& _v0, Vector3& _v1, Vector3& _v2);
    Matrix3x3(const Matrix3x3& _m);
    virtual ~Matrix3x3();
    
    static      Matrix3x3 transpose(const Matrix3x3& _m);
    static      Matrix3x3 rotationX(const double _radians);
    static      Matrix3x3 rotationY(const double _radians);
    static      Matrix3x3 rotationZ(const double _radians);
    static      Matrix3x3 rotation(const double _radians, const Vector3& _axis);
    
    void        transpose();
    void        rotateX(const double _radians);
    void        rotateY(const double _radians);
    void        rotateZ(const double _radians);
    
    // product
    Matrix3x3   operator*(const Matrix3x3& _m) const;
    Vector3     operator*(const Vector3& _v) const;
    Matrix3x3&  operator=(const Matrix3x3& _m);
    Matrix3x3&  operator*=(const Matrix3x3& _m);
    Matrix3x3&  operator*=(const double _s);
    Matrix3x3&  operator/=(const double _s);
    Matrix3x3   operator*(const double _s) const;
    Matrix3x3   operator/(const double _s) const;
    
    Vector3     getRow(size_t _i) const;
    Vector3     getColumn(size_t _i) const;

    void            setData(const double* _m);
    double*         getData() { return &m_data[0][0]; }
    const double*   getData() const { return &m_data[0][0]; }
    
protected:

    double m_data[3][3];
};
    