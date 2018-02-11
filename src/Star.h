#pragma once

#include "coordinates/Equatorial.h"
#include "coordinates/Horizontal.h"

#include "Observer.h"

class Star {
public:

    static const int TOTAL;

    Star();
    Star( int _id );
    Star( double _ra, double _dec, double _mag );
    virtual ~Star();

    virtual int     getId() const { return m_id; };
    virtual double  getMagnitud() const { return m_mag; };
    
    virtual Equatorial getEquatorial() const { return m_equatorial; }
    virtual Horizontal getHorizontal() const { return m_horizontal; }

    virtual void    compute( Observer& _obs );
    
protected:
    Equatorial  m_equatorial;
    Horizontal  m_horizontal;
    
    double      m_mag;
    int         m_id;
};

inline std::ostream& operator<<(std::ostream& strm, const Star& s) {
    strm << std::setprecision(3);
    strm << "id: " << std::setw(8) << s.getId() << ", ";
    strm << ", mag: " << std::setw(8) << s.getMagnitud() << ", ";
    strm << s.getEquatorial() << ", ";
    strm << s.getHorizontal();
    return strm;
}
