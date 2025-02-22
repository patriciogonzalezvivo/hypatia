#include "hypatia/coordinates/PrecessionMatrix.h"
#include "hypatia/MathOps.h"
#include "hypatia/CoordOps.h"

#include <math.h>
#include <cstring>

PrecessionMatrix::PrecessionMatrix() : Matrix3x3() {
    m_fromYear = 2000.0;
    m_toYear = 0.0;
    m_backward = true;
}

PrecessionMatrix::PrecessionMatrix(double _toYear, double _fromYear) : Matrix3x3() {
    m_fromYear = _fromYear;
    m_toYear = _toYear;
    m_backward = true;
    
    if ( _fromYear > _toYear ) {
        m_backward = true;
    }

    compute();
}

PrecessionMatrix::~PrecessionMatrix() {

}

//---------------------------------------------------------------------------- precess

void PrecessionMatrix::compute() {
    const double t1 = (m_fromYear - 2000.) / 100.;
    const double t2 = (m_toYear - 2000.) / 100.;
    const double obliquity1 = CoordOps::meanObliquity(t1);
    const double obliquity2 = CoordOps::meanObliquity(t2);

    Matrix3x3 m; 

    if (m_fromYear == m_toYear)
        m = Matrix3x3(); // identity
    
    else if (m_fromYear == 2000.0)
        m = CoordOps::eclipticPrecessionFromJ2000(m_toYear);

    else {
        m = CoordOps::eclipticPrecessionFromJ2000(m_fromYear);
        m.transpose();
        if( m_toYear != 2000.)
            m *= CoordOps::eclipticPrecessionFromJ2000(m_toYear);
    }

    m = Matrix3x3::rotationX(obliquity1) * m;
    m *= CoordOps::eclipticToEquatorial(obliquity2);

    double* matrix = m.getData();
    setData(matrix);
}

Vector3 PrecessionMatrix::precess(const Vector3& _v) const { 
    return Vector3(
        m_data[0][0] * _v.x + m_data[1][0] * _v.y + m_data[2][0] * _v.z,
        m_data[0][1] * _v.x + m_data[1][1] * _v.y + m_data[2][1] * _v.z,
        m_data[0][2] * _v.x + m_data[1][2] * _v.y + m_data[2][2] * _v.z
    );
}

Vector3 PrecessionMatrix::deprecess(const Vector3& _v) const {
    return Vector3(
        m_data[0][0] * _v.x + m_data[0][1] * _v.y + m_data[0][2] * _v.z,
        m_data[1][0] * _v.x + m_data[1][1] * _v.y + m_data[1][2] * _v.z,
        m_data[2][0] * _v.x + m_data[2][1] * _v.y + m_data[2][2] * _v.z
    );
}