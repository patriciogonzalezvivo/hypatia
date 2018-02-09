#pragma once

#include "EqPoint.h"
#include "HorPoint.h"

#include "Observer.h"

class Star {
public:

    static const int TOTAL;

    Star();
    Star( int _id );
    Star( double _ra, double _dec, double _mag );

    virtual int     getId() const { return m_id; };
    virtual double  getMagnitud() const { return m_mag; };
    
    virtual EqPoint getEquatorial() const { return m_equatorial; }
    virtual HorPoint getHorizontal() const { return m_horizontal; }

    virtual void    compute( Observer& _obs );
    
protected:
    EqPoint m_equatorial;
    HorPoint m_horizontal;
    
    double m_mag;
    int m_id;
};

inline std::ostream& operator<<(std::ostream& strm, const Star& s) {
    strm << std::setprecision(3);
    strm << "id: " << std::setw(8) << s.getId() << ", ";
    strm << ", mag: " << std::setw(8) << s.getMagnitud() << ", ";
    strm << s.getEquatorial() << ", ";
    strm << s.getHorizontal();
}
