//
//  Tle.cpp
//
//  Created by Patricio González Vivo on 5/31/18.
//

#include "TLE.h"

#include "../MathOps.h"
#include "../TimeOps.h"
#include "Exception.h"

#include <sstream>
#include <locale>

//  0.      Card number:                        0 Must be 1
//  1.      Blank:                              1 Must be a blank space.
//  2-6.    Satellite or USSTRATCOM number:     2-5 must be either an alphanumeric character or a blank space.
//                                              6 must be an alphanumeric character.
static const unsigned int TLE1_COL_NORADNUM = 2;
static const unsigned int TLE1_LEN_NORADNUM = 5;
//  7.      Security classification:            7 No validation performed.
static const unsigned int TLE1_COL_INTLDESC_A = 9;
static const unsigned int TLE1_LEN_INTLDESC_A = 2;
//  8.      Blank:                              8 Must be a blank space
//  9-16.   International number:               9-16 No validation performed
// static const unsigned int TLE1_COL_INTLDESC_B = 11;
static const unsigned int TLE1_LEN_INTLDESC_B = 3;
// static const unsigned int TLE1_COL_INTLDESC_C = 14;
static const unsigned int TLE1_LEN_INTLDESC_C = 3;
//  17.     Blank:                              17 Must be a blank space.
//  18.     Epoch Year:                         18 Must be a digit [0-9].
static const unsigned int TLE1_COL_EPOCH_A = 18;
static const unsigned int TLE1_LEN_EPOCH_A = 2;
//  20-31.  Epoch day to eight decimal places:  20-22 must be a digit [0-9]. The value of this 3-digit integer must not be greater than 366.
//                                              23 must be a period (.).
//                                              24-31 must be a digit [0-9].
static const unsigned int TLE1_COL_EPOCH_B = 20;
static const unsigned int TLE1_LEN_EPOCH_B = 12;
//  32.     Blank:                              32 must be a blank space.
//  33-42.  N/2 - Revolutions per day squared:  33 must be either - or + or 0 or a blank space.
//                                              34 must be a period (.).
//                                              35-42 must be a digit [0-9].
static const unsigned int TLE1_COL_MEANMOTIONDT2 = 33;
static const unsigned int TLE1_LEN_MEANMOTIONDT2 = 10;
//  43.     Blank:                              43 Must be a blank space
//  44-51   N/6 - Revolutions per day cubed:    44 must be either - or + or a blank space.
//                                              45-49 must be a digit [0-9].
//                                              50 must be either - or + or a blank space
//                                              51 must be either a digit [0-9] or a blank space.
static const unsigned int TLE1_COL_MEANMOTIONDDT6 = 44;
static const unsigned int TLE1_LEN_MEANMOTIONDDT6 = 8;
//  52.     Blank:                              52 Must be a blank space.
//  53-60.  Bstar drag:                         53 must be either - or + or a blank space.
//                                              54-58 must be a digit [0-9].
//                                              59 must be either - or + or a blank space.
//                                              60 must be either a digit [0-9] or a blank space.
static const unsigned int TLE1_COL_BSTAR = 53;
static const unsigned int TLE1_LEN_BSTAR = 8;
//  61.     Blank:                              61 Must be a blank space.
//  62.     Ephemeris:                          62 Must be either a digit [0-9] or a blank space.
// static const unsigned int TLE1_COL_EPHEMTYPE = 62;
// static const unsigned int TLE1_LEN_EPHEMTYPE = 1;
//  63.     Blank:                              63 Must be a blank space.
//  64-67.  Element set number:                 64-66 must be either a digit [0-9] or a blank space.
//                                              67 must be a digit [0-9]
// static const unsigned int TLE1_COL_ELNUM = 64;
// static const unsigned int TLE1_LEN_ELNUM = 4;
//  68.     CheckSum Value:                     68 No validation performed.

//  0.      Card number:                        0 Must be 2.
//  1.      Blank:                              Must be a blank space.
//  2-6.    Satellite or USSTRATCOM number:     3-6 must be either an alphanumeric character or a blank space.
//                                              7 must be either an alphanumeric character.
static const unsigned int TLE2_COL_NORADNUM = 2;
static const unsigned int TLE2_LEN_NORADNUM = 5;
//  7.      Blank:                              Must be a blank space.
//  8-15.   Inclination (degrees):              Determines the inclination value. Valid values are 0.0 through 180.0.
//                                              9-11 must be either a digit [0-9] or a blank space.
//                                              12 must be a period (.).
//                                              13-16 must be a digit [0-9].
static const unsigned int TLE2_COL_INCLINATION = 8;
static const unsigned int TLE2_LEN_INCLINATION = 8;
//  16.     Blank:                              Must be a blank space.
//  17.     Right ascension of node (degrees):  Determines the RAAN value. Valid values are 0.0 through 360.0.
//                                              18-20 must be either a digit [0-9] or a blank space.
//                                              21 must be a period (.).
//                                              22-24 must be a digit [0-9].
static const unsigned int TLE2_COL_RAASCENDNODE = 17;
static const unsigned int TLE2_LEN_RAASCENDNODE = 8;
//  25.     Blank:                              25 Must be a blank space.
//  26-32.  Eccentricity (decimal point):       26-32 Must be a digit [0-9].
static const unsigned int TLE2_COL_ECCENTRICITY = 26;
static const unsigned int TLE2_LEN_ECCENTRICITY = 7;
//  33.     Blank:                              33 Must be a blank space.
//  34-41   Argument of perigee (degrees)       Determines the argument of perigee value. Valid values are 0.0 through 360.0.
//                                              34-36 must be either a digit [0-9] or a blank space.
//                                              37 must be a period (.).
//                                              38-41 must be a digit [0-9].
static const unsigned int TLE2_COL_ARGPERIGEE = 34;
static const unsigned int TLE2_LEN_ARGPERIGEE = 8;
//  42.     Blank:                              42 Must be a blank space.
//  43-50   Mean anomaly (degrees):             Determines the mean anomaly value. Valid values are 0.0 through 360.0.
//                                              43-45 must be either a digit [0-9] or a blank space.
//                                              46 must be a period (.).
//                                              47-50 must be a digit [0-9].
static const unsigned int TLE2_COL_MEANANOMALY = 43;
static const unsigned int TLE2_LEN_MEANANOMALY = 8;
//  51.     Blank:                              51 Must be a blank space.
//  52-62.  Mean motion (revolutions per day):  Determines the mean motion value. The value must not be greater than 17.0.
//                                              52-53 must be either a digit [0-9] or a blank space.
//                                              54 must be a period (.).
//                                              55-62 must be a digit [0-9].
static const unsigned int TLE2_COL_MEANMOTION = 52;
static const unsigned int TLE2_LEN_MEANMOTION = 11;
//  63-67.  Revolution number at epoch:         63-66 must be either a digit [0-9] or a blank space.
//                                              67 must be a digit [0-9].
static const unsigned int TLE2_COL_REVATEPOCH = 63;
static const unsigned int TLE2_LEN_REVATEPOCH = 5;
//  68.     CheckSum Value:                     68 No validation performed.

// expected tle line length
static const unsigned int TLE_LEN_LINE_DATA = 69;
// static const unsigned int TLE_LEN_LINE_NAME = 22;

template <typename T>bool FromString(const std::string& str, T& val) {
    std::stringstream ss(str);
    return !(ss >> val).fail();
}

/**
 * Check
 * @param str The string to check
 * @returns Whether true of the string has a valid length
 */
bool isValidLineLength(const std::string& str) {
    return str.length() == TLE_LEN_LINE_DATA ? true : false;
}

/**
 * Convert a string containing an integer
 * @param[in] str The string to convert
 * @param[out] val The result
 * @exception TleException on conversion error
 */
void extractInteger(const std::string& str, unsigned int& val) {
    bool found_digit = false;
    unsigned int temp = 0;
    
    for (std::string::const_iterator i = str.begin(); i != str.end(); ++i) {
        if (isdigit(*i)) {
            found_digit = true;
            temp = (temp * 10) + static_cast<unsigned int>(*i - '0');
        }
        else if (found_digit) {
            throw Exception("Unexpected non digit");
        }
        else if (*i != ' ') {
            throw Exception("Invalid character");
        }
    }
    
    if (!found_digit) {
        val = 0;
    }
    else {
        val = temp;
    }
}

/**
 * Convert a string containing an double
 * @param[in] str The string to convert
 * @param[in] point_pos The position of the decimal point. (-1 if none)
 * @param[out] val The result
 * @exception TleException on conversion error
 */
void extractDouble(const std::string& str, int point_pos, double& val) {
    std::string temp;
    bool found_digit = false;
    
    for (std::string::const_iterator i = str.begin(); i != str.end(); ++i) {
        /*
         * integer part
         */
        if (point_pos >= 0 && i < str.begin() + point_pos - 1) {
            bool done = false;
            
            if (i == str.begin()) {
                if(*i == '-' || *i == '+') {
                    /*
                     * first character could be signed
                     */
                    temp += *i;
                    done = true;
                }
            }
            
            if (!done) {
                if (isdigit(*i)) {
                    found_digit = true;
                    temp += *i;
                }
                else if (found_digit) {
                    throw Exception("Unexpected non digit");
                }
                else if (*i != ' ') {
                    throw Exception("Invalid character");
                }
            }
        }
        /*
         * decimal point
         */
        else if (point_pos >= 0 && i == str.begin() + point_pos - 1) {
            if (temp.length() == 0) {
                /*
                 * integer part is blank, so add a '0'
                 */
                temp += '0';
            }
            
            if (*i == '.') {
                /*
                 * decimal point found
                 */
                temp += *i;
            }
            else {
                throw Exception("Failed to find decimal point");
            }
        }
        /*
         * fraction part
         */
        else {
            if (i == str.begin() && point_pos == -1)
            {
                /*
                 * no decimal point expected, add 0. beginning
                 */
                temp += '0';
                temp += '.';
            }
            
            /*
             * should be a digit
             */
            if (isdigit(*i)) {
                temp += *i;
            }
            else {
                throw Exception("Invalid digit");
            }
        }
    }
    
    if (!FromString<double>(temp, val)) {
        throw Exception("Failed to convert value to double");
    }
}

/**
 * Convert a string containing an exponential
 * @param[in] str The string to convert
 * @param[out] val The result
 * @exception TleException on conversion error
 */
void extractExponential(const std::string& str, double& val) {
    std::string temp;
    
    for (std::string::const_iterator i = str.begin(); i != str.end(); ++i) {
        if (i == str.begin()) {
            if (*i == '-' || *i == '+' || *i == ' ') {
                if (*i == '-') {
                    temp += *i;
                }
                temp += '0';
                temp += '.';
            }
            else {
                throw Exception("Invalid sign");
            }
        }
        else if (i == str.end() - 2) {
            if (*i == '-' || *i == '+') {
                temp += 'e';
                temp += *i;
            }
            else {
                throw Exception("Invalid exponential sign");
            }
        }
        else {
            if (isdigit(*i)) {
                temp += *i;
            }
            else {
                throw Exception("Invalid digit");
            }
        }
    }
    
    if (!FromString<double>(temp, val)) {
        throw Exception("Failed to convert value to double");
    }
}

/**
 * @details Initialise given the two lines of a tle
 * @param[in] line_one Tle line one
 * @param[in] line_two Tle line two
 */
TLE::TLE(const std::string& _line_one, const std::string& _line_two):m_line_one(_line_one), m_line_two(_line_two) {
    initialize();
}

/**
 * @details Initialise given the satellite name and the two lines of a tle
 * @param[in] name Satellite name
 * @param[in] line_one Tle line one
 * @param[in] line_two Tle line two
 */
TLE::TLE(const std::string& _name, const std::string& _line_one, const std::string& _line_two):m_name(_name), m_line_one(_line_one), m_line_two(_line_two) {
    m_name = _name;
    initialize();
}

/**
 * Copy constructor
 * @param[in] tle Tle object to copy from
 */
TLE::TLE(const TLE& tle) {
    m_name = tle.m_name;
    m_line_one = tle.m_line_one;
    m_line_two = tle.m_line_two;
    
    m_norad_number = tle.m_norad_number;
    m_int_designator = tle.m_int_designator;
    m_epoch = tle.m_epoch;
    m_mean_motion_dt2 = tle.m_mean_motion_dt2;
    m_mean_motion_ddt6 = tle.m_mean_motion_ddt6;
    m_bstar = tle.m_bstar;
    m_inclination = tle.m_inclination;
    m_right_ascending_node = tle.m_right_ascending_node;
    m_eccentricity = tle.m_eccentricity;
    m_argument_perigee = tle.m_argument_perigee;
    m_mean_anomaly = tle.m_mean_anomaly;
    m_mean_motion = tle.m_mean_motion;
    m_orbit_number = tle.m_orbit_number;
}

/**
 * Initialise the tle object.
 * @exception TleException
 */
void TLE::initialize() {
    if (!isValidLineLength(m_line_one)) {
        throw Exception("Invalid length for line one");
    }
    
    if (!isValidLineLength(m_line_two)) {
        throw Exception("Invalid length for line two");
    }
    
    if (m_line_one[0] != '1') {
        throw Exception("Invalid line beginning for line one");
    }

    if (m_line_two[0] != '2') {
        throw Exception("Invalid line beginning for line two");
    }
    
    unsigned int sat_number_1;
    unsigned int sat_number_2;
    
    extractInteger(m_line_one.substr(TLE1_COL_NORADNUM,
                                     TLE1_LEN_NORADNUM), sat_number_1);
    extractInteger(m_line_two.substr(TLE2_COL_NORADNUM,
                                     TLE2_LEN_NORADNUM), sat_number_2);
    
    if (sat_number_1 != sat_number_2) {
        throw Exception("Satellite numbers do not match");
    }
    
    m_norad_number = sat_number_1;
    
    if (m_name.empty()) {
        m_name = m_line_one.substr(TLE1_COL_NORADNUM, TLE1_LEN_NORADNUM);
    }
    
    m_int_designator = m_line_one.substr(TLE1_COL_INTLDESC_A,
                                         TLE1_LEN_INTLDESC_A + TLE1_LEN_INTLDESC_B + TLE1_LEN_INTLDESC_C);
    
    unsigned int year = 0;
    double day = 0.0;
    
    extractInteger(m_line_one.substr(TLE1_COL_EPOCH_A,
                                     TLE1_LEN_EPOCH_A), year);
    extractDouble(m_line_one.substr(TLE1_COL_EPOCH_B,
                                    TLE1_LEN_EPOCH_B), 4, day);
    extractDouble(m_line_one.substr(TLE1_COL_MEANMOTIONDT2,
                                    TLE1_LEN_MEANMOTIONDT2), 2, m_mean_motion_dt2);
    extractExponential(m_line_one.substr(TLE1_COL_MEANMOTIONDDT6,
                                         TLE1_LEN_MEANMOTIONDDT6), m_mean_motion_ddt6);
    extractExponential(m_line_one.substr(TLE1_COL_BSTAR,
                                         TLE1_LEN_BSTAR), m_bstar);
    
    /*
     * line 2
     */
    extractDouble(m_line_two.substr(TLE2_COL_INCLINATION,
                                    TLE2_LEN_INCLINATION), 4, m_inclination);
    extractDouble(m_line_two.substr(TLE2_COL_RAASCENDNODE,
                                    TLE2_LEN_RAASCENDNODE), 4, m_right_ascending_node);
    extractDouble(m_line_two.substr(TLE2_COL_ECCENTRICITY,
                                    TLE2_LEN_ECCENTRICITY), -1, m_eccentricity);
    extractDouble(m_line_two.substr(TLE2_COL_ARGPERIGEE,
                                    TLE2_LEN_ARGPERIGEE), 4, m_argument_perigee);
    extractDouble(m_line_two.substr(TLE2_COL_MEANANOMALY,
                                    TLE2_LEN_MEANANOMALY), 4, m_mean_anomaly);
    extractDouble(m_line_two.substr(TLE2_COL_MEANMOTION,
                                    TLE2_LEN_MEANMOTION), 3, m_mean_motion);
    extractInteger(m_line_two.substr(TLE2_COL_REVATEPOCH,
                                     TLE2_LEN_REVATEPOCH), m_orbit_number);
    
    if (year < 57)
        year += 2000;
    else
        year += 1900;
    
    m_epoch = TimeOps::toJD(DateTime(year, day));
}

/**
 * Get the inclination
 * @param in_degrees Whether to return the value in degrees or radians
 * @returns the inclination
 */
double TLE::getInclination(ANGLE_TYPE _type) const {
    if (_type == DEGS) {
        return m_inclination;
    }
    else
    {
        return MathOps::toRadians(m_inclination);
    }
}

/**
 * Get the right ascension of the ascending node
 * @param in_degrees Whether to return the value in degrees or radians
 * @returns the right ascension of the ascending node
 */
double TLE::getRightAscendingNode(ANGLE_TYPE _type) const {
    if (_type == DEGS) {
        return m_right_ascending_node;
    }
    else {
        return MathOps::toRadians(m_right_ascending_node);
    }
}

/**
 * Get the argument of perigee
 * @param in_degrees Whether to return the value in degrees or radians
 * @returns the argument of perigee
 */
double TLE::getArgumentPerigee(ANGLE_TYPE _type) const {
    if (_type == DEGS) {
        return m_argument_perigee;
    }
    else {
        return MathOps::toRadians(m_argument_perigee);
    }
}

/**
 * Get the mean anomaly
 * @param in_degrees Whether to return the value in degrees or radians
 * @returns the mean anomaly
 */
double TLE::getMeanAnomaly(ANGLE_TYPE _type) const {
    if (_type == DEGS) {
        return m_mean_anomaly;
    }
    else {
        return MathOps::toRadians(m_mean_anomaly);
    }
}
