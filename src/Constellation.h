#pragma once

#include <string>
#include <vector>

#include "TimeOps.h"
#include "EqPoint.h"

class Constellation : public EqPoint {
public:

    static const int TOTAL;

    Constellation();
    Constellation( int _id );
    Constellation( char * _abbr );
    Constellation( std::string _abbr );
    Constellation( EqPoint &_point );
    Constellation( double _ra, double _dec );

    void setId( int _id );
    int getId() const { return m_id; };

    std::string getName();
    std::string getAbbreviation();

    std::vector<int> getStarIndices() const { return m_sIndices; };

protected:
    std::vector<int> m_sIndices;

    int m_id;
};
