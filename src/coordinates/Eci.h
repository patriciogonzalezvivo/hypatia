//
//  Eci.hpp
//
//  Created by Patricio González Vivo on 5/31/18.
//
#pragma once

#include "../primitives/Vector.h"
#include "../primitives/DateTime.h"

/**
 * @brief Stores an Earth-centered inertial position for a particular time.
 */
class Eci {
public:
    Eci(const DateTime& _dt, const Vector &_pos, const Vector &_vel = Vector());
    virtual ~Eci();

    virtual bool operator==(const DateTime& _dt) const;
    virtual bool operator!=(const DateTime& _dt) const;
    
    virtual Vector      getPosition(UNIT_TYPE _type) const;
    virtual Vector      getVelocity(UNIT_TYPE _type) const;
    virtual DateTime    getDateTime() const;
    
private:
    DateTime    m_dt;
    Vector      m_position; // km
    Vector      m_velocity; // km/s
};

inline std::ostream& operator<<(std::ostream& strm, const Eci& e) {
    strm << std::setprecision(3);
    strm << "Pos: " << e.getPosition(KM);
    strm << ", Vel: " << e.getVelocity(KM);
    strm << ", DT: " << e.getDateTime();
    return strm;
};

