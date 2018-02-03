#include "TimeOps.h"
#include "MathOps.h"

#include <math.h>
#include <ctime>
#include <chrono>

#include <string.h>
#include <iostream>

const double TimeOps::HOURS_PER_DAY         = 24.;
const double TimeOps::DAYS_PER_HOUR         = 1./HOURS_PER_DAY;
const double TimeOps::SECONDS_PER_MINUTE    = 60.;
const double TimeOps::MINUTES_PER_HOUR      = 60.;
const double TimeOps::SECONDS_PER_HOUR      = SECONDS_PER_MINUTE * MINUTES_PER_HOUR;
const double TimeOps::MINUTES_PER_DAY       = HOURS_PER_DAY * MINUTES_PER_HOUR;
const double TimeOps::SECONDS_PER_DAY       = MINUTES_PER_DAY * SECONDS_PER_MINUTE;

const int    TimeOps::IHOURS_PER_DAY        = int(HOURS_PER_DAY);
const int    TimeOps::ISECONDS_PER_MINUTE   = int(SECONDS_PER_MINUTE);
const int    TimeOps::IMINUTES_PER_HOUR     = int(MINUTES_PER_HOUR);
const int    TimeOps::ISECONDS_PER_HOUR     = int(SECONDS_PER_HOUR);

const double TimeOps::TROPICAL_YEAR = 365.242195601852;
const double TimeOps::JULIAN_EPOCH = 2440587.5;
const double TimeOps::MJULIAN_EPOCH = 40587.0;

const double TimeOps::DAYS_PER_CENTURY    = 36525.;
const double TimeOps::DAYS_PER_MILLENIUM  = 365250.;

const double TimeOps::J2000 = 2451545.0;
const double TimeOps::JD_DIFF = 0.5;

const double TimeOps::DST_OFFSET = DAYS_PER_HOUR;

static char* MONTH[] = { (char*)"", /* month 0 */
    (char*)"January", (char*)"February", (char*)"March", (char*)"April", (char*)"May", (char*)"June",
    (char*)"July", (char*)"August", (char*)"September", (char*)"October", (char*)"November", (char*)"December"
};

static char* MONTH3[] = { (char*)"", /* month 0 */
    (char*)"Jan", (char*)"Feb", (char*)"Mar", (char*)"Apr", (char*)"May", (char*)"Jun", (char*)"Jul", (char*)"Aug", (char*)"Sep", (char*)"Oct", (char*)"Nov", (char*)"Dec"
};

char* TimeOps::getMonth( int _month ) {
    return MONTH[_month];
}

char* TimeOps::getMonthAbbreviation( int _month ){
    return MONTH3[_month];
}

/*----------------------------------------------------------------------------
 * greenwichSiderealTime (See p 84,  in Meeus)
 *
 * @param jd - julian day
 *
 * @return apparent Greenwich sidereal time (in Radians) for the given jd
 *
 * See p 84,  in Meeus
 */
double TimeOps::greenwichSiderealTime( double jd ) {
    jd -= TimeOps::J2000;
    double jdc = jd / TimeOps::DAYS_PER_CENTURY;      /* convert jd to julian centuries */
    double intPart = floor( jd );
    double fracPart = jd - intPart;
    double rval = 280.46061837 +
    360.98564736629 * fracPart +
    .98564736629 * intPart +
    jdc * jdc * ( 3.87933e-4 - jdc / 38710000. );
    
    return MathOps::toRadians( rval );
}

//----------------------------------------------------------------------------
/**
 * formatTime(): format a time into an HH:MM or HH:MM:SS string
 *
 * @param buf - where to put the string
 * @param dayFrac - a fractional day ( >= 0.0, < 1.0 )
 * @param doSecs - true to include seconds
 */
int TimeOps::formatTime( char* buf, double dayFrac, bool doSecs ) {
    if (!buf)
        return 0;
    
    int h, m, s, rv = 0;
    dayToHMS(dayFrac, h, m, s);
    
    if ( doSecs )
        sprintf( buf, "%02d:%02d:%02d", h, m, int(s) );
    else {
        rv = roundToNearestMinute( h, m, s );
        sprintf( buf, "%02d:%02d", h, m );
    }
    return rv;
}


/**
 * formatTime(): format a time into an HH:MM string, and write it to a FILE
 *
 * @param fp - where to write the string
 * @param dayFrac - a fractional day ( >= 0.0, < 1.0 )
 * @param doSecs - true to include seconds (HH:MM:SS)
 */
int TimeOps::formatTime( FILE* fp, double dayFrac, bool doSecs ) {
    if (!fp)
        return 0;
    
    int h = 0;
    int m, s, rv = 0;
    dayToHMS(dayFrac, h, m, s);
    
    if ( doSecs )
        fprintf( fp, "%02d:%02d:%02d", h, m, s );
    else {
        rv = roundToNearestMinute( h, m, s );
        fprintf( fp, "%02d:%02d", h, m );
    }
    return rv;
}

/**
 * formatMS(): format a fractional minute into a text string (MM:SS.S)
 *
 * @param buf - where to put the formatted string
 * @param m  - the value (in minutes) to format
 */
void TimeOps::formatMS( char* buf, double min ) {
    sprintf( buf, "%02d:%02.1f\n",
            int(min), (min - int(min)) * TimeOps::SECONDS_PER_MINUTE );
}

/**
 * formatMS(): format a fractional minute into a text string (MM:SS.S) and
 *             write it the spec'd file.
 *
 * @param fp - where to write the formatted string
 * @param m  - the value (in minutes) to format
 */
void TimeOps::formatMS( FILE* fp, double min ) {
    fprintf( fp, "%02d:%02.1f\n",
            int(min), (min - int(min)) * TimeOps::SECONDS_PER_MINUTE );
}

/**
 * timeToLDay(): convert a "local" time_t to Julian Day
 *
 * @param time - time_t to convert
 *
 * @return Julian Day
 */
double TimeOps::timeToLDay( time_t time ) {
    return timeToDay( localtime(&time) );
}

/**
 * timeToUDay(): convert UTC time_t to Julian Day
 *
 * @param time_t to convert
 *
 * @return Julian Day
 */
double TimeOps::timeToUDay( time_t time ) {
    return timeToDay( gmtime(&time) );
}

//----------------------------------------------------------------------------
/**
 * formatDateTime(): format a JD into a text string
 *
 * @param buf - where to put the fomatted string, must be > 12 chars long
 * @param jd
 * @param format type (DateOps::DATE_FMT)
 */
void TimeOps::formatDateTime( char* clientBuf, double jd, DATE_FMT fmt ) {
    if ( !clientBuf ) return;
    
    int d, m, y;
    char tbuf[16] = { 0 };
    
    if ( fmt >= Y_MON_D_HM )
        jd += formatTime(tbuf, jd);
    
    TimeOps::JDtoDMY( jd, d, m, y );
    
    switch (fmt) {
            // date only
        case Y_MON_D:
            sprintf ( clientBuf, "%04d %s %02d", y, MONTH3[m], d );
            break;
        case MON_D_Y:
            sprintf ( clientBuf, "%s %02d %04d", MONTH3[m], d, y );
            break;
        case MON_D:
            sprintf ( clientBuf, "%s %02d", MONTH3[m], d );
            break;
        case Y_M_D:
            sprintf ( clientBuf, "%04d-%02d-%02d", y, m, d );
            break;
        case M_D_Y:
            sprintf ( clientBuf, "%02d/%02d/%04d", m, d, y );
            break;
        case M_D:
            sprintf ( clientBuf, "%02d/%02d", m, d );
            break;
            // date + time
        case Y_MON_D_HM:
            sprintf ( clientBuf, "%04d %s %02d %s", y, MONTH3[m], d, tbuf );
            break;
        case MON_D_Y_HM:
            sprintf ( clientBuf, "%s %02d %04d %s", MONTH3[m], d, y, tbuf );
            break;
        case MON_D_HM:
            sprintf ( clientBuf, "%s %02d %s", MONTH3[m], d, tbuf );
            break;
        case Y_M_D_HM:
            sprintf ( clientBuf, "%04d-%02d-%02d %s", y, m, d, tbuf );
            break;
        case M_D_Y_HM:
            sprintf ( clientBuf, "%02d/%02d/%04d %s", m, d, y, tbuf );
            break;
        case M_D_HM:
            sprintf ( clientBuf, "%02d/%02d %s", m, d, tbuf );
            break;
            
        default:
            *clientBuf = 0;
    };
    
}

/**
 * formatDateTime(): format a JD into a text string and write it to a FILE
 *
 * @param fp - where to write the formatted string
 * @param jd  - the day to format
 * @param fmt - format type (see DateOps::DATE_FMT)
 */
void TimeOps::formatDateTime( FILE* fp, double jd, DATE_FMT fmt ) {
    char buf[32];
    formatDateTime( buf, jd, fmt );
    fputs( buf, fp );
}


//----------------------------------------------------------------------------
/**
 * timeToDay() - convert a struct tm to Julian Day
 */
double TimeOps::timeToDay( struct tm* pt )
{
    int secs = pt->tm_hour * TimeOps::ISECONDS_PER_HOUR +
    pt->tm_min * TimeOps::ISECONDS_PER_MINUTE +
    pt->tm_sec;
    return double(secs)/TimeOps::SECONDS_PER_DAY +
    DMYtoJD( pt->tm_mday,
             pt->tm_mon + 1,        // months are zero-based
             pt->tm_year + 1900 );  // years from 1900
}


//----------------------------------------------------------------------------
/**
 * dayToTime(): convert a JD to "local" time_t
 *
 * @param jd - jd to convert
 *
 * @return time_t
 */
time_t TimeOps::dayToTime( double jd ) {
    struct tm t;
    t.tm_isdst=-1;
    
    TimeOps::JDtoDMY( long(floor(jd)), t.tm_mday, t.tm_mon, t.tm_year );
    t.tm_mon--;
    t.tm_year -= 1900;
    
    dayToHMS(jd, t.tm_hour, t.tm_min, t.tm_sec);
    return mktime( &t );
}

//----------------------------------------------------------------------------
/*
 * dayToDmy()
 *
 * Estimates the year corresponding to an input JD and calls get_calendar_data();
 * for that year.  Occasionally,  it will find that the guesstimate was off;
 * in such cases,  it moves ahead or back a year and tries again.  Once it's
 * done,  jd - year_ends[0] gives the number of days since New Years Day;
 * by subtracting month_data[] values,  we quickly determine which month and
 * day of month we're in.
 */

// calendar: 0 = gregorian, 1 = julian
void TimeOps::JDtoDMY( double jd, int& _day, int& _month, int& _year, CalendarType calendar ) {
    return JDtoDMY( long(floor(jd)), _day, _month, _year, calendar);
}
void TimeOps::JDtoDMY( long jd, int& day, int& month, int& year, CalendarType calendar ) {
    day = -1;           /* to signal an error */
    
    YearEndDays yed;
    MonthDays md;
    year = TimeOps::getCalendarYear(jd, calendar);
    do {
        if( 0 != TimeOps::getCalendarData( year, yed, md, calendar ) )
            return;
        
        if( yed[0] > jd)
            year--;
        
        if( yed[1] <= jd)
            year++;
        
    } while( yed[0] > jd || yed[1] <= jd );
    
    long currJd = yed[0];
    month = -1;
    for( int i = 0; i < 13; i++) {
        day = int( jd - currJd );
        if( day < md[i] ) {
            month = i + 1;
            day++;
            return;
        }
        currJd += long( md[i] );
    }
    return;
}

//----------------------------------------------------------------------------
// Determine U.S. DST start JD (second Sunday in March as of 2007)
//
long TimeOps::dstStart(int year) {
    long jdStart = DMYtoJD( 7+1, 3, year, T_GREGORIAN );
    while ( 6 != (jdStart % 7 ) ) // Sunday
        jdStart++;
    
    return jdStart;
}

//----------------------------------------------------------------------------
// Determine U.S. DST end JD (first Sunday in November as of 2007)
//
long TimeOps::dstEnd(int year) {
    long jdEnd = DMYtoJD( 1, 11, year, T_GREGORIAN );
    while ( 6 != (jdEnd % 7 ) ) // Sunday
        jdEnd++;
    
    return jdEnd;
}

//----------------------------------------------------------------------------
/**
 * tzOffsetInDays(): calculate time zone offset from Universal Time in days
 *                   using the given time_t (includes DST)
 *
 * @return Offset (-0.5 ... +0.5 )
 */
double TimeOps::tzOffsetInDays(time_t tt) {
    struct tm* pt = localtime(&tt);
    
    int localH = pt->tm_hour;
    pt = gmtime(&tt);
    
    int gmtH = pt->tm_hour;
    if ( gmtH < localH )
        gmtH += TimeOps::IHOURS_PER_DAY;
    
    return hourToDay( localH - gmtH );  // local - GMT
}

//----------------------------------------------------------------------------
/**
 * dstOffsetInDays(): calculate daylight savings time offset in days at
 *                    the spec'd time
 *
 * @return Offset ( 0 or 1/24 )
 */
double TimeOps::dstOffsetInDays(time_t tt) {
    
    struct tm* pt = localtime(&tt);
    return (pt->tm_isdst > 0) ? DST_OFFSET : 0.;
}

//----------------------------------------------------------------------------
/**
 * dayToHMS(): break the fractional part of a Julian day into hours, minutes,
 * and seconds
 *
 * @param dayFrac - a fractional day ( >= 0.0, < 1.0 )
 * @param h - where to put the hour
 * @param m - where to put the minute
 * @param s - where to put the second
 *
 */
void TimeOps::dayToHMS( double jd, int& h, int& m, int& s) {
    
    if (jd < 0.)
        h = m = s = 0;
    else {
        double fd = jd - floor(jd);  // get rid of whole days, if any
        
        fd *= TimeOps::HOURS_PER_DAY;
        h  = int(fd);
        fd -= h;
        
        fd *= TimeOps::MINUTES_PER_HOUR;
        m  = int(fd);
        fd -= m;
        
        s = int( (fd * TimeOps::SECONDS_PER_MINUTE) + MathOps::ROUND );
    }
}

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

double TimeOps::greenwichSiderealHour (double jd) {
    jd -= J2000;      /* set relative to 2000.0 */
    double jdm = jd / DAYS_PER_CENTURY;  /* convert jd to julian centuries */
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

//----------------------------------------------------------------------------
/**
 * private
 * roundToNearestMinute(): round a time to the nearest minute
 *
 * if h:m wraps to zero, we roll back to 23:59 to preserve "today"
 *
 * @param h - hour
 * @param m - minute
 * @param s - second
 *
 * @return 1 if rounding wrapped to 00:00, else 0.
 */
int TimeOps::roundToNearestMinute(int &h, int& m, int& s) {
    
    int rv = 0;
    if ( s >= TimeOps::ISECONDS_PER_MINUTE/2 ) { // round to nearest minute
        m++;
        if ( m >= TimeOps::IMINUTES_PER_HOUR ) {
            m = 0;
            h++;
            if ( h >= TimeOps::IHOURS_PER_DAY ) {
                h = 0;
                rv = 1;
            }
        }
    }
    return rv;
}


//----------------------------------------------------------------------------

/* General calendrical comments:
 
 This code supports conversions between JD and five calendrical systems:
 Julian,  Gregorian,  Hebrew,  Islamic,  and (French) revolutionary.
 Comments pertaining to specific calendars are found near the code for
 those calendars.
 
 For each calendar,  there is a "get_(calendar_name)_year_data( )" function,
 used only within this source code. This function takes a particular year
 number,  and computes the JD corresponding to "new years day" (first day of
 the first month) in that calendar in that year.  It also figures out the
 number of days in each month of that year,  returned in the array
 month_data[].  There can be up to 13 months,  because the Hebrew calendar
 (and some others that may someday be added) can include an "intercalary
 month".  If a month doesn't exist,  then the month_data[] entry for it
 will be zero (thus,  in the Gregorian and Julian and Islamic calendars,
 month_data[12] is always zero,  since these calendars have only 12 months.)
 
 The next level up is the get_calendar_data( ) function,  which (through
 the wonders of a switch statement) can get the JD of New Years Day and
 the array of months for any given year for any calendar.  Above this
 point,  all calendars can be treated in a common way;  one is shielded
 from the oddities of individual calendrical systems.
 
 Finally,  at the top level,  we reach the only two functions that are
 exported for the rest of the world to use:  dmy_to_day( ) and day_to_dmy( ).
 The first takes a day,  month,  year,  and calendar system.  It calls
 get_calendar_data( ) for the given year,  adds in the days in the months
 intervening New Years Day and the desired month,  and adds in the day
 of the month,  and returns the resulting Julian Day.
 
 day_to_dmy( ) reverses this process.  It finds an "approximate" year
 corresponding to an input JD,  and calls get_calendar_data( ) for
 that year.  By adding all the month_data[] values for that year,  it
 can also find the JD for the _end_ of that year;  if the input JD is
 outside that range,  it may have to back up a year or add in a year.
 Once it finds "JD of New Years Day < JD < JD of New Years Eve",  it's
 a simple matter to track down which month and day of the month corresponds
 to the input JD.
 */

int TimeOps::getCalendarYear(long jd, CalendarType calendar) {
    int year = -1;
    switch( calendar) {
        case T_GREGORIAN:
        case T_JULIAN:
            year = (jd - E_JULIAN_GREGORIAN) / 365L;
            break;
#if defined( CALENDARS_OF_THE_WORLD )
        case T_HEBREW:
            year = (jd - E_HEBREW) / 365L;
            break;
        case T_ISLAMIC:
            year = (jd - E_ISLAMIC) / 354L;
            break;
        case T_REVOLUTIONARY:
            year = (jd - E_REVOLUTIONARY) / 365L;
            break;
        case T_PERSIAN:
            year = (jd - JALALI_ZERO) / 365L;
            if( year < LOWER_PERSIAN_YEAR)
                year = LOWER_PERSIAN_YEAR;
            if( year > UPPER_PERSIAN_YEAR)
                year = UPPER_PERSIAN_YEAR;
            break;
#endif
        default:       /* undefined calendar */
            break;
    }  // end switch()
    
    return year;
}


/*
 *  Begin:  Gregorian and Julian calendars (combined for simplicity)
 *
 *  It's common to implement Gregorian/Julian calendar code with the
 * aid of cryptic formulae,  rather than through simple lookup tables.
 * For example,  consider this formula from Fliegel and Van Flandern,
 * to convert Gregorian (D)ay, (M)onth, (Y)ear to JD:
 *
 * JD = (1461*(Y+4800+(M-14)/12))/4+(367*(M-2-12*((M-14)/12)))/12
 *       -(3*((Y+4900+(M-14)/12)/100))/4+D-32075
 *
 * The only way to verify that they work is to feed through all possible
 * cases.  Personally,  I like to be able to look at a chunk of code and
 * see what it means.  It should resemble the Reformation view of the
 * Bible:  anyone can read it and witness the truth thereof.
 */

//----------------------------------------------------------------------------
void TimeOps::getJulGregYearData( int year, long& daysInYear, MonthDays& md, bool julian ) {
    static const MonthDays months =
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0 };
    
    if( year >= 0L ) {
        daysInYear = year * 365L + year / 4L;
        if( !julian )
            daysInYear += -year / 100L + year / 400L;
    }
    else {
        daysInYear = (year * 365L) + (year - 3L) / 4L;
        if( !julian )
            daysInYear += -(year - 99L) / 100L + (year - 399L) / 400L;
    }
    
    if( julian )
        daysInYear -= 2L;
    
    memcpy( &md, months, sizeof(MonthDays) );
    
    if( !(year % 4)) {
        if( (year % 100L) || !(year % 400L) || julian ) {
            md[1] = 29;
            daysInYear--;
        }
    }
    daysInYear += E_JULIAN_GREGORIAN + 1;
}

//----------------------------------------------------------------------------
int TimeOps::getCalendarData( int year, YearEndDays& days, MonthDays& md, CalendarType calendar) {
    int rval = 0;
    
    memset( &md, 0, sizeof(MonthDays) );
    switch( calendar)
    {
        case T_GREGORIAN:
        case T_JULIAN:
            getJulGregYearData( year, days[0], md, (T_JULIAN == calendar) );
            break;
#if defined( CALENDARS_OF_THE_WORLD )
        case T_ISLAMIC:
            getIslamicYearData( year, days[0], md );
            break;
        case T_HEBREW:
            getHebrewYearData( year, days, md );
            break;
        case T_REVOLUTIONARY:
            getRevolutionaryYearData( year, days, md );
            break;
        case T_PERSIAN:
            if( year >= LOWER_PERSIAN_YEAR && year <= UPPER_PERSIAN_YEAR)
                getJalaliYearData( year, days, md );
            else
                rval = -1;
            break;
#endif
        default:
            rval = -1;
            break;
    }
    if( 0 == rval ) {
        //
        // days[1] = JD of "New Years Eve" + 1;  that is, New Years Day of the
        // following year.  If you have days[0] <= JD < days[1],  JD is in the
        // current year.
        //
        days[1] = days[0];
        for( int i=0; i<13; i++ )
            days[1] += md[i];
    }
    return( rval );
}


#if defined( CALENDARS_OF_THE_WORLD )

/* The following mod( ) function returns the _positive_ remainder after */
/* a division.  Annoyingly,  if x < 0,  then x % y <= 0;  thus,  this   */
/* function is needed for things such as determining a day of the week. */

long TimeOps::mod( long x, long y ) {
    long rval = x % y;
    
    if( rval < 0L )
        rval += y;
    
    return rval;
}

void TimeOps::getIslamicYearData( long year, long& daysInYear, MonthDays& md )
// long *days, char *month_data)
{
    static const long THIRTY_ISLAMIC_YEARS = 10631L;
    
    static const int isIslamicLeapYear[30] = {
        0, 0, 1, 0, 0, 1, 0, 1, 0, 0,
        1, 0, 0, 1, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 0, 1, 0, 1, 0, 0, 1 };
    
    long yearWithinCycle = mod( year, 30L );
    long thirtyYearCycles = (year - yearWithinCycle) / 30L;
    long rval = E_ISLAMIC +
    thirtyYearCycles * THIRTY_ISLAMIC_YEARS +
    yearWithinCycle * 354L;
    
    md[12] = 0;
    md[11] = 29 + isIslamicLeapYear[yearWithinCycle];
    while( yearWithinCycle-- )
        rval += long( isIslamicLeapYear[yearWithinCycle] );
    
    daysInYear = rval;
    
    /* The Islamic calendar alternates between 30-day and 29-day */
    /* months for the first eleven months;  the twelfth is 30    */
    /* days in a leap year,  29 otherwise (see above).           */
    
    for( int i=0; i<11; i++ )
        md[i] = 30 - (i % 2);
}

/* End:  Islamic calendar */


/* Begin:  Hebrew calendar */

/* See p 586,  _Explanatory Supplement_,  for explanation. */
/* There are 1080 Halakim,  or 'parts',  each 3.33 seconds long,  in  */
/* an hour.  So: */

static const int isHebrewLeapYear[19] = { 0, 0, 1, 0, 0, 1,
    0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1 };

long TimeOps::lunationsToTishri1( long year ) {
    long yearWithinCycle = mod( year - 1, 19L );
    
    long fullNineteenYearCycles = ( year - 1 - yearWithinCycle ) / 19L;
    
    long rval = fullNineteenYearCycles * 235L + yearWithinCycle * 12L;
    
    for( int y=0; y<yearWithinCycle; y++ )
        rval += isHebrewLeapYear[y];
    
    return rval;
}

void TimeOps::lunationsToDaysAndHalakim( long lunations, long& days, long& halakim ) {
    /*****
     One lunation is 29 days, 13753 halakim long.  To evade integer
     overflow for immense numbers of lunations,  we can use the fact
     that 25920 lunations is exactly 765433 days.  This cycle has no
     name that I know of,  and no real significance outside of this
     particular function... but it _does_ let us write simpler code
     that won't get wrong answers for large or negative numbers of
     lunations.  Let's call 25920 lunations a "glumph."  We figure
     out how many glumphs have passed and our location within that
     glumph,  and the rest is easy.
     *****/
    
    long lunationWithinGlumph = mod( lunations, 25920L );
    
    long currGlumph = ( lunations - lunationWithinGlumph ) / 25920L;
    
    days += currGlumph * 765433L + lunationWithinGlumph * 29L;
    halakim += lunationWithinGlumph * 13753L;
    
    // Now make sure excess halakim carry over correctly
    days += halakim / HALAKIM_IN_DAY;
    halakim %= HALAKIM_IN_DAY;
}

/* Set days and halakim to the 'epoch':  1 Tishri 1 = 2 5604 */
void TimeOps::findTishri1( long year, long& days, long& halakim ) {
    days = 2L;
    halakim = 5604L;
    lunationsToDaysAndHalakim( lunationsToTishri1( year), days, halakim );
}

/* Warning: Certain aspects of getHebrewYearData( ) will definitely fail for
 years before zero... something will have to be done about that.
 */

void TimeOps::getHebrewYearData( long year, YearEndDays& daysInYear, MonthDays& md ) {
    for( int i=0; i<2; i++ ) {
        long day, halakim;
        
        findTishri1( year + i, day, halakim );
        
        /* Check dehiyyah (c): */
        if( 3 == mod( day, 7L ) &&
           halakim >= 9L * 1080L + 204L &&
           !isHebrewLeapYear[ mod( year - 1 + i, 19L) ]
           )
        {
            day += 2;
        }
        /* Check dehiyyah (d): */
        else if( mod( day, 7L) == 2 &&
                halakim >= 15L * 1080L + 589L &&
                isHebrewLeapYear[ mod( year - 2 + i, 19L) ]
                )
        {
            day++;
        }
        else
        {
            if( halakim > 18L * 1080L )
                day++;
            
            if( mod( day, 7L ) == 1 ||
               mod( day, 7L ) == 4 ||
               mod( day, 7L ) == 6L
               )
            {
                day++;
            }
        }
        daysInYear[i] = day + E_HEBREW;
    }
    
    int yearLength = int( daysInYear[1] - daysInYear[0] );
    
    if( 0 != md[0] ) {
        for( int i=0; i<6; i++ )                 /* "normal" lengths */
            md[i] = md[i + 7] = (char)( 30 - (i & 1));
        
        if( isHebrewLeapYear[ mod( year - 1, 19L) ] ) {
            md[5] = 30;     /* Adar I is bumped up a day in leap years */
            md[6] = 29;
        }
        else                       /* In non-leap years,  Adar II doesn't    */
            md[6] = 0;      /* exist at all;  set it to zero days     */
        
        if( yearLength == 353 || yearLength == 383 )      /* deficient year */
            md[2] = 29;
        
        if( yearLength == 355 || yearLength == 385 )      /* complete year  */
            md[1] = 30;
    }
}

/*  Some test cases:  16 Av 5748 AM (16 12 5748) = 30 Jul 1988 Gregorian */
/*                 14 Nisan 5730 AM (14 8 5730) = 20 Apr 1970 Gregorian */
/*                  1 Tishri 5750 AM (1 1 5750) = 30 Sep 1989 Gregorian */

/* End:  Hebrew calendar */

/* Begin:  (French) Revolutionary calendar */

/*
 The French Revolutionary calendar is simplest,  in some respects;
 you just have twelve months,  each of 30 days,  with five or six
 "unattached" days at the end of the year.  Leap years are those
 divisible by four,  except for those divisible by 128.  This slight
 deviation from the Gregorian scheme,  of "divisible by four,  unless
 divisible by 100,  unless divisible by 400",  is slightly simpler
 and gives a calendar that is _much_ closer to the true tropical year.
 
 Unfortunately,  when first devised,  the French attempted to have
 New Years Day line up with the autumn equinox,  which is not
 particularly regular.  Thus, between 1 AR and 20 AR,  leap years
 occurred a year early;  i.e, years 3, 7, 11,  and 15 AR were leap
 years;  after that,  they were supposed to revert to the rule
 described above.  (There are also claims that leap years were to
 follow the Gregorian "4, 100, 400" rule.  I have no real evidence
 to support one scheme over the other.  But I suspect that a revolution
 so devoted to revising every aspect of human existence that it
 changed names of all months,  "regularized" each to be 30 days,  and
 made a week ten days long,  probably went out of its way not to
 resemble earlier calendars proposed by a Pope.  Also,  the fact that
 it would be an almost perfect match to the tropical year would lend
 support to the scheme.  Finally,  the irony of the Republic creating
 a calendar that would be good for a hundred thousand years appeals
 to me,  considering the short life of the Republic itself.  Those
 objecting to my choice are free to #define GREGORIAN_REVOLUTIONARY.)
 
 A 'proleptic' calendar wasn't defined,  to my knowledge...
 I've added one based on the logical assumption that zero,  and all
 "BR" (Before the Revolution) years divisible by four and not by 128,
 are leap;  so (as with all other calendars in this code) negative
 years are correctly supported.
 
 */

long TimeOps::jdOfFrenchRevYear( long year ) {
    long rval = E_REVOLUTIONARY + year * 365L;
    
    if( year >= 20)
        year--;
    
#ifdef GREGORIAN_REVOLUTIONARY
    rval += long(year / 4 - year / 100 + year / 400);
#else
    rval += long(year / 4 - year / 128);
#endif
    
    if( year <= 0L )
        rval--;
    
    return rval;
}

void TimeOps::getRevolutionaryYearData( long year, YearEndDays& daysInYear, MonthDays& md ) {
    daysInYear[0] = jdOfFrenchRevYear( year );
    daysInYear[1] = jdOfFrenchRevYear( year + 1 );
    
    /* There are twelve months of 30 days each,  followed by
     * five (leap years,  six) days;  call 'em an extra
     * thirteenth "month",  containing all remaining days:
     */
    for( int i=0; i<12; i++ )
        md[i] = 30;
    
    md[12] = ( daysInYear[1] - daysInYear[0] - 360L );
}

/* End:  (French) Revolutionary calendar */

/* Begin:  Persian (Jalaali) calendar */

long TimeOps::jalaliJd0( long jalaliYear ) {
    static const int breaks[12] = { -708, -221,   -3,    6,  394,  720,
        786, 1145, 1635, 1701, 1866, 2328 };
    static const int deltas[12] = { 1108, 1047,  984, 1249,  952,  891,
        930,  866,  869,  844,  848,  852 };
    long rval = -1L;
    
    if( jalaliYear >= LOWER_PERSIAN_YEAR ) {
        
        for( int i = 0; i < 12; i++ ) {
            if( jalaliYear < breaks[i] )
            {
                rval = JALALI_ZERO + jalaliYear * 365L +
                ( deltas[i] + jalaliYear * 303L ) / 1250L;
                
                if( i < 3 )  /* zero point drops one day in first three blocks */
                    rval--;
                
                break;
            }
        }
    }
    return rval;
}

void TimeOps::getJalaliYearData( long year, YearEndDays& daysInYear, MonthDays& md ) {
    if( year < LOWER_PERSIAN_YEAR || year > UPPER_PERSIAN_YEAR)
        return;
    
    daysInYear[0] = jalaliJd0( year) + 1L;
    daysInYear[1] = jalaliJd0( year + 1L ) + 1L;
    
    /* The first six months have 31 days.
     * The next five have 30 days.
     * The last month has 29 days in ordinary years,  30 in leap years.
     */
    for( int i=0; i<6; i++ )
        md[i] = 31;
    
    for( int i=6; i<11; i++ )
        md[i] = 30;
    
    md[11] = ( daysInYear[1] - daysInYear[0] - 336L );
}

/* End:  Persian (Jalali) calendar */

#endif  /* #if defined( CALENDARS_OF_THE_WORLD ) */


