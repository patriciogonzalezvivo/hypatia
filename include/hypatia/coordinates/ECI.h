//
//  Eci.hpp
//
//  Created by Patricio González Vivo on 5/31/18.
//
#pragma once

#include "../primitives/Vector3.h"

/**
 * @brief Stores an Earth-centered inertial position for a particular time.
 */
class ECI {
public:
    ECI();
    ECI(double _jd, const Vector3 &_pos, const Vector3 &_vel = Vector3(), DISTANCE_UNIT _type = KM);
    virtual ~ECI();

    virtual double      getJD() const { return m_jd; };
    virtual Vector3     getPosition(DISTANCE_UNIT _type) const;
    virtual Vector3     getVelocity(DISTANCE_UNIT _type) const;

private:
    Vector3     m_position; // km
    Vector3     m_velocity; // km/s
    double      m_jd;
};

inline std::ostream& operator<<(std::ostream& strm, const ECI& e) {
    strm << std::setprecision(3);
    strm << "JD: " << std::setw(12) << e.getJD();
    strm << ", Pos: " << e.getPosition(KM);
    strm << ", Vel: " << e.getVelocity(KM);
    return strm;
};

