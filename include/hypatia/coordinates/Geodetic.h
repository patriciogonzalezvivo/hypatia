#pragma once

#include "../primitives/Vector3.h"
#include "UTM.h"

class Geodetic : public Polar {
public:
    Geodetic();
    Geodetic( const UTM& _coord );
    Geodetic( double _lng, double _lat, double _alt, ANGLE_UNIT _a_type, DISTANCE_UNIT _alt_unit );
    virtual ~Geodetic();
    
    virtual double  getLongitude(ANGLE_UNIT _type) const;
    virtual double  getLatitude(ANGLE_UNIT _type) const;
    virtual double  getAltitude(DISTANCE_UNIT _type) const;
    virtual double  getRadius(DISTANCE_UNIT _type) const;
    
    virtual Vector3 getVector(DISTANCE_UNIT _type) const;
    virtual UTM     getUTM() const;
    
protected:
    double m_alt;   // always stored on KM
};

inline std::ostream& operator<<(std::ostream& strm, const Geodetic& p) {
    strm << std::setprecision(3);
    strm << "lng: " << std::setw(12) << MathOps::formatAngle(p.getLongitude(DEGS), DEGS, D_M_Ss);
    strm << ", lat: " << std::setw(12) << MathOps::formatAngle(p.getLatitude(DEGS), DEGS, D_M_Ss);
    strm << ", alt: " << std::setw(8) << p.getAltitude(KM);

    return strm;
};
