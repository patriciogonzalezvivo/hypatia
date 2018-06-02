/*****************************************************************************\
 * Orbit
 *
 * Created by Patricio González Vivo on 5/31/18.
 \*****************************************************************************/

#pragma once

#include "TLE.h"

class Orbit {
public:
    Orbit();
    Orbit(const TLE& tle);
    
    double      getEccentricity() const { return m_eccentricity; }
    
    double      getMeanAnomaly(ANGLE_TYPE _type) const;
    double      getAscendingNode(ANGLE_TYPE _type) const;
    double      getArgumentPeriapsis(ANGLE_TYPE _type) const;
    
    double      getInclination(ANGLE_TYPE _type) const;
    double      getMeanMotion() const { return m_meanMotion; }
    
    double      getBStar() const { return m_bstar; }
    double      getSemiMajorAxis() const { return m_semiMajorAxis; }
    double      getRecoveredMeanMotion() const { return m_recoveredMeanMotion; }
    double      getPeriapsis() const { return m_periapsis; }
    double      getPeriod() const { return m_period; }

    DateTime    getEpoch() const { return m_epoch; }
    
private:
    //  Shape
    double      m_eccentricity;         // shape of the ellipse, describing how much it is elongated compared to a circle
    double      m_semiMajorAxis;        // the sum of the periapsis and apoapsis distances divided by two. For circular orbits,
                                        // the semimajor axis is the distance between the centers of the bodies, not the distance
                                        // of the bodies from the center of mass.
    
    //  Plane Orientation
    double      m_inclination;          // vertical tilt of the ellipse with respect to the reference plane, measured at the ascending
                                        // node (where the orbit passes upward through the reference plane, the green angle i in the diagram). Tilt angle is measured perpendicular to line of intersection between orbital plane and reference plane. Any three points on an ellipse will define the ellipse orbital plane. The plane and the ellipse are both two-dimensional objects defined in three-dimensional space.
    double      m_ascendingNode;
    
    //
    double      m_argumentPeriapsis;
    double      m_periapsis;
    
    // Optional
    double      m_meanAnomoly;
    double      m_meanMotion;
    double      m_recoveredMeanMotion;
    double      m_bstar;
    double      m_period;
    DateTime    m_epoch;
};

inline std::ostream& operator<<(std::ostream& strm, const Orbit& o) {
    strm << std::right << std::fixed;
    strm << "Eccentricity:         " << std::setw(12) << o.getEccentricity() << std::endl;
    strm << "SemiMajor Axis:       " << std::setw(12) << o.getSemiMajorAxis() << std::endl;
    strm << "Inclination:          " << std::setw(12) << o.getInclination(DEGS) << std::endl;
    strm << "AscendingNode:        " << std::setw(12) << o.getAscendingNode(DEGS) << std::endl;
    strm << "ArgumentPeriapsis:    " << std::setw(12) << o.getArgumentPeriapsis(DEGS) << std::endl;
    strm << "MeanAnomoly:          " << std::setw(12) << o.getMeanAnomaly(DEGS) << std::endl;
    
    strm << "Mean Motion:          " << std::setw(12) << o.getMeanMotion() << std::endl;
    strm << "Recovered Mean Motion:" << std::setw(12) << o.getRecoveredMeanMotion() << std::endl;
    strm << "BStar:                " << std::setw(12) << o.getBStar() << std::endl;
    
    strm << "Periapsis:            " << std::setw(12) << o.getPeriapsis() << std::endl;
    strm << "Period:               " << std::setw(12) << o.getPeriod() << std::endl;
    strm << "Epoch:                " << std::setw(12) << o.getEpoch() << std::endl;
    return strm;
}

