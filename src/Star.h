#pragma once

#include "coordinates/Equatorial.h"
#include "coordinates/Horizontal.h"

#include "Observer.h"

#define USE_HIPPACOS

class Star {
public:

    static const int TOTAL;

    Star();
    Star( int _id );
    Star( double _ra, double _dec, double _mag );
    virtual ~Star();

    virtual int         getId() const { return m_id; };

    virtual int         getHIP() const;
    virtual char*       getName() const;

    virtual double      getMagnitud() const;
    
    // Positioning
    virtual Equatorial  getEquatorial() const { return m_equatorial; }
    virtual Horizontal  getHorizontal() const { return m_horizontal; }
    virtual double      getHourAngle(ANGLE_UNIT _type) const;
    virtual bool        haveHorizontal() const { return m_bHorizontal; }

    #ifdef USE_HIPPACOS
    virtual double      getAbsMagnitud() const;
    virtual double      getParalax() const;
    virtual double      getTemperature() const;
    virtual double      getVB() const;
    #endif

    virtual void    compute( Observer& _obs );
    
protected:

    // Position
    Equatorial  m_equatorial;
    Horizontal  m_horizontal;
    double      m_ha;
    
    // Magnitud
    double      m_mag;      // aparent

    // ID
    int         m_id;

    bool        m_bHorizontal;
};

inline std::ostream& operator<<(std::ostream& strm, const Star& s) {
    strm << std::setprecision(3);
    strm << "id: " << std::setw(8) << s.getId() << ", ";
    strm << ", mag: " << std::setw(8) << s.getMagnitud() << ", ";
    strm << s.getEquatorial() << ", ";
    strm << s.getHorizontal();
    return strm;
}
