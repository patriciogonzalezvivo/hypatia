//
//  Matrix3x3.cpp
//
//  Created by Patricio González Vivo on 2/21/25.
//

#include "hypatia/primitives/Matrix3x3.h"

#include <math.h>

Matrix3x3::Matrix3x3() {
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

Matrix3x3::Matrix3x3(const Matrix3x3& _m) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m_data[i][j] = _m.m_data[i][j];
        }
    }
}

Matrix3x3::~Matrix3x3() {
}

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

