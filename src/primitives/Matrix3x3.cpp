//
//  Matrix3x3.cpp
//
//  Created by Patricio González Vivo on 2/21/25.
//

#include "hypatia/primitives/Matrix3x3.h"

#include <math.h>
#include <cstring>

Matrix3x3::Matrix3x3() {
    // identity
    m_data[0][0] = 1.0;
    m_data[0][1] = 0.0;
    m_data[0][2] = 0.0;

    m_data[1][0] = 0.0;
    m_data[1][1] = 1.0;
    m_data[1][2] = 0.0;

    m_data[2][0] = 0.0;
    m_data[2][1] = 0.0;
    m_data[2][2] = 1.0;
}

Matrix3x3::Matrix3x3(const double* _m) { setData(_m); }

Matrix3x3::Matrix3x3(const double _m00, const double _m01, const double _m02,
                     const double _m10, const double _m11, const double _m12,
                     const double _m20, const double _m21, const double _m22) {
    m_data[0][0] = _m00;
    m_data[0][1] = _m01;
    m_data[0][2] = _m02;
    
    m_data[1][0] = _m10;
    m_data[1][1] = _m11;
    m_data[1][2] = _m12;
    
    m_data[2][0] = _m20;
    m_data[2][1] = _m21;
    m_data[2][2] = _m22;
}

Matrix3x3::Matrix3x3(Vector3& _v0, Vector3& _v1, Vector3& _v2) {
    m_data[0][0] = _v0.x;
    m_data[0][1] = _v0.y;
    m_data[0][2] = _v0.z;
    
    m_data[1][0] = _v1.x;
    m_data[1][1] = _v1.y;
    m_data[1][2] = _v1.z;
    
    m_data[2][0] = _v2.x;
    m_data[2][1] = _v2.y;
    m_data[2][2] = _v2.z;
}

Matrix3x3::Matrix3x3(const Matrix3x3& _m) { setData(_m.getData()); }

Matrix3x3::~Matrix3x3() {
}

Matrix3x3 Matrix3x3::transpose(const Matrix3x3& _m) {
    return Matrix3x3(_m.m_data[0][0], _m.m_data[1][0], _m.m_data[2][0],
                     _m.m_data[0][1], _m.m_data[1][1], _m.m_data[2][1],
                     _m.m_data[0][2], _m.m_data[1][2], _m.m_data[2][2]);
}

Matrix3x3 Matrix3x3::rotationX(const double _radians) {
    return Matrix3x3(1.0, 0.0, 0.0,
                     0.0, cos(_radians), -sin(_radians),
                     0.0, sin(_radians), cos(_radians));
}

Matrix3x3 Matrix3x3::rotationY(const double _radians) {
    return Matrix3x3(cos(_radians), 0.0, sin(_radians),
                     0.0, 1.0, 0.0,
                     -sin(_radians), 0.0, cos(_radians));
}

Matrix3x3 Matrix3x3::rotationZ(const double _radians) {
    return Matrix3x3(cos(_radians), -sin(_radians), 0.0,
                     sin(_radians), cos(_radians), 0.0,
                     0.0, 0.0, 1.0);
}

Matrix3x3 Matrix3x3::rotation(const double _radians, const Vector3& _axis) {
    const double c = cos(_radians);
    const double s = sin(_radians);
    const double t = 1.0 - c;
    
    return Matrix3x3(t * _axis.x * _axis.x + c, t * _axis.x * _axis.y - s * _axis.z, t * _axis.x * _axis.z + s * _axis.y,
                     t * _axis.x * _axis.y + s * _axis.z, t * _axis.y * _axis.y + c, t * _axis.y * _axis.z - s * _axis.x,
                     t * _axis.x * _axis.z - s * _axis.y, t * _axis.y * _axis.z + s * _axis.x, t * _axis.z * _axis.z + c);
}

void Matrix3x3::transpose() { *this = Matrix3x3::transpose(*this); }
void Matrix3x3::rotateX(const double _radians) { *this *= Matrix3x3::rotationX(_radians); }
void Matrix3x3::rotateY(const double _radians) { *this *= Matrix3x3::rotationY(_radians); }
void Matrix3x3::rotateZ(const double _radians) { *this *= Matrix3x3::rotationZ(_radians); }
void Matrix3x3::setData(const double* _m) { memcpy(m_data, _m, 3 * 3 * sizeof(double)); }

Vector3 Matrix3x3::getRow(size_t _i) const { return Vector3(m_data[_i][0], m_data[_i][1], m_data[_i][2]); }
Vector3 Matrix3x3::getColumn(size_t _i) const { return Vector3(m_data[0][_i], m_data[1][_i], m_data[2][_i]); }

Vector3 Matrix3x3::operator*(const Vector3& _v) const {
    return Vector3(
        m_data[0][0] * _v.x + m_data[0][1] * _v.y + m_data[0][2] * _v.z,
        m_data[1][0] * _v.x + m_data[1][1] * _v.y + m_data[1][2] * _v.z,
        m_data[2][0] * _v.x + m_data[2][1] * _v.y + m_data[2][2] * _v.z
    );
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& _m) const {
    Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.m_data[i][j] = m_data[i][0] * _m.m_data[0][j] +
                                   m_data[i][1] * _m.m_data[1][j] +
                                   m_data[i][2] * _m.m_data[2][j];
        }
    }
    return result;
}

Matrix3x3& Matrix3x3::operator=(const Matrix3x3& _m) {
    memcpy(m_data, _m.m_data, 3 * 3 * sizeof(double));
    return *this;
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& _m) {
    *this = *this * _m;
    return *this;
}

Matrix3x3& Matrix3x3::operator*=(const double _s) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m_data[i][j] *= _s;
        }
    }
    return *this;
}

Matrix3x3& Matrix3x3::operator/=(const double _s) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m_data[i][j] /= _s;
        }
    }
    return *this;
}

Matrix3x3 Matrix3x3::operator*(const double _s) const {
    Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.m_data[i][j] = m_data[i][j] * _s;
        }
    }
    return result;
}

Matrix3x3 Matrix3x3::operator/(const double _s) const {
    Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.m_data[i][j] = m_data[i][j] / _s;
        }
    }
    return result;
}