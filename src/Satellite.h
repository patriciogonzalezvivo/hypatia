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
    
    virtual ECI         getECI() const { return m_eci; };
    
    virtual Geodetic    getGeodetic() const { return m_geodetic; };
    virtual Vector      getEquatorialVector(UNIT_TYPE _type) const { return m_geodetic.getVector(_type); }
    
    virtual char*       getName() const;
    
    virtual void        compute( Observer& _obs );
    
protected:
    SGP4        m_sgp4;
    ECI         m_eci;
    Geodetic    m_geodetic;
    
    std::string m_name;
};

