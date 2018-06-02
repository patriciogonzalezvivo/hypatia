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

/**
 * @param[in] jd the date to be used for this position
 * @param[in] position the position
 * @param[in] velocity the velocity
 */
ECI::ECI(double _jd, const Vector &_pos, const Vector &_vel): m_position(_pos), m_velocity(_vel), m_jd(_jd) {
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
