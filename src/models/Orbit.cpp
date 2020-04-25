//
//  Orbit.cpp
//
//  Created by Patricio González Vivo on 5/31/18.
//

#include "hypatia/MathOps.h"
#include "hypatia/TimeOps.h"
#include "hypatia/CoordOps.h"

#include <math.h>

#include "hypatia/models/Orbit.h"

const double kAE = 1.0;
const double kXJ2 = 1.082616e-3;

/*
 * alternative XKE
 * affects final results
 * aiaa-2006-6573
 * const double kXKE = 60.0 / sqrt(kXKMPER * kXKMPER * kXKMPER / kMU);
 * dundee
 * const double kXKE = 7.43669161331734132e-2;
 */
const double kXKE = 60.0 / sqrt(CoordOps::EARTH_EQUATORIAL_RADIUS_KM * CoordOps::EARTH_EQUATORIAL_RADIUS_KM * CoordOps::EARTH_EQUATORIAL_RADIUS_KM / CoordOps::EARTH_GRAVITATIONAL_CONSTANT);
const double kCK2 = 0.5 * kXJ2 * kAE * kAE;

Orbit::Orbit() {
    
}

Orbit::Orbit(const TLE& tle) {
    /*
     * extract and format tle data
     */
    m_meanAnomoly = tle.getMeanAnomaly(RADS);
    m_ascendingNode = tle.getRightAscendingNode(RADS);
    m_argumentPeriapsis = tle.getArgumentPerigee(RADS);
    m_eccentricity = tle.getEccentricity();
    m_inclination = tle.getInclination(RADS);
    m_meanMotion = tle.getMeanMotion() * MathOps::TAU / TimeOps::MINUTES_PER_DAY;
    m_bstar = tle.getBStar();
    m_epoch = tle.getEpoch();
    
    /*
     * recover original mean motion (xnodp) and semimajor axis (aodp)
     * from input elements
     */
    const double a1 = pow(kXKE / getMeanMotion(), MathOps::TWO_THIRD);
    const double cosio = cos(getInclination(RADS));
    const double theta2 = cosio * cosio;
    const double x3thm1 = 3.0 * theta2 - 1.0;
    const double eosq = getEccentricity() * getEccentricity();
    const double betao2 = 1.0 - eosq;
    const double betao = sqrt(betao2);
    const double temp = (1.5 * kCK2) * x3thm1 / (betao * betao2);
    const double del1 = temp / (a1 * a1);
    const double a0 = a1 * (1.0 - del1 * (1.0 / 3.0 + del1 * (1.0 + del1 * 134.0 / 81.0)));
    const double del0 = temp / (a0 * a0);
    
    m_recoveredMeanMotion = getMeanMotion() / (1.0 + del0);
    /*
     * alternative way to calculate
     * doesnt affect final results
     * m_semimajorAxis = pow(XKE / RecoveredMeanMotion(), TWOTHIRD);
     */
    m_semiMajorAxis = a0 / (1.0 - del0);
    
    /*
     * find perigee and period
     */
    m_periapsis = (getSemiMajorAxis() * (1.0 - getEccentricity()) - kAE) * CoordOps::EARTH_EQUATORIAL_RADIUS_KM;
    m_period = MathOps::TAU / getMeanMotion();
}

double Orbit::getInclination(ANGLE_UNIT _type) const {
    if (_type == DEGS) {
        return MathOps::toDegrees(m_inclination);
    }
    else {
        return m_inclination;
    }
}

double Orbit::getAscendingNode(ANGLE_UNIT _type) const {
    if (_type == DEGS) {
        return MathOps::toDegrees(m_ascendingNode);
    }
    else {
        return m_ascendingNode;
    }
}

double Orbit::getArgumentPeriapsis(ANGLE_UNIT _type) const {
    if (_type == DEGS) {
        return MathOps::toDegrees(m_argumentPeriapsis);
    }
    else {
        return m_argumentPeriapsis;
    }
}

double Orbit::getMeanAnomaly(ANGLE_UNIT _type) const {
    if (_type == DEGS) {
        return MathOps::toDegrees(m_meanAnomoly);
    }
    else {
        return m_meanAnomoly;
    }
}
