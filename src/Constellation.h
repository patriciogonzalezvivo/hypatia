#pragma once

#include <string>

#include "Body.h"
#include "Lunar.h"
#include "TimeOps.h"

class Constellation {
public:
    Constellation();
    Constellation( int _id );
    Constellation( Body &_body, double _epoch = TimeOps::J2000 );
    Constellation( Lunar &_luna, double _epoch = TimeOps::J2000 );
    Constellation( double _ra, double _dec, double _epoch = TimeOps::J2000 );

    int getId() const { return m_id; };
    std::string getName();
    std::string getAbbreviation();

protected:
    int m_id;
};
