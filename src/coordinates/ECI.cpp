//
//  ECI.cpp
//  Solar
//
//  Created by Patricio González Vivo on 5/31/18.
//

#include "ECI.h"

#include "../TimeOps.h"
#include "../AstroOps.h"

#include <math.h>

ECI::ECI() {
    
}
/**
 * @param[in] jd the date to be used for this position
 * @param[in] position the position
 * @param[in] velocity the velocity
 */
ECI::ECI(double _jd, const Vector &_pos, const Vector &_vel, UNIT_TYPE _type) {
    m_jd = _jd;
    if (_type == KM) {
        m_position = _pos;
        m_velocity = _vel;
    }
    else {
        m_position = _pos * AstroOps::AU_TO_KM;
        m_velocity = _vel * AstroOps::AU_TO_KM;;
    }
}

ECI::~ECI(){    
}

/**
 * @returns the position
 */
Vector ECI::getPosition(UNIT_TYPE _type) const {
    if (_type == KM) {
        return m_position;
    }
    else {
        return m_position * AstroOps::KM_TO_AU;
    }
}

/**
 * @returns the velocity
 */
Vector ECI::getVelocity(UNIT_TYPE _type) const {
    if (_type == KM) {
        return m_velocity;
    }
    else {
        return m_velocity * AstroOps::KM_TO_AU;
    }
}
