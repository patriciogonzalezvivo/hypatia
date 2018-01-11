#pragma once

#include <string>
#include <vector>

#include "Body.h"
#include "Luna.h"
#include "TimeOps.h"

class Constellation {
public:

    static const int TOTAL;

    Constellation();
    Constellation( int _id );
    Constellation( Body &_body);
    Constellation( Luna &_luna);
    Constellation( double _ra, double _dec);

    int getId() const { return m_id; };
    std::string getName();
    std::string getAbbreviation();

    std::vector<int> getStarsIds() const { return m_starsIds; };

    // Ecuatorial Centroid (Geocentric)
    double getDeclination() const { return MathOps::toDegrees( m_dec ); }
    double getDeclinationRadians() const { return m_dec; }
    double getRightAscension() const { return MathOps::toDegrees( m_ra ); }
    double getRightAscensionRadians() const { return m_ra; }

    // Horizontal (Topocentric)
    double getAzimuth() const { return MathOps::toDegrees( m_az ); }
    double getAzimuthRadians() const { return m_az; }
    double getAltitud() const { return MathOps::toDegrees( m_alt ); }
    double getAltitudRadians() const { return m_alt; }

    void update(Observer &_obs);

protected:
    std::vector<int> m_starsIds;

    // Centroid
    double m_ra;
    double m_dec;

    double m_az;
    double m_alt;

    int m_id;
};
