#include "EqPoint.h"

#include "AstroOps.h"

#include <math.h>
#include <sstream>
#include <iomanip>

EqPoint::EqPoint() : m_ra(0.0), m_dec(0.0) {
}

EqPoint::EqPoint(const Vector& _parent) {
    m_ra = atan2(_parent.y, _parent.x);
    m_dec = atan2(_parent.z, sqrt(_parent.x * _parent.x + _parent.y * _parent.y));
}

EqPoint::EqPoint( double _ra, double _dec, bool _radians ) {
    if (_radians) {
        m_ra = _ra;
        m_dec = _dec;
    }
    else {
        m_ra = MathOps::toRadians( _ra );
        m_dec = MathOps::toRadians( _dec );
    }
}

EqPoint::~EqPoint() {
}

Vector EqPoint::getVector() const {
    Vector v;
    v.setPolar(m_ra, m_dec, true);
    return v;
}
