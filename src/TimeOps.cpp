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
const double TimeOps::MJULIAN_EPOCH = 40587.0;

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

// Julian Date to Modify Julian Date (http://tycho.usno.navy.mil/mjd.html)
double TimeOps::JDtoMJD(double _jd) {
    return _jd - 2400000.5;
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

/* given the modified Julian date (number of days elapsed since 1900 jan 0.5,),
 * mj, return the calendar date in months, *mn, days, *dy, and years, *yr.
 */
void TimeOps::MJDtoMDY(double _mjd, int &_month, double &_day, int &_year) {
    static double last_mj, last_dy;
    static int last_mn, last_yr;
    double d, f;
    double i, a, b, ce, g;

    /* we get called with 0 quite a bit from unused epoch fields.
     * 0 is noon the last day of 1899.
     */
    if (_mjd == 0.0) {
        _month = 12;
        _day = 31.5;
        _year = 1899;
        return;
    }

    if (_mjd == last_mj) {
        _month = last_mn;
        _year = last_yr;
        _day = last_dy;
        return;
    }

    d = _mjd + 0.5;
    i = floor(d);
    f = d-i;
    if (f == 1) {
        f = 0;
        i += 1;
    }

    if (i > -115860.0) {
        a = floor((i/36524.25)+.99835726)+14;
        i += 1 + a - floor(a/4.0);
    }

    b = floor((i/365.25)+.802601);
    ce = i - floor((365.25*b)+.750001)+416;
    g = floor(ce/30.6001);
    _month = (int)(g - 1);
    _day = ce - floor(30.6001*g)+f;
    _year = (int)(b + 1899);

    if (g > 13.5) {
        _month = (int)(g - 13);
    }
    if (_month < 2.5) {
        _year = (int)(b + 1900);
    }
    if (_year < 1) {
        _year -= 1;
    }

    last_mn = _month;
    last_dy = _day;
    last_yr = _year;
    last_mj = _mjd;
}

/* given a date in months, mn, days, dy, years, yr,
 * return the modified Julian date (number of days elapsed since 1900 jan 0.5),
 * *mjd.
 */
double TimeOps::MDYtoMJD (int mn, double dy, int yr) {
    static double last_mjd, last_dy;
    static int last_mn, last_yr;
    int b, d, m, y;
    long c;
    double mjp;

    if (mn == last_mn && yr == last_yr && dy == last_dy) {
        mjp = last_mjd;
        return mjp;
    }

    m = mn;
    y = (yr < 0) ? yr + 1 : yr;
    if (mn < 3) {
        m += 12;
        y -= 1;
    }

    if (yr < 1582 || (yr == 1582 && (mn < 10 || (mn == 10 && dy < 15))))
        b = 0;
    else {
        int a;
        a = y/100;
        b = 2 - a + a/4;
    }

    if (y < 0)
        c = (long)((365.25*y) - 0.75) - 694025L;
    else
        c = (long)(365.25*y) - 694025L;

    d = (int)(30.6001*(m+1));

    mjp = b + c + d + dy - 0.5;

    last_mn = mn;
    last_dy = dy;
    last_yr = yr;
    last_mjd = mjp;

    return mjp;
}

/* given a mjd, return the year as a double. */
double TimeOps::MJDtoYears (double _mjd) {
    static double last_mj, last_yr;
    int m, y;
    double d;
    double e0, e1;  /* mjd of start of this year, start of next year */

    if (_mjd == last_mj) {
        return last_yr;
    }

    MJDtoMDY(_mjd, m, d, y);
    if (y == -1) {
        y = -2;
    }
    e0 = MDYtoMJD (1, 1.0, y);
    e1 = MDYtoMJD (1, 1.0, y+1);

    last_mj = _mjd;
    last_yr = y + (_mjd - e0)/(e1 - e0);
    return last_yr;
}

// https://quasar.as.utexas.edu/BillInfo/JulianDatesG.html
void TimeOps::JDtoMDY(double _jd, int &_month, double &_day, int &_year) {
    double Q = _jd + 0.5;
    int Z = int(Q);
    int W = (Z - 1867216.25)/36524.25;
    int X = W / 4;
    int A = Z + 1 + W - X;
    int B = A + 1524;
    int C = (B - 122.1) / 365.25;
    int D = 365.25 * C;
    int E = (B - D) / 30.6001;
    int F = 30.6001 * E;
    
    _day = B-D-F+(Q-Z);
    _month = int(E-1)%12;
    if (_month == 0) {
        _month = 12;
    }
    if (_month < 3) {
        _year = C-4715;
    }
    else {
        _year = C-4716;
    }
}

int TimeOps::MJDtoDOW (double _mjd) {
    /* cal_mjd() uses Gregorian dates on or after Oct 15, 1582.
     * (Pope Gregory XIII dropped 10 days, Oct 5..14, and improved the leap-
     * year algorithm). however, Great Britian and the colonies did not
     * adopt it until Sept 14, 1752 (they dropped 11 days, Sept 3-13,
     * due to additional accumulated error). leap years before 1752 thus
     * can not easily be accounted for from the cal_mjd() number...
     */
    if (_mjd < -53798.5) {
        /* pre sept 14, 1752 too hard to correct |:-S */
        return (-1);
    }
    int dow = ((long)floor(_mjd-.5) + 1) % 7;/* 1/1/1900 (mj 0.5) is a Monday*/
    if (dow < 0)
        dow += 7;
    return dow;
}
