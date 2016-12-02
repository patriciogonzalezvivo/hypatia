#include "TimeOps.h"
#include "MathOps.h"

#include <math.h>
#include <ctime>
#include <chrono>

#include <string.h>
#include <iostream>

const double TimeOps::SECONDS_PER_DAY = 86400.0;
const double TimeOps::TROPICAL_YEAR = 365.242195601852;
const double TimeOps::JULIAN_EPOCH = 2440587.5;

const double TimeOps::SPEED_OF_LIGHT = 299792.458;
const double TimeOps::TO_CENTURIES = 36525.;

const double TimeOps::J2000 = 2451545.0;

const double TimeOps::HOURS_PER_DAY = 24.;
const int    TimeOps::IHOURS_PER_DAY = 24;

// Milliseconds elapsed since epoch (1 January 1970 00:00:00 UTC)
unsigned long TimeOps::getCurrentMilliseconds() {
    return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}

// Milliseconds elapsed since epoch (1 January 1970 00:00:00 UTC)
unsigned long TimeOps::getCurrentSeconds() {
    return std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1);
}


// To JulianDate
double TimeOps::julianDates (unsigned long _sec) {
    if (_sec == 0.0)
        _sec = getCurrentSeconds();
    
    return _sec / SECONDS_PER_DAY + 2440587.5;
}

// Milliseconds elapsed since epoch
// http://scienceworld.wolfram.com/astronomy/ModifiedJulianDate.html
double TimeOps::modifiedJulianDates (unsigned long _sec) {
    if (_sec == 0.0)
        _sec = getCurrentSeconds();
    return _sec / SECONDS_PER_DAY + 40587.0;
}

// Jordan Cosmological Theory??
double TimeOps::MJDtoJCT (double _mjd) {
    return (_mjd - 51544.5) / 36525.0;
}


double TimeOps::jordanCosmologicalTheory(unsigned long _sec) {
    return MJDtoJCT(modifiedJulianDates(_sec));
}

double fract (double _x) {
    return _x-floor(_x);
}

double mod (double _x, double _r) {
    return _r * fract(_x/_r);
}

// Greenwich Mean Sidereal Time
// https://www.cv.nrao.edu/~rfisher/Ephemerides/times.html
double TimeOps::MJDtoGMS(double _mjd) {
    double mjd0 = floor(_mjd);
    double ut = SECONDS_PER_DAY * (_mjd-mjd0);
    double t0 = MJDtoJCT(mjd0);
    double t = MJDtoJCT(_mjd);
    double gmst = 24110.54841 + 8640184.812866 * t0 + 1.0027379093 * ut + (0.093104 - (6.2e-6) * t) * t * t;
    return MathOps::TAU / SECONDS_PER_DAY * mod(gmst, SECONDS_PER_DAY);
}

double TimeOps::greenwichMeanSiderealTime(unsigned long _sec){
    return MJDtoGMS(modifiedJulianDates(_sec));
}

/*----------------------------------------------------------------------------
 *  See p 84,  in Meeus:
 * The following returns apparent Greenwich sidereal time for the given jd
 */
double TimeOps::greenwichSiderealTime ( double jd ) {
    jd -= J2000;      /* set relative to 2000.0 */
    double jdm = jd / TO_CENTURIES;  /* convert jd to julian centuries */
    double intPart = floor( jd );
    jd -= intPart;
    double rval =   280.46061837 +
    360.98564736629 * jd +
    .98564736629 * intPart +
    jdm * jdm * ( 3.87933e-4 - jdm / 38710000. );
    
    return MathOps::toRadians( rval );
}

double TimeOps::greenwichSiderealHour (double jd) {
    jd -= J2000;      /* set relative to 2000.0 */
    double jdm = jd / TO_CENTURIES;  /* convert jd to julian centuries */
    double intPart = floor( jd );
    jd -= intPart;
    double rval = 280.46061837 + 360.98564736629 * jd + .98564736629 * intPart + jdm * jdm * ( 3.87933e-4 - jdm / 38710000. );
    return (fmod(rval,360.)/360.)*24.;
}

double TimeOps::localSiderealTime (double _jd, double _lng_deg) {
    double gst24 = greenwichSiderealHour(_jd);
    double d = (gst24 + _lng_deg/15.0)/24.0;
    d = d - floor(d);
    if (d < 0.0)
        d += 1.;
    return 24.0*d;
}

