#include "hypatia/coordinates/PrecessionMatrix.h"
#include "hypatia/MathOps.h"
#include "hypatia/CoordOps.h"

#include <math.h>
#include <string.h>

PrecessionMatrix::PrecessionMatrix() : Matrix3x3() {
    m_fromYear = 0;
    m_toYear = 0;
    m_backward = false;
}

PrecessionMatrix::PrecessionMatrix(double _fromYear, double _toYear) : Matrix3x3() {
    m_fromYear = _fromYear;
    m_toYear = _toYear;
    m_backward = false;
    
    if ( _fromYear < _toYear ) {
        m_backward = true;
    }

    compute();
}

PrecessionMatrix::~PrecessionMatrix() {

}

//---------------------------------------------------------------------------- precess

void set_identity_matrix(double *matrix) {
    for(int i = 0; i < 9; i++)
       matrix[i] = ((i & 3) ? 0. : 1.);
}

#define SWAP( A, B, TEMP)  { TEMP = A;  A = B;  B = TEMP; }
void invert_orthonormal_matrix(double *matrix) {
   double temp;

   SWAP( matrix[1], matrix[3], temp);
   SWAP( matrix[2], matrix[6], temp);
   SWAP( matrix[5], matrix[7], temp);
}
 
void pre_spin_matrix( double *v1, double *v2, const double angle) {
    const double sin_ang = sin( angle);
    const double cos_ang = cos( angle);
    int i;
 
     for( i = 3; i; i--) {
        const double tval = *v1 * cos_ang - *v2 * sin_ang;

        *v2 = *v2 * cos_ang + *v1 * sin_ang;
        *v1 = tval;
        v1 += 3;
        v2 += 3;
    }
 }
 
 void spin_matrix( double *v1, double *v2, const double angle) {
    const double sin_ang = sin( angle);
    const double cos_ang = cos( angle);
    int i;
 
    for( i = 3; i; i--) {
        const double tval = *v1 * cos_ang - *v2 * sin_ang;

        *v2 = *v2 * cos_ang + *v1 * sin_ang;
        *v1++ = tval;
        v2++;
    }
}

int setup_ecliptic_precession_from_j2000(double *matrix, const double _year) {
   const double t = (_year - 2000.) / 100.;
   const double S2R = (MathOps::PI / 180.) / 3600.;   /* converts arcSeconds to Radians */
   const double eta = t * (47.0029 * S2R + (-.03302 * S2R + 6.e-5 * S2R * t) * t);
   const double pie = 174.876384 * MathOps::PI / 180. - t * (869.8089 * S2R - .03536 * S2R * t);
   const double p = t * (5029.0966 * S2R + (1.11113 * S2R - 6.e-5 * S2R * t) * t);

   set_identity_matrix(matrix);
   spin_matrix( matrix, matrix + 3, -pie);
   spin_matrix( matrix + 3, matrix + 6, -eta);
   spin_matrix( matrix + 3, matrix, -p);
   spin_matrix( matrix, matrix + 3, pie);
   return( 0);
}

#define SEMIRANDOM_GARBAGE1  314.8145501e+12
#define SEMIRANDOM_GARBAGE2  -9.19001473e-08
int setup_ecliptic_precession(double *matrix, const double year_from, const double year_to) {
    int rval;
    /* It's pretty common to precess a few zillion data points.  So   */
    /* it helps to cache the most recently computed precession matrix */
    /* so that repeated calls don't result in repeated computation.   */
    static double prev_year_from = SEMIRANDOM_GARBAGE1;
    static double prev_year_to   = SEMIRANDOM_GARBAGE2;
    static double prev_matrix[9];

    if( fabs( year_from - year_to) < 1.e-5) {   /* dates sensibly equal; */
        set_identity_matrix( matrix);           /* avoid pointless math */
        return( 0);
    }

    if( year_from == prev_year_from && year_to == prev_year_to) {
        memcpy( matrix, prev_matrix, 9 * sizeof( double));
        return( 0);
    }
    /* Similarly,  it's common to precess first  */
    /* in one direction,  then the other:        */
    if( year_from == prev_year_to && year_to == prev_year_from) {
        memcpy( matrix, prev_matrix, 9 * sizeof( double));
        invert_orthonormal_matrix( matrix);
        return( 0);
    }

    /* OK,  this is definitely something new: */
    if( year_from == 2000.)
        rval = setup_ecliptic_precession_from_j2000( matrix, year_to);
    else {
        rval = setup_ecliptic_precession_from_j2000( matrix, year_from);
        invert_orthonormal_matrix( matrix);
        if( year_to != 2000.) {
        double product[9], tmatrix[9];
        int i, j;

        setup_ecliptic_precession_from_j2000( tmatrix, year_to);
        for( i = 0; i < 3; i++)
            for( j = 0; j < 3; j++)
            product[j + i * 3] = matrix[i * 3    ] * tmatrix[j]
                            + matrix[i * 3 + 1] * tmatrix[j + 3]
                            + matrix[i * 3 + 2] * tmatrix[j + 6];
            memcpy( matrix, product, 9 * sizeof( double));
        }
    }

    /* Store matrix for likely subsequent use: */
    memcpy( prev_matrix, matrix, 9 * sizeof( double));
    prev_year_from = year_from;
    prev_year_to = year_to;
    return( rval);
}

int precess_vector(const double *matrix, const double *v1, double *v2) {
    int i = 3;
    double temp_v[3];

    if( v1 == v2) {        /* allow precession in place */
        memcpy( temp_v, v1, 3 * sizeof( double));
        v1 = temp_v;
    }
    while( i--) {
        *v2++ = matrix[0] * v1[0] + matrix[1] * v1[1] + matrix[2] * v1[2];
        matrix += 3;
    }
    return( 0);
}


int deprecess_vector( const double *matrix, const double *v1, double *v2) {
    int i = 3;
    double temp_v[3];

    if( v1 == v2) {       /* allow precession in place */
        memcpy( temp_v, v1, 3 * sizeof( double));
        v1 = temp_v;
    }
    while( i--) {
        *v2++ = matrix[0] * v1[0] + matrix[3] * v1[1] + matrix[6] * v1[2];
        matrix++;
    }
    return( 0);
}


void PrecessionMatrix::compute() {
    const double obliquity1 = CoordOps::meanObliquity( (m_fromYear - 2000.) / 100.);
    const double obliquity2 = CoordOps::meanObliquity( (m_toYear - 2000.) / 100.);

    double *matrix = new double[9];
    setup_ecliptic_precession( matrix, m_fromYear, m_toYear);
    pre_spin_matrix( matrix + 1, matrix + 2, obliquity1);
    spin_matrix( matrix + 3, matrix + 6, obliquity2);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m_data[i][j] = matrix[i * 3 + j];
        }
    }
}

Vector3 PrecessionMatrix::precess(const Vector3& _v) const { 
    return Vector3(
        m_data[0][0] * _v.x + m_data[0][1] * _v.y + m_data[0][2] * _v.z,
        m_data[1][0] * _v.x + m_data[1][1] * _v.y + m_data[1][2] * _v.z,
        m_data[2][0] * _v.x + m_data[2][1] * _v.y + m_data[2][2] * _v.z
    );
}

Vector3 PrecessionMatrix::deprecess(const Vector3& _v) const {
    return Vector3(
        m_data[0][0] * _v.x + m_data[1][0] * _v.y + m_data[2][0] * _v.z,
        m_data[0][1] * _v.x + m_data[1][1] * _v.y + m_data[2][1] * _v.z,
        m_data[0][2] * _v.x + m_data[1][2] * _v.y + m_data[2][2] * _v.z
    );
}