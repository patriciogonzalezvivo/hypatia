#include "Equatorial.h"

#include <math.h>

Equatorial::Equatorial() {
}

Equatorial::Equatorial(const Vector& _parent) {
    m_phi = atan2(_parent.y, _parent.x);
    m_theta = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
}

Equatorial::Equatorial(const double _ra, const double _dec, ANGLE_UNIT _type) {
    if ( _type == RADS ) {
        m_phi = _ra;
        m_theta = _dec;
    }
    else {
        m_phi = MathOps::toRadians( _ra );
        m_theta = MathOps::toRadians( _dec );
    }
}

Equatorial::~Equatorial() {
}

double Equatorial::getRightAscension(ANGLE_UNIT _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_phi );
    }
    else {
        return m_phi;
    }
}

double Equatorial::getDeclination(ANGLE_UNIT _type) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_theta );
    }
    else {
        return m_theta;
    }
}

Vector Equatorial::getVector() const {
    return Vector(*this);
}

double Equatorial::getAngularDistance(const Equatorial& _equ, ANGLE_UNIT _type) const {
    // Astronomical Formulae for Calculators by J. Meeus
    // Pag 109

    double ra1 = m_phi;
    double dec1 = m_theta;

    double ra2 = _equ.getRightAscension(RADS);
    double dec2 = _equ.getDeclination(RADS);

    double raD = ra1 - ra2;

    double ang_dist = acos( sin(dec1) * sin(dec2) + cos(dec1) * cos(dec2) * cos(raD) );

    // if the angular distance is close to 0 o 180 deg by 10 sec use approximate formula
    if (ang_dist < 0.00290888) {
        double decD = dec1 - dec2;
        double a = raD * cos(dec1);

        ang_dist = sqrt(a * a + decD * decD);
    }

    if (_type == RADS) {
        return ang_dist;
    }
    else {
        return MathOps::toDegrees(ang_dist);
    }
}