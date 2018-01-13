#pragma once

#include "EqPoint.h"

class Star : public EqPoint {
public:
    Star();
    Star( int _id );
    Star( double _ra, double _dec, double _mag );

    virtual int getId() const { return m_id; };

protected:
    double m_mag;
    int m_id;
};
