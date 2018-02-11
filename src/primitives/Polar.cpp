//
//  Polar.cpp
//  Solar
//
//  Created by Patricio González Vivo on 2/11/18.
//

#include "Polar.h"

#include "../MathOps.h"
#include <math.h>

Polar::Polar() : m_phi(0.0),  m_theta(0.0) {
}

Polar::Polar(const double _phi, const double _theta, bool _radians) {
    if (_radians) {
        m_phi = _phi;
        m_theta = _theta;
    }
    else {
        m_phi = MathOps::toRadians( _phi );
        m_theta = MathOps::toRadians( _theta );
    }
}

Polar::~Polar() {
}

double& Polar::operator[]( int _n ) {
    return getPtr()[_n];
}

double  Polar::operator[]( int _n ) const {
    return getPtr()[_n];
}

double * Polar::getPtr() {
    return (double*)&m_phi;
}

const double * Polar::getPtr() const {
    return (const double *)&m_phi;
}
