/*****************************************************************************\
 * TLE
 *
 * Created by Patricio González Vivo on 5/31/18.
 \*****************************************************************************/

#pragma once

#include "../MathOps.h"
#include "../primitives/DateTime.h"

/**
 * @brief Processes a two-line element set used to convey OrbitalElements.
 *
 * Used to extract the various raw fields from a two-line element set.
 */
class TLE {
public:
    TLE(const std::string& line_one, const std::string& line_two);
    TLE(const std::string& name, const std::string& line_one, const std::string& line_two);
    TLE(const TLE& tle);
    
    std::string         getName() const { return m_name; }
    std::string         getLine1() const { return m_line_one; }
    std::string         getLine2() const { return m_line_two; }

    unsigned int        getNoradNumber() const { return m_norad_number; }
    std::string         getIntDesignator() const { return m_int_designator; }
    DateTime            getEpoch() const { return m_epoch; }
    double              getMeanMotionDt2() const { return m_mean_motion_dt2; }
    double              getMeanMotionDdt6() const { return m_mean_motion_ddt6; }
    double              getBStar() const { return m_bstar; }
    double              getInclination(ANGLE_TYPE _type) const;
    double              getRightAscendingNode(ANGLE_TYPE _type) const;
    double              getEccentricity() const { return m_eccentricity; }
    double              getArgumentPerigee(ANGLE_TYPE _type) const;
    double              getMeanAnomaly(ANGLE_TYPE _type) const ;
    double              getMeanMotion() const {  return m_mean_motion; }
    unsigned int        getOrbitNumber() const { return m_orbit_number; }
    static unsigned int getLineLength() { return TLE_LEN_LINE_DATA; }

private:
    void        initialize();
    static bool isValidLineLength(const std::string& str);
    void        extractInteger(const std::string& str, unsigned int& val);
    void        extractDouble(const std::string& str, int point_pos, double& val);
    void        extractExponential(const std::string& str, double& val);
    
private:
    std::string     m_name;
    std::string     m_line_one;
    std::string     m_line_two;
    
    std::string     m_int_designator;       // International designator
    DateTime        m_epoch;
    double          m_mean_motion_dt2;      //  first time derivative of the mean motion divided by two
    double          m_mean_motion_ddt6;     //  second time derivative of mean motion divided by six
    double          m_bstar;                //  BSTAR drag term
    double          m_inclination;
    double          m_right_ascending_node;
    double          m_eccentricity;
    double          m_argument_perigee;
    double          m_mean_anomaly;
    double          m_mean_motion;
    unsigned int    m_norad_number;
    unsigned int    m_orbit_number;
    
    // expected tle line length
    static const unsigned int TLE_LEN_LINE_DATA = 69;
    static const unsigned int TLE_LEN_LINE_NAME = 22;
};

inline std::ostream& operator<<(std::ostream& strm, const TLE& t) {
    strm << std::right << std::fixed;
    strm << "Norad Number:         " << t.getNoradNumber() << std::endl;
    strm << "Int. Designator:      " << t.getIntDesignator() << std::endl;
    strm << "Epoch:                " << t.getEpoch() << std::endl;
    strm << "Orbit Number:         " << t.getOrbitNumber() << std::endl;
    strm << std::setprecision(8);
    strm << "Eccentricity:         ";
    strm << std::setw(12) << t.getEccentricity() << std::endl;
    strm << "Inclination:          ";
    strm << std::setw(12) << t.getInclination(DEGS) << std::endl;
    strm << "Right Ascending Node: ";
    strm << std::setw(12) << t.getRightAscendingNode(DEGS) << std::endl;
    strm << "Argument Perigee:     ";
    strm << std::setw(12) << t.getArgumentPerigee(DEGS) << std::endl;
    strm << "Mean Anomaly:         ";
    strm << std::setw(12) << t.getMeanAnomaly(DEGS) << std::endl;
    
    strm << "Mean Motion:          ";
    strm << std::setw(12) << t.getMeanMotion() << std::endl;
    strm << "Mean Motion Dt2:      ";
    strm << std::setw(12) << t.getMeanMotionDt2() << std::endl;
    strm << "Mean Motion Ddt6:     ";
    strm << std::setw(12) << t.getMeanMotionDdt6() << std::endl;
    
    strm << "BStar:                ";
    strm << std::setw(12) << t.getBStar() << std::endl;
    return strm;
}

