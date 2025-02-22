#pragma once

#include "../primitives/Matrix3x3.h"

class PrecessionMatrix : public Matrix3x3 {
public:
    PrecessionMatrix();
    PrecessionMatrix(double _toYear, double _fromYear = 2000.0);
    virtual ~PrecessionMatrix();

    double  fromYear() const { return m_fromYear; }
    double  toYear() const { return m_toYear; }
    bool    isBackward() const { return m_backward; }

    Vector3 precess(const Vector3& _v) const;
    Vector3 deprecess(const Vector3& _v) const;

protected:
    void    compute();

    double  m_fromYear;
    double  m_toYear;
    bool    m_backward;
};