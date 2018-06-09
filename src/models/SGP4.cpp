/*
 * Copyright 2013 Daniel Warner <contact@danrw.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SGP4.h"
#include "Exception.h"

#include "../TimeOps.h"
#include "../CoordOps.h"
#include "../primitives/Vector.h"

#include <cmath>
#include <iomanip>

const double kAE = 1.0;
const double kQ0 = 120.0;
const double kS0 = 78.0;
const double kXJ2 = 1.082616e-3;
const double kXJ3 = -2.53881e-6;
const double kXJ4 = -1.65597e-6;

/*
 * alternative XKE
 * affects final results
 * aiaa-2006-6573
 * const double kXKE = 60.0 / sqrt(kXKMPER * kXKMPER * kXKMPER / CoordOps::EARTH_GRAVITATIONAL_CONSTANT);
 * dundee
 * const double kXKE = 7.43669161331734132e-2;
 */
const double kXKE = 60.0 / sqrt(CoordOps::EARTH_EQUATORIAL_RADIUS_KM * CoordOps::EARTH_EQUATORIAL_RADIUS_KM * CoordOps::EARTH_EQUATORIAL_RADIUS_KM / CoordOps::EARTH_GRAVITATIONAL_CONSTANT);
const double kCK2 = 0.5 * kXJ2 * kAE * kAE;
const double kCK4 = -0.375 * kXJ4 * kAE * kAE * kAE * kAE;

/*
 * alternative QOMS2T
 * affects final results
 * aiaa-2006-6573
 * #define QOMS2T   (pow(((Q0 - S0) / XKMPER), 4.0))
 * dundee
 * #define QOMS2T   (1.880279159015270643865e-9)
 */
const double kQOMS2T = pow(((kQ0 - kS0) / CoordOps::EARTH_EQUATORIAL_RADIUS_KM), 4.0);
const double kS = kAE * (1.0 + kS0 / CoordOps::EARTH_EQUATORIAL_RADIUS_KM);
const double kTHDT = 4.37526908801129966e-3;

const SGP4::CommonConstants SGP4::Empty_CommonConstants = SGP4::CommonConstants();
const SGP4::NearSpaceConstants SGP4::Empty_NearSpaceConstants = SGP4::NearSpaceConstants();
const SGP4::DeepSpaceConstants SGP4::Empty_DeepSpaceConstants = SGP4::DeepSpaceConstants();
const SGP4::IntegratorConstants SGP4::Empty_IntegratorConstants = SGP4::IntegratorConstants();
const SGP4::IntegratorParams SGP4::Empty_IntegratorParams = SGP4::IntegratorParams();

SGP4::SGP4() {
}

SGP4::~SGP4() {
    
}

void SGP4::setTLE(const TLE& tle) {
    /*
     * extract and format tle data
     */
    m_elements = Orbit(tle);
    
    /*
     * reset all constants etc
     */
    reset();
    
    /*
     * error checks
     */
    if (m_elements.getEccentricity() < 0.0 || m_elements.getEccentricity() > 0.999) {
        throw Exception("Eccentricity out of range");
    }
    
    if (m_elements.getInclination(RADS) < 0.0 || m_elements.getInclination(RADS) > MathOps::PI) {
        throw Exception("Inclination out of range");
    }
    
    m_commonConsts.cosio = cos(m_elements.getInclination(RADS));
    m_commonConsts.sinio = sin(m_elements.getInclination(RADS));
    const double theta2 = m_commonConsts.cosio * m_commonConsts.cosio;
    m_commonConsts.x3thm1 = 3.0 * theta2 - 1.0;
    const double eosq = m_elements.getEccentricity() * m_elements.getEccentricity();
    const double betao2 = 1.0 - eosq;
    const double betao = sqrt(betao2);
    
    if (m_elements.getPeriod() >= 225.0) {
        m_useDeepSpace = true;
    }
    else {
        m_useDeepSpace = false;
        m_useSimpleModel = false;
        /*
         * for perigee less than 220 kilometers, the simple_model flag is set
         * and the equations are truncated to linear variation in sqrt a and
         * quadratic variation in mean anomly. also, the c3 term, the
         * delta omega term and the delta m term are dropped
         */
        if (m_elements.getPeriapsis() < 220.0) {
            m_useSimpleModel = true;
        }
    }
    
    /*
     * for perigee below 156km, the values of
     * s4 and qoms2t are altered
     */
    double s4 = kS;
    double qoms24 = kQOMS2T;
    if (m_elements.getPeriapsis() < 156.0) {
        s4 = m_elements.getPeriapsis() - 78.0;
        if (m_elements.getPeriapsis() < 98.0) {
            s4 = 20.0;
        }
        qoms24 = pow((120.0 - s4) * kAE / CoordOps::EARTH_EQUATORIAL_RADIUS_KM, 4.0);
        s4 = s4 / CoordOps::EARTH_EQUATORIAL_RADIUS_KM + kAE;
    }
    
    /*
     * generate constants
     */
    const double pinvsq = 1.0
    / (m_elements.getSemiMajorAxis()
       * m_elements.getSemiMajorAxis()
       * betao2 * betao2);
    const double tsi = 1.0 / (m_elements.getSemiMajorAxis() - s4);
    m_commonConsts.eta = m_elements.getSemiMajorAxis()
    * m_elements.getEccentricity() * tsi;
    const double etasq = m_commonConsts.eta * m_commonConsts.eta;
    const double eeta = m_elements.getEccentricity() * m_commonConsts.eta;
    const double psisq = fabs(1.0 - etasq);
    const double coef = qoms24 * pow(tsi, 4.0);
    const double coef1 = coef / pow(psisq, 3.5);
    const double c2 = coef1 * m_elements.getRecoveredMeanMotion()
    * (m_elements.getSemiMajorAxis()
       * (1.0 + 1.5 * etasq + eeta * (4.0 + etasq))
       + 0.75 * kCK2 * tsi / psisq * m_commonConsts.x3thm1
       * (8.0 + 3.0 * etasq * (8.0 + etasq)));
    m_commonConsts.c1 = m_elements.getBStar() * c2;
    m_commonConsts.a3ovk2 = -kXJ3 / kCK2 * kAE * kAE * kAE;
    m_commonConsts.x1mth2 = 1.0 - theta2;
    m_commonConsts.c4 = 2.0 * m_elements.getRecoveredMeanMotion()
    * coef1 * m_elements.getSemiMajorAxis() * betao2
    * (m_commonConsts.eta * (2.0 + 0.5 * etasq) + m_elements.getEccentricity()
       * (0.5 + 2.0 * etasq)
       - 2.0 * kCK2 * tsi / (m_elements.getSemiMajorAxis() * psisq)
       * (-3.0 * m_commonConsts.x3thm1 * (1.0 - 2.0 * eeta + etasq
                                          * (1.5 - 0.5 * eeta))
          + 0.75 * m_commonConsts.x1mth2 * (2.0 * etasq - eeta *
                                            (1.0 + etasq)) * cos(2.0 * m_elements.getArgumentPeriapsis(RADS))));
    const double theta4 = theta2 * theta2;
    const double temp1 = 3.0 * kCK2 * pinvsq * m_elements.getRecoveredMeanMotion();
    const double temp2 = temp1 * kCK2 * pinvsq;
    const double temp3 = 1.25 * kCK4 * pinvsq * pinvsq * m_elements.getRecoveredMeanMotion();
    m_commonConsts.xmdot = m_elements.getRecoveredMeanMotion() + 0.5 * temp1 * betao *
    m_commonConsts.x3thm1 + 0.0625 * temp2 * betao *
    (13.0 - 78.0 * theta2 + 137.0 * theta4);
    const double x1m5th = 1.0 - 5.0 * theta2;
    m_commonConsts.omgdot = -0.5 * temp1 * x1m5th +
    0.0625 * temp2 * (7.0 - 114.0 * theta2 + 395.0 * theta4) +
    temp3 * (3.0 - 36.0 * theta2 + 49.0 * theta4);
    const double xhdot1 = -temp1 * m_commonConsts.cosio;
    m_commonConsts.xnodot = xhdot1 + (0.5 * temp2 * (4.0 - 19.0 * theta2) + 2.0 * temp3 *
                                      (3.0 - 7.0 * theta2)) * m_commonConsts.cosio;
    m_commonConsts.xnodcf = 3.5 * betao2 * xhdot1 * m_commonConsts.c1;
    m_commonConsts.t2cof = 1.5 * m_commonConsts.c1;
    
    if (fabs(m_commonConsts.cosio + 1.0) > 1.5e-12) {
        m_commonConsts.xlcof = 0.125 * m_commonConsts.a3ovk2 * m_commonConsts.sinio * (3.0 + 5.0 * m_commonConsts.cosio) / (1.0 + m_commonConsts.cosio);
    }
    else {
        m_commonConsts.xlcof = 0.125 * m_commonConsts.a3ovk2 * m_commonConsts.sinio * (3.0 + 5.0 * m_commonConsts.cosio) / 1.5e-12;
    }
    
    m_commonConsts.aycof = 0.25 * m_commonConsts.a3ovk2 * m_commonConsts.sinio;
    m_commonConsts.x7thm1 = 7.0 * theta2 - 1.0;
    
    if (m_useDeepSpace) {
        m_deepspaceConsts.gsto = TimeOps::toGreenwichSiderealTime(m_elements.getEpoch());
        deepSpaceInitialise(eosq, m_commonConsts.sinio, m_commonConsts.cosio, betao,
                            theta2, betao2,
                            m_commonConsts.xmdot, m_commonConsts.omgdot, m_commonConsts.xnodot);
    }
    else {
        double c3 = 0.0;
        if (m_elements.getEccentricity() > 1.0e-4)
        {
            c3 = coef * tsi * m_commonConsts.a3ovk2 * m_elements.getRecoveredMeanMotion() * kAE *
            m_commonConsts.sinio / m_elements.getEccentricity();
        }
        
        m_nearspaceConsts.c5 = 2.0 * coef1 * m_elements.getSemiMajorAxis() * betao2 * (1.0 + 2.75 *
                                                                                            (etasq + eeta) + eeta * etasq);
        m_nearspaceConsts.omgcof = m_elements.getBStar() * c3 * cos(m_elements.getArgumentPeriapsis(RADS));
        
        m_nearspaceConsts.xmcof = 0.0;
        if (m_elements.getEccentricity() > 1.0e-4) {
            m_nearspaceConsts.xmcof = -MathOps::TWO_THIRD * coef * m_elements.getBStar() * kAE / eeta;
        }
        
        m_nearspaceConsts.delmo = pow(1.0 + m_commonConsts.eta * (cos(m_elements.getMeanAnomaly(RADS))), 3.0);
        m_nearspaceConsts.sinmo = sin(m_elements.getMeanAnomaly(RADS));
        
        if (!m_useSimpleModel) {
            const double c1sq = m_commonConsts.c1 * m_commonConsts.c1;
            m_nearspaceConsts.d2 = 4.0 * m_elements.getSemiMajorAxis() * tsi * c1sq;
            const double temp = m_nearspaceConsts.d2 * tsi * m_commonConsts.c1 / 3.0;
            m_nearspaceConsts.d3 = (17.0 * m_elements.getSemiMajorAxis() + s4) * temp;
            m_nearspaceConsts.d4 = 0.5 * temp * m_elements.getSemiMajorAxis() *
            tsi * (221.0 * m_elements.getSemiMajorAxis() + 31.0 * s4) * m_commonConsts.c1;
            m_nearspaceConsts.t3cof = m_nearspaceConsts.d2 + 2.0 * c1sq;
            m_nearspaceConsts.t4cof = 0.25 * (3.0 * m_nearspaceConsts.d3 + m_commonConsts.c1 *
                                              (12.0 * m_nearspaceConsts.d2 + 10.0 * c1sq));
            m_nearspaceConsts.t5cof = 0.2 * (3.0 * m_nearspaceConsts.d4 + 12.0 * m_commonConsts.c1 *
                                             m_nearspaceConsts.d3 + 6.0 * m_nearspaceConsts.d2 * m_nearspaceConsts.d2 + 15.0 *
                                             c1sq * (2.0 * m_nearspaceConsts.d2 + c1sq));
        }
    }
}

ECI SGP4::getECI(double _jd) const {
    if (m_useDeepSpace) {
        return findPositionSDP4(_jd);
    }
    else {
        return findPositionSGP4(_jd);
    }
}

ECI SGP4::findPositionSDP4(double _jd) const {
    double tsince = (_jd - m_elements.getEpoch()) * TimeOps::MINUTES_PER_DAY;
    
    /*
     * the final values
     */
    double e;
    double a;
    double omega;
    double xl;
    double xnode;
    double xincl;
    
    /*
     * update for secular gravity and atmospheric drag
     */
    double xmdf = m_elements.getMeanAnomaly(RADS) + m_commonConsts.xmdot * tsince;
    double omgadf = m_elements.getArgumentPeriapsis(RADS) + m_commonConsts.omgdot * tsince;
    const double xnoddf = m_elements.getAscendingNode(RADS) + m_commonConsts.xnodot * tsince;
    
    const double tsq = tsince * tsince;
    xnode = xnoddf + m_commonConsts.xnodcf * tsq;
    double tempa = 1.0 - m_commonConsts.c1 * tsince;
    double tempe = m_elements.getBStar() * m_commonConsts.c4 * tsince;
    double templ = m_commonConsts.t2cof * tsq;
    
    double xn = m_elements.getRecoveredMeanMotion();
    e = m_elements.getEccentricity();
    xincl = m_elements.getInclination(RADS);
    
    deepSpaceSecular(tsince, xmdf, omgadf, xnode, e, xincl, xn);
    
    if (xn <= 0.0) {
        throw Exception("Error: (xn <= 0.0)");
    }
    
    a = pow(kXKE / xn, MathOps::TWO_THIRD) * tempa * tempa;
    e -= tempe;
    double xmam = xmdf + m_elements.getRecoveredMeanMotion() * templ;
    
    deepSpacePeriodics(tsince, e, xincl, omgadf, xnode, xmam);
    
    /*
     * keeping xincl positive important unless you need to display xincl
     * and dislike negative inclinations
     */
    if (xincl < 0.0) {
        xincl = -xincl;
        xnode += MathOps::PI;
        omgadf -= MathOps::PI;
    }
    
    xl = xmam + omgadf + xnode;
    omega = omgadf;
    
    /*
     * fix tolerance for error recognition
     */
    if (e <= -0.001) {
        throw Exception("Error: (e <= -0.001)");
    }
    else if (e < 1.0e-6) {
        e = 1.0e-6;
    }
    else if (e > (1.0 - 1.0e-6)) {
        e = 1.0 - 1.0e-6;
    }
    
    /*
     * re-compute the perturbed values
     */
    const double perturbed_sinio = sin(xincl);
    const double perturbed_cosio = cos(xincl);
    
    const double perturbed_theta2 = perturbed_cosio * perturbed_cosio;
    
    const double perturbed_x3thm1 = 3.0 * perturbed_theta2 - 1.0;
    const double perturbed_x1mth2 = 1.0 - perturbed_theta2;
    const double perturbed_x7thm1 = 7.0 * perturbed_theta2 - 1.0;
    
    double perturbed_xlcof;
    if (fabs(perturbed_cosio + 1.0) > 1.5e-12) {
        perturbed_xlcof = 0.125 * m_commonConsts.a3ovk2 * perturbed_sinio * (3.0 + 5.0 * perturbed_cosio) / (1.0 + perturbed_cosio);
    }
    else {
        perturbed_xlcof = 0.125 * m_commonConsts.a3ovk2 * perturbed_sinio * (3.0 + 5.0 * perturbed_cosio) / 1.5e-12;
    }
    
    const double perturbed_aycof = 0.25 * m_commonConsts.a3ovk2 * perturbed_sinio;
    
    /*
     * using calculated values, find position and velocity
     */
    return calculateFinalPositionVelocity(tsince, e,
                                          a, omega, xl, xnode,
                                          xincl, perturbed_xlcof, perturbed_aycof,
                                          perturbed_x3thm1, perturbed_x1mth2, perturbed_x7thm1,
                                          perturbed_cosio, perturbed_sinio);
    
}

ECI SGP4::findPositionSGP4(const double _jd) const {
    double tsince = (_jd - m_elements.getEpoch()) * TimeOps::MINUTES_PER_DAY;
    /*
     * the final values
     */
    double e;
    double a;
    double omega;
    double xl;
    double xnode;
    double xincl;
    
    /*
     * update for secular gravity and atmospheric drag
     */
    const double xmdf = m_elements.getMeanAnomaly(RADS) + m_commonConsts.xmdot * tsince;
    const double omgadf = m_elements.getArgumentPeriapsis(RADS) + m_commonConsts.omgdot * tsince;
    const double xnoddf = m_elements.getAscendingNode(RADS) + m_commonConsts.xnodot * tsince;
    
    const double tsq = tsince * tsince;
    xnode = xnoddf + m_commonConsts.xnodcf * tsq;
    double tempa = 1.0 - m_commonConsts.c1 * tsince;
    double tempe = m_elements.getBStar() * m_commonConsts.c4 * tsince;
    double templ = m_commonConsts.t2cof * tsq;
    
    xincl = m_elements.getInclination(RADS);
    omega = omgadf;
    double xmp = xmdf;
    
    if (!m_useSimpleModel) {
        const double delomg = m_nearspaceConsts.omgcof * tsince;
        const double delm = m_nearspaceConsts.xmcof
        * (pow(1.0 + m_commonConsts.eta * cos(xmdf), 3.0)
           * - m_nearspaceConsts.delmo);
        const double temp = delomg + delm;
        
        xmp += temp;
        omega -= temp;
        
        const double tcube = tsq * tsince;
        const double tfour = tsince * tcube;
        
        tempa = tempa - m_nearspaceConsts.d2 * tsq - m_nearspaceConsts.d3
        * tcube - m_nearspaceConsts.d4 * tfour;
        tempe += m_elements.getBStar() * m_nearspaceConsts.c5
        * (sin(xmp) - m_nearspaceConsts.sinmo);
        templ += m_nearspaceConsts.t3cof * tcube + tfour
        * (m_nearspaceConsts.t4cof + tsince * m_nearspaceConsts.t5cof);
    }
    
    a = m_elements.getSemiMajorAxis() * tempa * tempa;
    e = m_elements.getEccentricity() - tempe;
    xl = xmp + omega + xnode + m_elements.getRecoveredMeanMotion() * templ;
    
    /*
     * fix tolerance for error recognition
     */
    if (e <= -0.001) {
        throw Exception("Error: (e <= -0.001)");
    }
    else if (e < 1.0e-6) {
        e = 1.0e-6;
    }
    else if (e > (1.0 - 1.0e-6)) {
        e = 1.0 - 1.0e-6;
    }
    
    /*
     * using calculated values, find position and velocity
     * we can pass in constants from Initialise() as these dont change
     */
    return calculateFinalPositionVelocity(tsince, e,
                                          a, omega, xl, xnode,
                                          xincl, m_commonConsts.xlcof, m_commonConsts.aycof,
                                          m_commonConsts.x3thm1, m_commonConsts.x1mth2, m_commonConsts.x7thm1,
                                          m_commonConsts.cosio, m_commonConsts.sinio);
    
}

ECI SGP4::calculateFinalPositionVelocity(
                                         const double tsince,
                                         const double e,
                                         const double a,
                                         const double omega,
                                         const double xl,
                                         const double xnode,
                                         const double xincl,
                                         const double xlcof,
                                         const double aycof,
                                         const double x3thm1,
                                         const double x1mth2,
                                         const double x7thm1,
                                         const double cosio,
                                         const double sinio) const
{
    const double beta2 = 1.0 - e * e;
    const double xn = kXKE / pow(a, 1.5);
    /*
     * long period periodics
     */
    const double axn = e * cos(omega);
    const double temp11 = 1.0 / (a * beta2);
    const double xll = temp11 * xlcof * axn;
    const double aynl = temp11 * aycof;
    const double xlt = xl + xll;
    const double ayn = e * sin(omega) + aynl;
    const double elsq = axn * axn + ayn * ayn;
    
    if (elsq >= 1.0) {
        throw Exception("Error: (elsq >= 1.0)");
    }
    
    /*
     * solve keplers equation
     * - solve using Newton-Raphson root solving
     * - here capu is almost the mean anomoly
     * - initialise the eccentric anomaly term epw
     * - The fmod saves reduction of angle to +/-2pi in sin/cos() and prevents
     * convergence problems.
     */
    const double capu = fmod(xlt - xnode, MathOps::TAU);
    double epw = capu;
    
    double sinepw = 0.0;
    double cosepw = 0.0;
    double ecose = 0.0;
    double esine = 0.0;
    
    /*
     * sensibility check for N-R correction
     */
    const double max_newton_naphson = 1.25 * fabs(sqrt(elsq));
    
    bool kepler_running = true;
    
    for (int i = 0; i < 10 && kepler_running; i++) {
        sinepw = sin(epw);
        cosepw = cos(epw);
        ecose = axn * cosepw + ayn * sinepw;
        esine = axn * sinepw - ayn * cosepw;
        
        double f = capu - epw + esine;
        
        if (fabs(f) < 1.0e-12) {
            kepler_running = false;
        }
        else {
            /*
             * 1st order Newton-Raphson correction
             */
            const double fdot = 1.0 - ecose;
            double delta_epw = f / fdot;
            
            /*
             * 2nd order Newton-Raphson correction.
             * f / (fdot - 0.5 * d2f * f/fdot)
             */
            if (i == 0) {
                if (delta_epw > max_newton_naphson) {
                    delta_epw = max_newton_naphson;
                }
                else if (delta_epw < -max_newton_naphson) {
                    delta_epw = -max_newton_naphson;
                }
            }
            else {
                delta_epw = f / (fdot + 0.5 * esine * delta_epw);
            }
            
            /*
             * Newton-Raphson correction of -F/DF
             */
            epw += delta_epw;
        }
    }
    /*
     * short period preliminary quantities
     */
    const double temp21 = 1.0 - elsq;
    const double pl = a * temp21;
    
    if (pl < 0.0) {
        throw Exception("Error: (pl < 0.0)");
    }
    
    const double r = a * (1.0 - ecose);
    const double temp31 = 1.0 / r;
    const double rdot = kXKE * sqrt(a) * esine * temp31;
    const double rfdot = kXKE * sqrt(pl) * temp31;
    const double temp32 = a * temp31;
    const double betal = sqrt(temp21);
    const double temp33 = 1.0 / (1.0 + betal);
    const double cosu = temp32 * (cosepw - axn + ayn * esine * temp33);
    const double sinu = temp32 * (sinepw - ayn - axn * esine * temp33);
    const double u = atan2(sinu, cosu);
    const double sin2u = 2.0 * sinu * cosu;
    const double cos2u = 2.0 * cosu * cosu - 1.0;
    
    /*
     * update for short periodics
     */
    const double temp41 = 1.0 / pl;
    const double temp42 = kCK2 * temp41;
    const double temp43 = temp42 * temp41;
    
    const double rk = r * (1.0 - 1.5 * temp43 * betal * x3thm1)
    + 0.5 * temp42 * x1mth2 * cos2u;
    const double uk = u - 0.25 * temp43 * x7thm1 * sin2u;
    const double xnodek = xnode + 1.5 * temp43 * cosio * sin2u;
    const double xinck = xincl + 1.5 * temp43 * cosio * sinio * cos2u;
    const double rdotk = rdot - xn * temp42 * x1mth2 * sin2u;
    const double rfdotk = rfdot + xn * temp42 * (x1mth2 * cos2u + 1.5 * x3thm1);
    
    /*
     * orientation vectors
     */
    const double sinuk = sin(uk);
    const double cosuk = cos(uk);
    const double sinik = sin(xinck);
    const double cosik = cos(xinck);
    const double sinnok = sin(xnodek);
    const double cosnok = cos(xnodek);
    const double xmx = -sinnok * cosik;
    const double xmy = cosnok * cosik;
    const double ux = xmx * sinuk + cosnok * cosuk;
    const double uy = xmy * sinuk + sinnok * cosuk;
    const double uz = sinik * sinuk;
    const double vx = xmx * cosuk - cosnok * sinuk;
    const double vy = xmy * cosuk - sinnok * sinuk;
    const double vz = sinik * cosuk;
    /*
     * position and velocity
     */
    const double x = rk * ux * CoordOps::EARTH_EQUATORIAL_RADIUS_KM;
    const double y = rk * uy * CoordOps::EARTH_EQUATORIAL_RADIUS_KM;
    const double z = rk * uz * CoordOps::EARTH_EQUATORIAL_RADIUS_KM;
    Vector position(x, y, z);
    const double xdot = (rdotk * ux + rfdotk * vx) * CoordOps::EARTH_EQUATORIAL_RADIUS_KM / 60.0;
    const double ydot = (rdotk * uy + rfdotk * vy) * CoordOps::EARTH_EQUATORIAL_RADIUS_KM / 60.0;
    const double zdot = (rdotk * uz + rfdotk * vz) * CoordOps::EARTH_EQUATORIAL_RADIUS_KM / 60.0;
    Vector velocity(xdot, ydot, zdot);
    
    if (rk < 1.0)
    {
        throw Exception("DACADE EXCEPTION");
//                        elements_.Epoch().AddMinutes(tsince),
//                               position,
//                               velocity);
    }
    
    
    return ECI(m_elements.getEpoch() + tsince / TimeOps::MINUTES_PER_DAY, position, velocity);
}

static inline double evaluateCubicPolynomial(
                                             const double x,
                                             const double constant,
                                             const double linear,
                                             const double squared,
                                             const double cubed) {
    return constant + x * (linear + x * (squared + x * cubed));
}

void SGP4::deepSpaceInitialise(
                               const double eosq,
                               const double sinio,
                               const double cosio,
                               const double betao,
                               const double theta2,
                               const double betao2,
                               const double xmdot,
                               const double omgdot,
                               const double xnodot) {
    double se = 0.0;
    double si = 0.0;
    double sl = 0.0;
    double sgh = 0.0;
    double shdq = 0.0;
    
    double bfact = 0.0;
    
    static const double ZNS = 1.19459E-5;
    static const double C1SS = 2.9864797E-6;
    static const double ZES = 0.01675;
    static const double ZNL = 1.5835218E-4;
    static const double C1L = 4.7968065E-7;
    static const double ZEL = 0.05490;
    static const double ZCOSIS = 0.91744867;
    static const double ZSINI = 0.39785416;
    static const double ZSINGS = -0.98088458;
    static const double ZCOSGS = 0.1945905;
    static const double Q22 = 1.7891679E-6;
    static const double Q31 = 2.1460748E-6;
    static const double Q33 = 2.2123015E-7;
    static const double ROOT22 = 1.7891679E-6;
    static const double ROOT32 = 3.7393792E-7;
    static const double ROOT44 = 7.3636953E-9;
    static const double ROOT52 = 1.1428639E-7;
    static const double ROOT54 = 2.1765803E-9;
    
    const double aqnv = 1.0 / m_elements.getSemiMajorAxis();
    const double xpidot = omgdot + xnodot;
    const double sinq = sin(m_elements.getAscendingNode(RADS));
    const double cosq = cos(m_elements.getAscendingNode(RADS));
    const double sing = sin(m_elements.getArgumentPeriapsis(RADS));
    const double cosg = cos(m_elements.getArgumentPeriapsis(RADS));
    
    /*
     * initialize lunar / solar terms
     */
    const double jday = TimeOps::toJD( m_elements.getEpoch() ) - TimeOps::J2000;
    
    const double xnodce = 4.5236020 - 9.2422029e-4 * jday;
    const double xnodce_temp = fmod(xnodce, MathOps::TAU);
    const double stem = sin(xnodce_temp);
    const double ctem = cos(xnodce_temp);
    const double zcosil = 0.91375164 - 0.03568096 * ctem;
    const double zsinil = sqrt(1.0 - zcosil * zcosil);
    const double zsinhl = 0.089683511 * stem / zsinil;
    const double zcoshl = sqrt(1.0 - zsinhl * zsinhl);
    const double c = 4.7199672 + 0.22997150 * jday;
    const double gam = 5.8351514 + 0.0019443680 * jday;
    m_deepspaceConsts.zmol = MathOps::normalize(c - gam, RADS);
    double zx = 0.39785416 * stem / zsinil;
    double zy = zcoshl * ctem + 0.91744867 * zsinhl * stem;
    zx = atan2(zx, zy);
    zx = fmod(gam + zx - xnodce, MathOps::TAU);
    
    const double zcosgl = cos(zx);
    const double zsingl = sin(zx);
    m_deepspaceConsts.zmos = MathOps::normalize(6.2565837 + 0.017201977 * jday, RADS);
    
    /*
     * do solar terms
     */
    double zcosg = ZCOSGS;
    double zsing = ZSINGS;
    double zcosi = ZCOSIS;
    double zsini = ZSINI;
    double zcosh = cosq;
    double zsinh = sinq;
    double cc = C1SS;
    double zn = ZNS;
    double ze = ZES;
    const double xnoi = 1.0 / m_elements.getRecoveredMeanMotion();
    
    for (int cnt = 0; cnt < 2; cnt++) {
        /*
         * solar terms are done a second time after lunar terms are done
         */
        const double a1 = zcosg * zcosh + zsing * zcosi * zsinh;
        const double a3 = -zsing * zcosh + zcosg * zcosi * zsinh;
        const double a7 = -zcosg * zsinh + zsing * zcosi * zcosh;
        const double a8 = zsing * zsini;
        const double a9 = zsing * zsinh + zcosg * zcosi*zcosh;
        const double a10 = zcosg * zsini;
        const double a2 = cosio * a7 + sinio * a8;
        const double a4 = cosio * a9 + sinio * a10;
        const double a5 = -sinio * a7 + cosio * a8;
        const double a6 = -sinio * a9 + cosio * a10;
        const double x1 = a1 * cosg + a2 * sing;
        const double x2 = a3 * cosg + a4 * sing;
        const double x3 = -a1 * sing + a2 * cosg;
        const double x4 = -a3 * sing + a4 * cosg;
        const double x5 = a5 * sing;
        const double x6 = a6 * sing;
        const double x7 = a5 * cosg;
        const double x8 = a6 * cosg;
        const double z31 = 12.0 * x1 * x1 - 3. * x3 * x3;
        const double z32 = 24.0 * x1 * x2 - 6. * x3 * x4;
        const double z33 = 12.0 * x2 * x2 - 3. * x4 * x4;
        double z1 = 3.0 * (a1 * a1 + a2 * a2) + z31 * eosq;
        double z2 = 6.0 * (a1 * a3 + a2 * a4) + z32 * eosq;
        double z3 = 3.0 * (a3 * a3 + a4 * a4) + z33 * eosq;
        
        const double z11 = -6.0 * a1 * a5
        + eosq * (-24. * x1 * x7 - 6. * x3 * x5);
        const double z12 = -6.0 * (a1 * a6 + a3 * a5)
        + eosq * (-24. * (x2 * x7 + x1 * x8) - 6. * (x3 * x6 + x4 * x5));
        const double z13 = -6.0 * a3 * a6
        + eosq * (-24. * x2 * x8 - 6. * x4 * x6);
        const double z21 = 6.0 * a2 * a5
        + eosq * (24. * x1 * x5 - 6. * x3 * x7);
        const double z22 = 6.0 * (a4 * a5 + a2 * a6)
        + eosq * (24. * (x2 * x5 + x1 * x6) - 6. * (x4 * x7 + x3 * x8));
        const double z23 = 6.0 * a4 * a6
        + eosq * (24. * x2 * x6 - 6. * x4 * x8);
        
        z1 = z1 + z1 + betao2 * z31;
        z2 = z2 + z2 + betao2 * z32;
        z3 = z3 + z3 + betao2 * z33;
        
        const double s3 = cc * xnoi;
        const double s2 = -0.5 * s3 / betao;
        const double s4 = s3 * betao;
        const double s1 = -15.0 * m_elements.getEccentricity() * s4;
        const double s5 = x1 * x3 + x2 * x4;
        const double s6 = x2 * x3 + x1 * x4;
        const double s7 = x2 * x4 - x1 * x3;
        
        se = s1 * zn * s5;
        si = s2 * zn * (z11 + z13);
        sl = -zn * s3 * (z1 + z3 - 14.0 - 6.0 * eosq);
        sgh = s4 * zn * (z31 + z33 - 6.0);
        
        /*
         * replaced
         * sh = -zn * s2 * (z21 + z23
         * with
         * shdq = (-zn * s2 * (z21 + z23)) / sinio
         */
        if (m_elements.getInclination(RADS) < 5.2359877e-2 || m_elements.getInclination(RADS) > MathOps::PI - 5.2359877e-2) {
            shdq = 0.0;
        }
        else {
            shdq = (-zn * s2 * (z21 + z23)) / sinio;
        }
        
        m_deepspaceConsts.ee2 = 2.0 * s1 * s6;
        m_deepspaceConsts.e3 = 2.0 * s1 * s7;
        m_deepspaceConsts.xi2 = 2.0 * s2 * z12;
        m_deepspaceConsts.xi3 = 2.0 * s2 * (z13 - z11);
        m_deepspaceConsts.xl2 = -2.0 * s3 * z2;
        m_deepspaceConsts.xl3 = -2.0 * s3 * (z3 - z1);
        m_deepspaceConsts.xl4 = -2.0 * s3 * (-21.0 - 9.0 * eosq) * ze;
        m_deepspaceConsts.xgh2 = 2.0 * s4 * z32;
        m_deepspaceConsts.xgh3 = 2.0 * s4 * (z33 - z31);
        m_deepspaceConsts.xgh4 = -18.0 * s4 * ze;
        m_deepspaceConsts.xh2 = -2.0 * s2 * z22;
        m_deepspaceConsts.xh3 = -2.0 * s2 * (z23 - z21);
        
        if (cnt == 1) {
            break;
        }
        /*
         * do lunar terms
         */
        m_deepspaceConsts.sse = se;
        m_deepspaceConsts.ssi = si;
        m_deepspaceConsts.ssl = sl;
        m_deepspaceConsts.ssh = shdq;
        m_deepspaceConsts.ssg = sgh - cosio * m_deepspaceConsts.ssh;
        m_deepspaceConsts.se2 = m_deepspaceConsts.ee2;
        m_deepspaceConsts.si2 = m_deepspaceConsts.xi2;
        m_deepspaceConsts.sl2 = m_deepspaceConsts.xl2;
        m_deepspaceConsts.sgh2 = m_deepspaceConsts.xgh2;
        m_deepspaceConsts.sh2 = m_deepspaceConsts.xh2;
        m_deepspaceConsts.se3 = m_deepspaceConsts.e3;
        m_deepspaceConsts.si3 = m_deepspaceConsts.xi3;
        m_deepspaceConsts.sl3 = m_deepspaceConsts.xl3;
        m_deepspaceConsts.sgh3 = m_deepspaceConsts.xgh3;
        m_deepspaceConsts.sh3 = m_deepspaceConsts.xh3;
        m_deepspaceConsts.sl4 = m_deepspaceConsts.xl4;
        m_deepspaceConsts.sgh4 = m_deepspaceConsts.xgh4;
        zcosg = zcosgl;
        zsing = zsingl;
        zcosi = zcosil;
        zsini = zsinil;
        zcosh = zcoshl * cosq + zsinhl * sinq;
        zsinh = sinq * zcoshl - cosq * zsinhl;
        zn = ZNL;
        cc = C1L;
        ze = ZEL;
    }
    
    m_deepspaceConsts.sse += se;
    m_deepspaceConsts.ssi += si;
    m_deepspaceConsts.ssl += sl;
    m_deepspaceConsts.ssg += sgh - cosio * shdq;
    m_deepspaceConsts.ssh += shdq;
    
    m_deepspaceConsts.resonance_flag = false;
    m_deepspaceConsts.synchronous_flag = false;
    bool initialise_integrator = true;
    
    if (m_elements.getRecoveredMeanMotion() < 0.0052359877 && m_elements.getRecoveredMeanMotion() > 0.0034906585) {
        /*
         * 24h synchronous resonance terms initialisation
         */
        m_deepspaceConsts.resonance_flag = true;
        m_deepspaceConsts.synchronous_flag = true;
        
        const double g200 = 1.0 + eosq * (-2.5 + 0.8125 * eosq);
        const double g310 = 1.0 + 2.0 * eosq;
        const double g300 = 1.0 + eosq * (-6.0 + 6.60937 * eosq);
        const double f220 = 0.75 * (1.0 + cosio) * (1.0 + cosio);
        const double f311 = 0.9375 * sinio * sinio * (1.0 + 3.0 * cosio)
        - 0.75 * (1.0 + cosio);
        double f330 = 1.0 + cosio;
        f330 = 1.875 * f330 * f330 * f330;
        m_deepspaceConsts.del1 = 3.0 * m_elements.getRecoveredMeanMotion()
        * m_elements.getRecoveredMeanMotion()
        * aqnv * aqnv;
        m_deepspaceConsts.del2 = 2.0 * m_deepspaceConsts.del1
        * f220 * g200 * Q22;
        m_deepspaceConsts.del3 = 3.0 * m_deepspaceConsts.del1
        * f330 * g300 * Q33 * aqnv;
        m_deepspaceConsts.del1 = m_deepspaceConsts.del1
        * f311 * g310 * Q31 * aqnv;
        
        m_integratorConsts.xlamo = m_elements.getMeanAnomaly(RADS) + m_elements.getAscendingNode(RADS)  + m_elements.getArgumentPeriapsis(RADS) - m_deepspaceConsts.gsto;
        bfact = xmdot + xpidot - kTHDT;
        bfact += m_deepspaceConsts.ssl + m_deepspaceConsts.ssg + m_deepspaceConsts.ssh;
    }
    else if (m_elements.getRecoveredMeanMotion() < 8.26e-3
             || m_elements.getRecoveredMeanMotion() > 9.24e-3
             || m_elements.getEccentricity() < 0.5) {
        initialise_integrator = false;
    }
    else {
        /*
         * geopotential resonance initialisation for 12 hour orbits
         */
        m_deepspaceConsts.resonance_flag = true;
        
        double g211;
        double g310;
        double g322;
        double g410;
        double g422;
        double g520;
        
        double g201 = -0.306 - (m_elements.getEccentricity() - 0.64) * 0.440;
        
        if (m_elements.getEccentricity() <= 0.65) {
            g211 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           3.616, -13.247, +16.290, 0.0);
            g310 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -19.302, 117.390, -228.419, 156.591);
            g322 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -18.9068, 109.7927, -214.6334, 146.5816);
            g410 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -41.122, 242.694, -471.094, 313.953);
            g422 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -146.407, 841.880, -1629.014, 1083.435);
            g520 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -532.114, 3017.977, -5740.032, 3708.276);
        }
        else {
            g211 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -72.099, 331.819, -508.738, 266.724);
            g310 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -346.844, 1582.851, -2415.925, 1246.113);
            g322 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -342.585, 1554.908, -2366.899, 1215.972);
            g410 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -1052.797, 4758.686, -7193.992, 3651.957);
            g422 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -3581.69, 16178.11, -24462.77, 12422.52);
            
            if (m_elements.getEccentricity() <= 0.715)  {
                g520 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                               1464.74, -4664.75, 3763.64, 0.0);
            }
            else {
                g520 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                               -5149.66, 29936.92, -54087.36, 31324.56);
            }
        }
        
        double g533;
        double g521;
        double g532;
        
        if (m_elements.getEccentricity() < 0.7) {
            g533 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -919.2277, 4988.61, -9064.77, 5542.21);
            g521 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -822.71072, 4568.6173, -8491.4146, 5337.524);
            g532 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -853.666, 4690.25, -8624.77, 5341.4);
        }
        else {
            g533 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -37995.78, 161616.52, -229838.2, 109377.94);
            g521 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -51752.104, 218913.95, -309468.16, 146349.42);
            g532 = evaluateCubicPolynomial(m_elements.getEccentricity(),
                                           -40023.88, 170470.89, -242699.48, 115605.82);
        }
        
        const double sini2 = sinio * sinio;
        const double f220 = 0.75 * (1.0 + 2.0 * cosio + theta2);
        const double f221 = 1.5 * sini2;
        const double f321 = 1.875 * sinio * (1.0 - 2.0 * cosio - 3.0 * theta2);
        const double f322 = -1.875 * sinio * (1.0 + 2.0 * cosio - 3.0 * theta2);
        const double f441 = 35.0 * sini2 * f220;
        const double f442 = 39.3750 * sini2 * sini2;
        const double f522 = 9.84375 * sinio
        * (sini2 * (1.0 - 2.0 * cosio - 5.0 * theta2)
           + 0.33333333 * (-2.0 + 4.0 * cosio + 6.0 * theta2));
        const double f523 = sinio
        * (4.92187512 * sini2 * (-2.0 - 4.0 * cosio + 10.0 * theta2)
           + 6.56250012 * (1.0 + 2.0 * cosio - 3.0 * theta2));
        const double f542 = 29.53125 * sinio * (2.0 - 8.0 * cosio + theta2 *
                                                (-12.0 + 8.0 * cosio + 10.0 * theta2));
        const double f543 = 29.53125 * sinio * (-2.0 - 8.0 * cosio + theta2 *
                                                (12.0 + 8.0 * cosio - 10.0 * theta2));
        
        const double xno2 = m_elements.getRecoveredMeanMotion()
        * m_elements.getRecoveredMeanMotion();
        const double ainv2 = aqnv * aqnv;
        
        double temp1 = 3.0 * xno2 * ainv2;
        double temp = temp1 * ROOT22;
        m_deepspaceConsts.d2201 = temp * f220 * g201;
        m_deepspaceConsts.d2211 = temp * f221 * g211;
        temp1 = temp1 * aqnv;
        temp = temp1 * ROOT32;
        m_deepspaceConsts.d3210 = temp * f321 * g310;
        m_deepspaceConsts.d3222 = temp * f322 * g322;
        temp1 = temp1 * aqnv;
        temp = 2.0 * temp1 * ROOT44;
        m_deepspaceConsts.d4410 = temp * f441 * g410;
        m_deepspaceConsts.d4422 = temp * f442 * g422;
        temp1 = temp1 * aqnv;
        temp = temp1 * ROOT52;
        m_deepspaceConsts.d5220 = temp * f522 * g520;
        m_deepspaceConsts.d5232 = temp * f523 * g532;
        temp = 2.0 * temp1 * ROOT54;
        m_deepspaceConsts.d5421 = temp * f542 * g521;
        m_deepspaceConsts.d5433 = temp * f543 * g533;
        
        m_integratorConsts.xlamo = m_elements.getMeanAnomaly(RADS) + m_elements.getAscendingNode(RADS) + m_elements.getAscendingNode(RADS) - m_deepspaceConsts.gsto - m_deepspaceConsts.gsto;
        bfact = xmdot + xnodot + xnodot - kTHDT - kTHDT;
        bfact = bfact + m_deepspaceConsts.ssl + m_deepspaceConsts.ssh + m_deepspaceConsts.ssh;
    }
    
    if (initialise_integrator) {
        /*
         * initialise integrator
         */
        m_integratorConsts.xfact = bfact - m_elements.getRecoveredMeanMotion();
        m_integratorParams.atime = 0.0;
        m_integratorParams.xni = m_elements.getRecoveredMeanMotion();
        m_integratorParams.xli = m_integratorConsts.xlamo;
        /*
         * precompute dot terms for epoch
         */
        deepSpaceCalcDotTerms(m_integratorConsts.values_0);
    }
}

void SGP4::deepSpaceCalculateLunarSolarTerms(
                                             const double tsince,
                                             double& pe,
                                             double& pinc,
                                             double& pl,
                                             double& pgh,
                                             double& ph) const {
    static const double ZES = 0.01675;
    static const double ZNS = 1.19459E-5;
    static const double ZNL = 1.5835218E-4;
    static const double ZEL = 0.05490;
    
    /*
     * calculate solar terms for time tsince
     */
    double zm = m_deepspaceConsts.zmos + ZNS * tsince;
    double zf = zm + 2.0 * ZES * sin(zm);
    double sinzf = sin(zf);
    double f2 = 0.5 * sinzf * sinzf - 0.25;
    double f3 = -0.5 * sinzf * cos(zf);
    
    const double ses = m_deepspaceConsts.se2 * f2
    + m_deepspaceConsts.se3 * f3;
    const double sis = m_deepspaceConsts.si2 * f2
    + m_deepspaceConsts.si3 * f3;
    const double sls = m_deepspaceConsts.sl2 * f2
    + m_deepspaceConsts.sl3 * f3
    + m_deepspaceConsts.sl4 * sinzf;
    const double sghs = m_deepspaceConsts.sgh2 * f2
    + m_deepspaceConsts.sgh3 * f3
    + m_deepspaceConsts.sgh4 * sinzf;
    const double shs = m_deepspaceConsts.sh2 * f2
    + m_deepspaceConsts.sh3 * f3;
    
    /*
     * calculate lunar terms for time tsince
     */
    zm = m_deepspaceConsts.zmol + ZNL * tsince;
    zf = zm + 2.0 * ZEL * sin(zm);
    sinzf = sin(zf);
    f2 = 0.5 * sinzf * sinzf - 0.25;
    f3 = -0.5 * sinzf * cos(zf);
    
    const double sel = m_deepspaceConsts.ee2 * f2
    + m_deepspaceConsts.e3 * f3;
    const double sil = m_deepspaceConsts.xi2 * f2
    + m_deepspaceConsts.xi3 * f3;
    const double sll = m_deepspaceConsts.xl2 * f2
    + m_deepspaceConsts.xl3 * f3
    + m_deepspaceConsts.xl4 * sinzf;
    const double sghl = m_deepspaceConsts.xgh2 * f2
    + m_deepspaceConsts.xgh3 * f3
    + m_deepspaceConsts.xgh4 * sinzf;
    const double shl = m_deepspaceConsts.xh2 * f2
    + m_deepspaceConsts.xh3 * f3;
    
    /*
     * merge calculated values
     */
    pe = ses + sel;
    pinc = sis + sil;
    pl = sls + sll;
    pgh = sghs + sghl;
    ph = shs + shl;
}

void SGP4::deepSpacePeriodics(
                              const double tsince,
                              double& em,
                              double& xinc,
                              double& omgasm,
                              double& xnodes,
                              double& xll) const {
    /*
     * storage for lunar / solar terms
     * set by DeepSpaceCalculateLunarSolarTerms()
     */
    double pe = 0.0;
    double pinc = 0.0;
    double pl = 0.0;
    double pgh = 0.0;
    double ph = 0.0;
    
    /*
     * calculate lunar / solar terms for current time
     */
    deepSpaceCalculateLunarSolarTerms(tsince, pe, pinc, pl, pgh, ph);
    
    xinc += pinc;
    em += pe;
    
    /* Spacetrack report #3 has sin/cos from before perturbations
     * added to xinc (oldxinc), but apparently report # 6 has then
     * from after they are added.
     * use for strn3
     * if (elements_.Inclination() >= 0.2)
     * use for gsfc
     * if (xinc >= 0.2)
     * (moved from start of function)
     */
    const double sinis = sin(xinc);
    const double cosis = cos(xinc);
    
    if (xinc >= 0.2) {
        /*
         * apply periodics directly
         */
        const double tmp_ph = ph / sinis;
        
        omgasm += pgh - cosis * tmp_ph;
        xnodes += tmp_ph;
        xll += pl;
    }
    else {
        /*
         * apply periodics with lyddane modification
         */
        const double sinok = sin(xnodes);
        const double cosok = cos(xnodes);
        double alfdp = sinis * sinok;
        double betdp = sinis * cosok;
        const double dalf = ph * cosok + pinc * cosis * sinok;
        const double dbet = -ph * sinok + pinc * cosis * cosok;
        
        alfdp += dalf;
        betdp += dbet;
        
        xnodes = MathOps::normalize(xnodes, RADS);
        
        double xls = xll + omgasm + cosis * xnodes;
        double dls = pl + pgh - pinc * xnodes * sinis;
        xls += dls;
        
        /*
         * save old xnodes value
         */
        const double oldxnodes = xnodes;
        
        xnodes = atan2(alfdp, betdp);
        if (xnodes < 0.0) {
            xnodes += MathOps::TAU;
        }
        
        /*
         * Get perturbed xnodes in to same quadrant as original.
         * RAAN is in the range of 0 to 360 degrees
         * atan2 is in the range of -180 to 180 degrees
         */
        if (fabs(oldxnodes - xnodes) > MathOps::PI)  {
            if (xnodes < oldxnodes) {
                xnodes += MathOps::TAU;
            }
            else {
                xnodes -= MathOps::TAU;
            }
        }
        
        xll += pl;
        omgasm = xls - xll - cosis * xnodes;
    }
}

void SGP4::deepSpaceSecular(
                            const double tsince,
                            double& xll,
                            double& omgasm,
                            double& xnodes,
                            double& em,
                            double& xinc,
                            double& xn) const {
    static const double STEP = 720.0;
    static const double STEP2 = 259200.0;
    
    xll += m_deepspaceConsts.ssl * tsince;
    omgasm += m_deepspaceConsts.ssg * tsince;
    xnodes += m_deepspaceConsts.ssh * tsince;
    em += m_deepspaceConsts.sse * tsince;
    xinc += m_deepspaceConsts.ssi * tsince;
    
    if (m_deepspaceConsts.resonance_flag) {
        /*
         * 1st condition (if tsince is less than one time step from epoch)
         * 2nd condition (if integrator_params_.atime and
         *     tsince are of opposite signs, so zero crossing required)
         * 3rd condition (if tsince is closer to zero than
         *     integrator_params_.atime, only integrate away from zero)
         */
        if (fabs(tsince) < STEP ||
            tsince * m_integratorParams.atime <= 0.0 ||
            fabs(tsince) < fabs(m_integratorParams.atime)) {
            /*
             * restart from epoch
             */
            m_integratorParams.atime = 0.0;
            m_integratorParams.xni = m_elements.getRecoveredMeanMotion();
            m_integratorParams.xli = m_integratorConsts.xlamo;
            
            /*
             * restore precomputed values for epoch
             */
            m_integratorParams.values_t = m_integratorConsts.values_0;
        }
        
        double ft = tsince - m_integratorParams.atime;
        
        /*
         * if time difference (ft) is greater than the time step (720.0)
         * loop around until integrator_params_.atime is within one time step of
         * tsince
         */
        if (fabs(ft) >= STEP) {
            /*
             * calculate step direction to allow integrator_params_.atime
             * to catch up with tsince
             */
            double delt = -STEP;
            if (ft >= 0.0) {
                delt = STEP;
            }
            
            do {
                /*
                 * integrate using current dot terms
                 */
                deepSpaceIntegrator(delt, STEP2, m_integratorParams.values_t);
                
                /*
                 * calculate dot terms for next integration
                 */
                deepSpaceCalcDotTerms(m_integratorParams.values_t);
                
                ft = tsince - m_integratorParams.atime;
            } while (fabs(ft) >= STEP);
        }
        
        /*
         * integrator
         */
        xn = m_integratorParams.xni
        + m_integratorParams.values_t.xndot * ft
        + m_integratorParams.values_t.xnddt * ft * ft * 0.5;
        const double xl = m_integratorParams.xli
        + m_integratorParams.values_t.xldot * ft
        + m_integratorParams.values_t.xndot * ft * ft * 0.5;
        const double temp = -xnodes + m_deepspaceConsts.gsto + tsince * kTHDT;
        
        if (m_deepspaceConsts.synchronous_flag) {
            xll = xl + temp - omgasm;
        }
        else {
            xll = xl + temp + temp;
        }
    }
}

void SGP4::deepSpaceCalcDotTerms(struct IntegratorValues& values) const {
    static const double G22 = 5.7686396;
    static const double G32 = 0.95240898;
    static const double G44 = 1.8014998;
    static const double G52 = 1.0508330;
    static const double G54 = 4.4108898;
    static const double FASX2 = 0.13130908;
    static const double FASX4 = 2.8843198;
    static const double FASX6 = 0.37448087;
    
    if (m_deepspaceConsts.synchronous_flag) {
        
        values.xndot = m_deepspaceConsts.del1
        * sin(m_integratorParams.xli - FASX2)
        + m_deepspaceConsts.del2
        * sin(2.0 * (m_integratorParams.xli - FASX4))
        + m_deepspaceConsts.del3
        * sin(3.0 * (m_integratorParams.xli - FASX6));
        values.xnddt = m_deepspaceConsts.del1
        * cos(m_integratorParams.xli - FASX2)
        + 2.0 * m_deepspaceConsts.del2
        * cos(2.0 * (m_integratorParams.xli - FASX4))
        + 3.0 * m_deepspaceConsts.del3
        * cos(3.0 * (m_integratorParams.xli - FASX6));
    }
    else {
        const double xomi = m_elements.getArgumentPeriapsis(RADS) + m_commonConsts.omgdot * m_integratorParams.atime;
        const double x2omi = xomi + xomi;
        const double x2li = m_integratorParams.xli + m_integratorParams.xli;
        
        values.xndot = m_deepspaceConsts.d2201
        * sin(x2omi + m_integratorParams.xli - G22)
        * + m_deepspaceConsts.d2211
        * sin(m_integratorParams.xli - G22)
        + m_deepspaceConsts.d3210
        * sin(xomi + m_integratorParams.xli - G32)
        + m_deepspaceConsts.d3222
        * sin(-xomi + m_integratorParams.xli - G32)
        + m_deepspaceConsts.d4410
        * sin(x2omi + x2li - G44)
        + m_deepspaceConsts.d4422
        * sin(x2li - G44)
        + m_deepspaceConsts.d5220
        * sin(xomi + m_integratorParams.xli - G52)
        + m_deepspaceConsts.d5232
        * sin(-xomi + m_integratorParams.xli - G52)
        + m_deepspaceConsts.d5421
        * sin(xomi + x2li - G54)
        + m_deepspaceConsts.d5433
        * sin(-xomi + x2li - G54);
        values.xnddt = m_deepspaceConsts.d2201
        * cos(x2omi + m_integratorParams.xli - G22)
        + m_deepspaceConsts.d2211
        * cos(m_integratorParams.xli - G22)
        + m_deepspaceConsts.d3210
        * cos(xomi + m_integratorParams.xli - G32)
        + m_deepspaceConsts.d3222
        * cos(-xomi + m_integratorParams.xli - G32)
        + m_deepspaceConsts.d5220
        * cos(xomi + m_integratorParams.xli - G52)
        + m_deepspaceConsts.d5232
        * cos(-xomi + m_integratorParams.xli - G52)
        + 2.0 * (m_deepspaceConsts.d4410 * cos(x2omi + x2li - G44)
                 + m_deepspaceConsts.d4422
                 * cos(x2li - G44)
                 + m_deepspaceConsts.d5421
                 * cos(xomi + x2li - G54)
                 + m_deepspaceConsts.d5433
                 * cos(-xomi + x2li - G54));
    }
    
    values.xldot = m_integratorParams.xni + m_integratorConsts.xfact;
    values.xnddt *= values.xldot;
}

void SGP4::deepSpaceIntegrator(
                               const double delt,
                               const double step2,
                               const struct IntegratorValues &values) const {
    /*
     * integrator
     */
    m_integratorParams.xli += values.xldot * delt + values.xndot * step2;
    m_integratorParams.xni += values.xndot * delt + values.xnddt * step2;
    
    /*
     * increment integrator time
     */
    m_integratorParams.atime += delt;
}

void SGP4::reset() {
    m_useSimpleModel = false;
    m_useDeepSpace = false;
    
    m_commonConsts     = Empty_CommonConstants;
    m_nearspaceConsts  = Empty_NearSpaceConstants;
    m_deepspaceConsts  = Empty_DeepSpaceConstants;
    m_integratorConsts = Empty_IntegratorConstants;
    m_integratorParams = Empty_IntegratorParams;
}
