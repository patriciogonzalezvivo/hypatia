#pragma once

#include "EqPoint.h"

#include <vector>

class Constellation {
public:

    static const int TOTAL;

    Constellation();
    Constellation( int _id );
    Constellation( char * _abbr );
    Constellation( const EqPoint &_point );
    Constellation( double _ra, double _dec );

    void setId( int _id );
    int getId() const { return m_id; };

    char * getName() const;
    char * getAbbreviation() const;

    std::vector<int> getStarIndices() const { return m_sIndices; };
    std::vector<EqPoint> getBoundary() const { return m_boundary; };

protected:
    int                     m_id;
    std::vector<int>        m_sIndices;
    std::vector<EqPoint>    m_boundary;
};

inline std::ostream& operator<<(std::ostream& strm, const Constellation& c) {
    strm << c.getName() << " (" << c.getAbbreviation() << ") ";
}
