//
//  Eci.cpp
//  Solar
//
//  Created by Patricio González Vivo on 5/31/18.
//

#include "Eci.h"

#include "../TimeOps.h"
#include "../AstroOps.h"

#include <math.h>

/**
 * @param[in] dt the date to be used for this position
 * @param[in] position the position
 * @param[in] velocity the velocity
 */
Eci::Eci(const DateTime &_dt, const Vector &_pos, const Vector &_vel) : m_dt(_dt), m_position(_pos), m_velocity(_vel) {
}

Eci::~Eci(){    
}

/**
 * Equality operator
 * @param dt the date to compare
 * @returns true if the object matches
 */
bool Eci::operator==(const DateTime& _dt) const {
    return m_dt == _dt;
}

/**
 * Inequality operator
 * @param dt the date to compare
 * @returns true if the object doesn't match
 */
bool Eci::operator!=(const DateTime& _dt) const {
    return m_dt != _dt;
}

/**
 * @returns the position
 */
Vector Eci::getPosition(UNIT_TYPE _type) const {
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
Vector Eci::getVelocity(UNIT_TYPE _type) const {
    if (_type == KM) {
        return m_velocity;
    }
    else {
        return m_velocity * AstroOps::KM_TO_AU;
    }
}

/**
 * @returns the date
 */
DateTime Eci::getDateTime() const{
    return m_dt;
}
