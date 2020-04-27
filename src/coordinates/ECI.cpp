//
//  ECI.cpp
//  Solar
//
//  Created by Patricio González Vivo on 5/31/18.
//

#include "hypatia/coordinates/ECI.h"

#include "hypatia/TimeOps.h"
#include "hypatia/CoordOps.h"

#include <math.h>

ECI::ECI() {
    
}
/**
 * @param[in] jd the date to be used for this position
 * @param[in] position the position
 * @param[in] velocity the velocity
 */
ECI::ECI(double _jd, const Vector3 &_pos, const Vector3 &_vel, DISTANCE_UNIT _type) {
    m_jd = _jd;

    if (_type == KM) {
        m_position = _pos;
        m_velocity = _vel;
    }
    else if (_type == AU) {
        m_position = _pos * CoordOps::AU_TO_KM;
        m_velocity = _vel * CoordOps::AU_TO_KM;
    }
    else if (_type == LY) {
        m_position = _pos * CoordOps::LY_TO_AU * CoordOps::AU_TO_KM;
        m_velocity = _vel * CoordOps::LY_TO_AU * CoordOps::AU_TO_KM;
    }
    else if (_type == PC) {
        m_position = _pos * CoordOps::PC_TO_LY * CoordOps::LY_TO_AU * CoordOps::AU_TO_KM;
        m_velocity = _vel * CoordOps::PC_TO_LY * CoordOps::LY_TO_AU * CoordOps::AU_TO_KM;
    }
}

ECI::~ECI(){    
}

/**
 * @returns the position
 */
Vector3 ECI::getPosition(DISTANCE_UNIT _type) const {
    if (_type == KM) {
        return m_position;
    }
    else if (_type == AU) {
        return getPosition(KM) * CoordOps::KM_TO_AU;
    }
    else if (_type == LY) {
        return getPosition(AU) * CoordOps::AU_TO_LY;
    }
    else if (_type == PC) {
        return getPosition(LY) * CoordOps::LY_TO_PC;
    }
    else {
        return m_position;
    }
}

/**
 * @returns the velocity
 */
Vector3 ECI::getVelocity(DISTANCE_UNIT _type) const {
    if (_type == KM) {
        return m_velocity;
    }
    else if (_type == AU) {
        return getVelocity(KM) * CoordOps::KM_TO_AU;
    }
    else if (_type == LY) {
        return getVelocity(AU) * CoordOps::AU_TO_LY;
    }
    else if (_type == PC) {
        return getVelocity(LY) * CoordOps::LY_TO_PC;
    }
    else {
        return m_velocity;
    }
}
