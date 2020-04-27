#pragma once

#include "Body.h"
#include "models/SGP4.h"

class Satellite : public Body {
public:
    Satellite();
    Satellite(const TLE& _tle);
    virtual ~Satellite();

    virtual void        setTLE(const TLE& _tle);
    
    virtual Orbit       getOrbit() const { return m_sgp4.getOrbit(); };
    virtual double      getPeriod(TIME_UNIT _unit) const;
    
    virtual ECI         getECI() const { return m_eci; };
    virtual Geodetic    getGeodetic() const;
    virtual Vector3     getEquatorialVector(DISTANCE_UNIT _type) const { return getGeodetic().getVector(_type); }
    
    virtual char*       getName() const;
    
    virtual void        compute( Observer& _obs );
    
protected:
    SGP4        m_sgp4;
    ECI         m_eci;
    
    std::string m_name;
};

