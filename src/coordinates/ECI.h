//
//  Eci.hpp
//
//  Created by Patricio González Vivo on 5/31/18.
//
#pragma once

#include "../primitives/Vector.h"

/**
 * @brief Stores an Earth-centered inertial position for a particular time.
 */
class ECI {
public:
    ECI();
    ECI(double _jd, const Vector &_pos, const Vector &_vel = Vector(), UNIT_TYPE _type = KM);
    virtual ~ECI();

    virtual double      getJD() const { return m_jd; };
    virtual Vector      getPosition(UNIT_TYPE _type) const;
    virtual Vector      getVelocity(UNIT_TYPE _type) const;

private:
    Vector      m_position; // km
    Vector      m_velocity; // km/s
    double      m_jd;
};

inline std::ostream& operator<<(std::ostream& strm, const ECI& e) {
    strm << std::setprecision(3);
    strm << "JD: " << std::setw(12) << e.getJD();
    strm << ", Pos: " << e.getPosition(KM);
    strm << ", Vel: " << e.getVelocity(KM);
    return strm;
};

