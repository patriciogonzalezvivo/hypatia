#include "TimeOps.h"

#include <math.h>
#include <string.h>

#include <ctime>
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

static char* DOW[] = { 
    (char*)"Sunday", (char*)"Monday", (char*)"Tuesday", (char*)"Wednesday", (char*)"Thursday", (char*)"Friday",(char*)"Saturday"
};

static char* DOW3[] = { 
    (char*)"Sun", (char*)"Mon", (char*)"Tue", (char*)"Wed", (char*)"Thu", (char*)"Fri",(char*)"Sat"
};

static int daysInMonth[2][13] = {
    //  1   2   3   4   5   6   7   8   9   10  11  12
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

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
int roundToNearestMinute(int& h, int& m, int& s) {
    
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

char* TimeOps::getMonth( int _month ) {
    return MONTH[_month];
}

char* TimeOps::getMonthAbbreviation( int _month ){
    return MONTH3[_month];
}

char* TimeOps::getDOW( int _dow ) {
    return DOW[_dow];
}

char* TimeOps::getDOWAbbreviation( int _dow ){
    return DOW3[_dow];
}

//----------------------------------------------------------------------------
/**
 * now(): Determine the Julian Day value at the present moment
 *
 * @param local - true for local time, false for UTC
 *
 * @return The Julian Day value
 */
double TimeOps::now( TIME_TYPE _type ) {
    return toJD( time(0), _type ) - TimeOps::JD_DIFF;
}

//----------------------------------------------------------------------------

/**
 * toJD(): convert a seconds to Julian Century
 *
 * @param seconds (time stamp since epoch )
 *
 * @return Julian Date
 */
double TimeOps::toJD( unsigned long _sec ) {
    return _sec / SECONDS_PER_DAY + JULIAN_EPOCH;
}

//----------------------------------------------------------------------------

/**
 * toJD(): convert UTC time_t to Julian Day
 *
 * @param time_t to convert
 * @param time type (UTC/LOCAL)
 *
 * @return Julian Day
 */
double TimeOps::toJD( time_t time, TIME_TYPE _type ) {
    if ( _type == LOCAL) {
        return toJD( localtime(&time) );
    }
    else {
        return toJD( gmtime(&time) );
    }
    
}

/**
 * toJD() - convert a struct tm to Julian Day
 *
 * @param tm struct
 *
 * @return Julian Date
 */
double TimeOps::toJD( struct tm* pt ) {
    int secs = pt->tm_hour * TimeOps::ISECONDS_PER_HOUR +
    pt->tm_min * TimeOps::ISECONDS_PER_MINUTE +
    pt->tm_sec;
    return double(secs)/TimeOps::SECONDS_PER_DAY +
    toJD(   pt->tm_year + 1900, // years from 1900
            pt->tm_mon + 1,     // months are zero-based
            pt->tm_mday );
}

/**
 * toJC(): Convert to a julian date
 *
 * @param DateTime
 *
 * @returns the julian date
 */
double TimeOps::toJD( const DateTime& _dt )  {
    TimeSpan ts = TimeSpan(_dt.getTicks());
    return ts.getTotalDays() + 1721425.5;
}

/*
 * toJD()
 *
 * Get calendar data for the current year,  including the JD of New Years
 * Day for that year.  After that, add up the days in intervening months,
 * plus the day of the month:
 */
long TimeOps::toJD( int year, int month, int day, CALENDAR_TYPE calendar ) {
    long jd = 0;
    MonthDays md;
    YearEndDays yed;
    
    if( 0 == TimeOps::getCalendarData( year, yed, md, calendar ) ) {
        jd = yed[0];
        for( int i=0; i<(month-1); i++ ) {
            jd += md[i];
        }
        jd += long(day - 1);
    }
    return jd;
}

/**
 * toJD(): convert a day/month/year/hour/minute/second to a double Julian Day
 *
 * @param year - year
 * @param month - month of the year
 * @param day - day of the month
 * @param hours - hours of the day
 * @param minutes - minutes of the hour
 * @param seconds - seconds of the minute
 * @param calendar - (optional) T_GREGORIAN or T_JULIAN, former is the default
 *
 * @return equivalent Julian Day rounded to a long
 */
double TimeOps::toJD( int _year,  int _month, int _day, int _hrs, int _min, int _sec, CALENDAR_TYPE _calendar ) {
    double jd = (double)toJD(_year, _month, _day, _calendar);
    if (_hrs < 12) {
        jd -= MathOps::toDegrees(_hrs, _min, _sec) / 24.0;
    }
    else {
        jd = jd -1. + MathOps::toDegrees(_hrs, _min, _sec) / 24.0;
    }
    
    return jd;
}

//----------------------------------------------------------------------------
/**
 * toMJD():  Julian Date to Modify Julian Date
 *           See p 63,  in Meeus
 *           (http://tycho.usno.navy.mil/mjd.html)
 *
 * @param   The Julian Day value
 *
 * @return  Modify Julian Date
 */
double TimeOps::toMJD( double _jd ) {
    return _jd - 2400000.5;
}

//----------------------------------------------------------------------------
/**
 * toJC(): convert a JD to Julian Century referenced to epoch
 *     J2000. This is the number of days since J2000 converted to centuries.
 *
 * @param Julian Day Number
 *
 * @return centuries since J2000 (12 noon on January 1, 2000)
 */
double TimeOps::toJC( double _jd ) {
    return ( _jd - TimeOps::J2000 ) / TimeOps::DAYS_PER_CENTURY;
}

//----------------------------------------------------------------------------
/**
 * toJM(): convert a JD to Julian Millenia referenced to epoch
 *     J2000. This is the number of days since J2000 converted to millenia.
 *
 * @param Julian Day Number
 *
 * @return millenia since J2000 (12 noon on January 1, 2000)
 */
double TimeOps::toJM( double _jd ) {
    return ( _jd - TimeOps::J2000 ) / TimeOps::DAYS_PER_MILLENIUM;
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
double TimeOps::toGreenwichSiderealTime( double jd ) {
    jd -= TimeOps::J2000;
    double jdc = jd / TimeOps::DAYS_PER_CENTURY;      /* convert jd to julian centuries */
    double intPart = floor( jd );
    double fracPart = jd - intPart;
    double rval =   280.46061837 +
                    360.98564736629 * fracPart +
                    .98564736629 * intPart +
    jdc * jdc * ( 3.87933e-4 - jdc / 38710000. );
    
    return MathOps::toRadians( rval );
}

/**
 * Convert to greenwich sidereal time
 * @returns the greenwich sidereal time
 */
double TimeOps::toGreenwichSiderealTime( const DateTime& _dt )  {
    // t = Julian centuries from 2000 Jan. 1 12h UT1
    const double t = (toJD(_dt) - 2451545.0) / 36525.0;
    
    // Rotation angle in arcseconds
    double theta =  67310.54841
                    + (876600.0 * 3600.0 + 8640184.812866) * t
                    + 0.093104 * t * t
                    - 0.0000062 * t * t * t;
    
    // 360.0 / 86400.0 = 1.0 / 240.0
    return MathOps::normalize( MathOps::toRadians(theta / 240.0), RADS);
}

/**
 * toLMST(): Convert to local mean sidereal time (GMST plus the observer's longitude)
 *
 * @param[in] _dt observers DateTime
 * @param[in] lon observers longitude
 *
 * @returns the local mean sidereal time
 */
double TimeOps::toLocalMeanSideralTime(const DateTime& _dt, double _lng, ANGLE_TYPE _type) {
    if ( _type == DEGS ) {
        _lng = MathOps::toRadians(_lng);
    }
    return MathOps::normalize(toGreenwichSiderealTime(_dt) + _lng, RADS);
}

/**
 * toLST(): Convert Julian Day and geographic longitud to Local Sideral Time
 *          See p 84,  in Meeus
 *
 *          http://129.79.46.40/~foxd/cdrom/musings/formulas/formulas.htm
 *
 * @param jd - julian day
 * @param lng - observer's geographical longitud
 * @param _radians - is observer's geographical longitud in radians
 *
 * @return Local Sidereal Time
 */
double TimeOps::toLocalSideralTime (double _jd, double _lng, ANGLE_TYPE _type) {
    if ( _type == DEGS ) {
        _lng = MathOps::toRadians(_lng);
    }
    return TimeOps::toGreenwichSiderealTime(_jd) + _lng;
}

/**
 * toLST(): Convert to local mean sidereal time (GMST plus the observer's longitude)
 *          See p 84,  in Meeus
 *
 * @param _dt - DateTime
 * @param _lng - observers longitude
 * @param _lng - observers longitude type (DEGS or RADS)
 *
 * @returns the local mean sidereal time
 */
double TimeOps::toLocalSideralTime ( const DateTime& _dt, double _lng, ANGLE_TYPE _type  ) {
    if ( _type == DEGS ) {
        _lng = MathOps::toRadians(_lng);
    }
    return MathOps::normalize(toGreenwichSiderealTime(_dt) + _lng, RADS);
}

//----------------------------------------------------------------------------
/**
 * toTime(): convert a JD to "local" time_t
 *
 * @param jd - jd to convert
 *
 * @return time_t
 */
time_t TimeOps::toTime( double jd ) {
    struct tm t;
    t.tm_isdst=-1;
    
    TimeOps::toDMY( long(floor(jd)), t.tm_mday, t.tm_mon, t.tm_year );
    t.tm_mon--;
    t.tm_year -= 1900;
    
    toHMS(jd, t.tm_hour, t.tm_min, t.tm_sec);
    return mktime( &t );
}

//----------------------------------------------------------------------------
/*
 * toDMY()
 *
 * Estimates the year corresponding to an input JD and calls get_calendar_data();
 * for that year.  Occasionally,  it will find that the guesstimate was off;
 * in such cases,  it moves ahead or back a year and tries again.  Once it's
 * done,  jd - year_ends[0] gives the number of days since New Years Day;
 * by subtracting month_data[] values,  we quickly determine which month and
 * day of month we're in.
 */

// calendar: 0 = gregorian, 1 = julian
void TimeOps::toDMY( double jd, int& _day, int& _month, int& _year, CALENDAR_TYPE calendar ) {
    return toDMY( long(floor(jd)), _day, _month, _year, calendar);
}
void TimeOps::toDMY( long jd, int& day, int& month, int& year, CALENDAR_TYPE calendar ) {
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

void TimeOps::toDMY( const DateTime& _dt, int& day, int& month, int& year ) {
    int totalDays = static_cast<int>(_dt.getTicks() / TimeSpan::TICKS_PER_DAY);
    
    /*
     * number of 400 year cycles
     */
    int num400 = totalDays / 146097;
    totalDays -= num400 * 146097;
    /*
     * number of 100 year cycles
     */
    int num100 = totalDays / 36524;
    if (num100 == 4)
    {
        /*
         * last day of the last leap century
         */
        num100 = 3;
    }
    totalDays -= num100 * 36524;
    /*
     * number of 4 year cycles
     */
    int num4 = totalDays / 1461;
    totalDays -= num4 * 1461;
    /*
     * number of years
     */
    int num1 = totalDays / 365;
    if (num1 == 4) {
        /*
         * last day of the last leap olympiad
         */
        num1 = 3;
    }
    totalDays -= num1 * 365;
    
    /*
     * find year
     */
    year = (num400 * 400) + (num100 * 100) + (num4 * 4) + num1 + 1;
    
    /*
     * convert day of year to month/day
     */
    const int* daysInMonthPtr;
    if (isLeapYear(year)) {
        daysInMonthPtr = daysInMonth[1];
    }
    else {
        daysInMonthPtr = daysInMonth[0];
    }
    
    month = 1;
    while (totalDays >= daysInMonthPtr[month] && month <= 12) {
        totalDays -= daysInMonthPtr[month++];
    }
    
    day = totalDays + 1;
}


//----------------------------------------------------------------------------
/**
 * ToHMS(): break the fractional part of a Julian day into hours, minutes,
 * and seconds
 *
 * @param dayFrac - a fractional day ( >= 0.0, < 1.0 )
 * @param h - where to put the hour
 * @param m - where to put the minute
 * @param s - where to put the second
 *
 */
void TimeOps::toHMS( double jd, int& h, int& m, int& s) {
    
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
        
        if (s == 60) {
            m++;
            s = 0;
        }
        if (m == 60) {
            h++;
            m = 0;
        }
        
        if (h == 24) {
            h = 0;
        }
    }
}

DateTime TimeOps::toDateTime ( double _jd ) {
    double day;
    int month;
    int year;
    int hs;
    int min;
    int sec;
    TimeOps::toYMD(_jd, year, month, day);
    TimeOps::toHMS(_jd - 0.5, hs, min, sec);
    return DateTime(year, month, floor(day), hs, min, sec);
}

//----------------------------------------------------------------------------
/**
 * hourToDay(): convert hour to decimal day, e.g., 12h -> .5d
 *
 * @param hour
 *
 * @return day fraction
 */
double TimeOps::hourToDay ( int _hrs ) { 
    return (double)_hrs / HOURS_PER_DAY; 
}

/**
 * toYMD(): convert a long Julian Day to year/month/day
 *         - See p 63,  in Meeus
 *
 * @param jd - Julian Day to convert
 * @param year& - where to put the year (int)
 * @param month& - where to put the month of the year (int)
 * @param day& - where to put the day of the month (double)
 */
void TimeOps::toYMD(double _jd, int &_year, int &_month, double &_day) {
    int A, B, C, D, E, Z;
    
    double Q = _jd + 0.5;
    Z = floor(Q);
    
    if (Z < 2299161) {
        A = Z;
    }
    else {
        int a = floor( (Z - 1867216.25) / 36524.25 );
        A = Z + 1 + a - floor(a / 4);
    }

    B = A + 1524;
    C = floor( (B - 122.1) / 365.25 );
    D = floor( 365.25 * C );
    E = floor( (B - D) / 30.6001 );

    _day = B - D - floor(30.6001 * E) + (Q-Z);
    if (E < 14) {
        _month = E - 1;
    }
    else {
        _month = E - 13;
    }

    if (_month > 2) {
        _year = C - 4716;
    }
    else {
        _year = C - 4715;
    }
}

//----------------------------------------------------------------------------

/**
 * toDOW(): convert a JD to Day Of the Week 
 *          See p 65,  in Meeus
 *
 * @param Julian Day Number
 *
 * @return day Of the week (sunday = 0)   
 */
int TimeOps::toDOW ( double _jd ) {
    return int(_jd + 1.5) % 7;
}

/**
 * toDOW(): convert a JD to Day Of the Week
 *          See p 65,  in Meeus
 *
 * @param Julian Day Number
 *
 * @return day Of the week (sunday = 0)
 */
int TimeOps::toDOW ( const DateTime &_dt ) {
    /*
     * The fixed day 1 (January 1, 1 Gregorian) is Monday.
     * 0 Sunday
     * 1 Monday
     * 2 Tuesday
     * 3 Wednesday
     * 4 Thursday
     * 5 Friday
     * 6 Saturday
     */
    return static_cast<int>(((_dt.getTicks() / TimeSpan::TICKS_PER_DAY) + 1LL) % 7LL);
}

//----------------------------------------------------------------------------
/**
 * Checks whether the given year is valid
 * @param[in] year the year to check
 * @returns whether the year is valid
 */
bool TimeOps::isValidYear( int year ) {
    bool valid = true;
    if (year < 1 || year > 9999) {
        valid = false;
    }
    return valid;
}

/**
 * Find whether a year is a leap year
 * @param[in] year the year to check
 * @returns whether the year is a leap year
 */
bool TimeOps::isLeapYear( int year ) {
    if (!isValidYear(year)) {
        throw 1;
    }
    
    return (((year % 4) == 0 && (year % 100) != 0) || (year % 400) == 0);
}

//----------------------------------------------------------------------------
// Determine U.S. DST start JD (second Sunday in March as of 2007)
//
long TimeOps::dstStart( int year ) {
    long jdStart = toJD( year, 3, 7+1, T_GREGORIAN );
    while ( 6 != (jdStart % 7 ) ) // Sunday
        jdStart++;
    
    return jdStart;
}

//----------------------------------------------------------------------------
// Determine U.S. DST end JD (first Sunday in November as of 2007)
//
long TimeOps::dstEnd( int year ) {
    long jdEnd = toJD( year, 11, 1, T_GREGORIAN );
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
double TimeOps::tzOffsetInDays( time_t tt ) {
    struct tm* pt = localtime(&tt);
    
    int localH = pt->tm_hour;
    pt = gmtime(&tt);
    
    int gmtH = pt->tm_hour;
    if ( gmtH < localH )
        gmtH += TimeOps::IHOURS_PER_DAY;
    
    return hourToDay( localH - gmtH );  // local - GMT
}

/**
 * tzOffsetInDays(): calculate time zone offset from Universal Time in days
 *                   at spec'd date/time (includes DST)
 *
 * @param jd - day to do calculation for
 *
 * @return Offset (-0.5 ... +0.5 )
 */
double TimeOps::tzOffsetInDays( double jd ) {
    return tzOffsetInDays( toTime(jd) );
}

/**
 * tzOffsetInDays(): calculate time zone offset from Universal Time
 *                   in days using the current time (includes DST)
 *
 * @return Offset (-0.5 ... +0.5 )
 */
double TimeOps::tzOffsetInDays() {
    return tzOffsetInDays( time(0) );
}

//----------------------------------------------------------------------------
/**
 * dstOffsetInDays(): calculate daylight savings time offset in days at
 *                    the spec'd time
 *
 * @return Offset ( 0 or 1/24 )
 */
double TimeOps::dstOffsetInDays( time_t tt ) {
    struct tm* pt = localtime(&tt);
    return (pt->tm_isdst > 0) ? DST_OFFSET : 0.;
}

/**
 * dstOffsetInDays(): calculate daylight savings time offset in days at
 *                    the spec'd time
 *
 * @param jd - date to use
 *
 * @return Offset ( 0 or 1/24 )
 */
double TimeOps::dstOffsetInDays( double jd ) {
    return dstOffsetInDays( toTime(jd) );
}

/**
 * dstOffsetInDays(): calculate current daylight savings time offset in days
 *
 * @return Offset ( 0 or 1/24 )
 */
double TimeOps::dstOffsetInDays() {
    return dstOffsetInDays( time(0) );
}

//----------------------------------------------------------------------------
/**
 * formatTime(): format a time into an HH:MM or HH:MM:SS string
 *
 * @param buf - where to put the string
 * @param dayFrac - a fractional day ( >= 0.0, < 1.0 )
 * @param doSecs - true to include seconds
 */
int formatHMS( char* buf, double dayFrac, bool doSecs ) {
    if (!buf)
        return 0;
    
    int h, m, s, rv = 0;
    TimeOps::toHMS(dayFrac, h, m, s);
    
    if ( doSecs )
        sprintf( buf, "%02d:%02d:%02d", h, m, int(s) );
    else {
        rv = roundToNearestMinute( h, m, s );
        sprintf( buf, "%02d:%02d", h, m );
    }
    return rv;
}


/**
 * formatTime(): format a time into an HH:MM string
 *
 * @param dayFrac - a fractional day ( >= 0.0, < 1.0 )
 * @param doSecs - true to include seconds (HH:MM:SS)
 *
 * @return formated string
 */
char* TimeOps::formatTime( double dayFrac, bool doSecs ) {
    char *buf = new char[8];
    formatHMS( buf, dayFrac, doSecs );
    return buf;
}


/**
 * formatMS(): format a fractional minute into a text string (MM:SS.S)
 *
 * @param m  - the value (in minutes) to format
 *
 * @return ormatted string
 */
char* TimeOps::formatMS( double _min ) {
    char *buf = new char[6];
    sprintf( buf, "%02d:%02.1f\n", int(_min), (_min - int(_min)) * TimeOps::SECONDS_PER_MINUTE );
    return buf;
}

//----------------------------------------------------------------------------
/**
 * formatDateTime(): format a JD into a text string
 *
 * @param jd  - the day to format
 * @param fmt - format type (see DateOps::DATE_FMT)
 *
 * @return formatted string
 */
char* TimeOps::formatDateTime( double _jd, DATE_FMT _fmt ) {
    char *clientBuf = new char[32];
    
    int d, m, y;
    char tbuf[16] = { 0 };
    
    double day;
    TimeOps::toYMD( _jd, y, m, day );
    d = floor(day);
    
    if ( _fmt >= Y_MON_D_HM ) {
        _jd += formatHMS(tbuf, _jd + TimeOps::JD_DIFF, false);
    }
    
    // TimeOps::toDMY( _jd, d, m, y );
    
    switch (_fmt) {
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
    
    return clientBuf;
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

int TimeOps::getCalendarYear(long jd, CALENDAR_TYPE calendar) {
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
        case T_PERSINA_MODERN:
            year = (jd - JALALI_ZERO) / 365L;
            if( year < LOWER_PERSIAN_YEAR)
                year = LOWER_PERSIAN_YEAR;
            if( year > UPPER_PERSIAN_YEAR)
                year = UPPER_PERSIAN_YEAR;
            break;
        case T_CHINESE:
            year = (jd - E_CHINESE) / 365L;
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
    
    if ( year >= 0L ) {
        daysInYear = year * 365L + year / 4L;
        if( !julian )
            daysInYear += -year / 100L + year / 400L;
    }
    else {
        daysInYear = (year * 365L) + (year - 3L) / 4L;
        if( !julian )
            daysInYear += -(year - 99L) / 100L + (year - 399L) / 400L;
    }
    
    if ( julian ) {
        daysInYear -= 2L;
    }
    
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
int TimeOps::getCalendarData( int year, YearEndDays& days, MonthDays& md, CALENDAR_TYPE calendar ) {
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
        case T_PERSINA_MODERN:
            if( year >= LOWER_PERSIAN_YEAR && year <= UPPER_PERSIAN_YEAR)
                getPersianModernYearData( year, days, md );
            else
                rval = -1;
            break;
        case T_CHINESE:
            getChineseYearData( year, days[0], md );
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

void TimeOps::getIslamicYearData( long year, long& daysInYear, MonthDays& md ) {
    static const long THIRTY_ISLAMIC_YEARS = 10631L;
    
    static const int isIslamicLeapYear[30] = {
        0, 0, 1, 0, 0, 1, 0, 1, 0, 0,
        1, 0, 0, 1, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 0, 1, 0, 1, 0, 0, 1 };
    
    long yearWithinCycle = MathOps::mod( year, 30L );
    long thirtyYearCycles = (year - yearWithinCycle) / 30L;
    long rval = E_ISLAMIC +
                thirtyYearCycles * THIRTY_ISLAMIC_YEARS +
                yearWithinCycle * 354L;
    
    md[12] = 0;
    md[11] = (char)(29 + isIslamicLeapYear[yearWithinCycle]);
    while( yearWithinCycle-- ) {
        rval += long( isIslamicLeapYear[yearWithinCycle] );
    }
    
    daysInYear = rval;
    
    /* The Islamic calendar alternates between 30-day and 29-day */
    /* months for the first eleven months;  the twelfth is 30    */
    /* days in a leap year,  29 otherwise (see above).           */
    
    for ( int i=0; i < 11; i++ ) {
        md[i] = 30 - (i % 2);
    }
}

/* End:  Islamic calendar */


/* Begin:  Hebrew calendar */

/* See p 586,  _Explanatory Supplement_,  for explanation. */
/* There are 1080 Halakim,  or 'parts',  each 3.33 seconds long,  in  */
/* an hour.  So: */

static const int isHebrewLeapYear[19] = { 0, 0, 1, 0, 0, 1,
    0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1 };

long TimeOps::lunationsToTishri1( long year ) {
    long yearWithinCycle = MathOps::mod( year - 1, 19L );
    
    long fullNineteenYearCycles = ( year - 1 - yearWithinCycle ) / 19L;
    
    long rval = fullNineteenYearCycles * 235L + yearWithinCycle * 12L;
    
    for( int y=0; y<yearWithinCycle; y++ ) {
        rval += isHebrewLeapYear[y];
    }
    
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
    
    long lunationWithinGlumph = MathOps::mod( lunations, 25920L );
    
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
        if( 3 == MathOps::mod( day, 7L ) &&
           halakim >= 9L * 1080L + 204L &&
           !isHebrewLeapYear[ MathOps::mod( year - 1 + i, 19L) ]
           )
        {
            day += 2;
        }
        /* Check dehiyyah (d): */
        else if( MathOps::mod( day, 7L) == 2 &&
                halakim >= 15L * 1080L + 589L &&
                isHebrewLeapYear[ MathOps::mod( year - 2 + i, 19L) ]
                )
        {
            day++;
        }
        else
        {
            if( halakim > 18L * 1080L )
                day++;
            
            if( MathOps::mod( day, 7L ) == 1 ||
                MathOps::mod( day, 7L ) == 4 ||
                MathOps::mod( day, 7L ) == 6L
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
        
        if( isHebrewLeapYear[ MathOps::mod( year - 1, 19L) ] ) {
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
    
    daysInYear[0] = jalaliJd0( (int)year ) + 1L;
    daysInYear[1] = jalaliJd0( (int)year + 1L ) + 1L;
    
    /* The first six months have 31 days.
     * The next five have 30 days.
     * The last month has 29 days in ordinary years,  30 in leap years.
     */
    for( int i=0; i<6; i++ )
        md[i] = 31;
    
    for( int i=6; i<11; i++ )
        md[i] = 30;
    
    md[11] = (char)( daysInYear[1] - daysInYear[0] - 336L );
    md[12] = 0;      /* always a twelve-month/year calendar */
}

/* End:  Persian (Jalali) calendar */

/* Begin:  Persian Modenr calendar */

/* 7 April 2004:  added the 'modern Persian calendar',  which follows the  */
/* pattern of the astronomically-based Persian (Jalaali) calendar closely, */
/* but not exactly.  The 'modern' (algorithmic) flavor has a pattern of    */
/* 683 leap years over a 2820-year cycle,  in a manner that collapses      */
/* nicely into a few lines of code.                                        */

long TimeOps::persianModernJd0 ( const long year) {
   const long epbase = year - 474L;
   const long epyear = 474L + (epbase + 282000000) % 2820L;

   return( (epyear * 31 - 5) / 128 + (epyear - 1) * 365
               + ((year - epyear) / 2820) * 1029983 + E_PERSIAN);
}

/* 7 April 2004:  modified 'get_jalali_year_data()' so that it can use   */
/* either the traditional astronomical Jalaali-based start of the year,  */
/* or the 'modern' (algorithmic) Persian calendar.  If we're outside the */
/* range covered by the Jalaali algorithm in this code,  we fall back on */
/* use of the algorithmic rule.                                          */

void TimeOps::getPersianModernYearData ( const long year, YearEndDays& daysInYear, MonthDays& md ) {
    if( year < LOWER_PERSIAN_YEAR || year > UPPER_PERSIAN_YEAR) {
        return;
    }
    
    daysInYear[0] = persianModernJd0( (int)year ) + 1L;
    daysInYear[1] = persianModernJd0( (int)year + 1L ) + 1L;
    
    /* The first six months have 31 days.
     * The next five have 30 days.
     * The last month has 29 days in ordinary years,  30 in leap years.
     */
    for( int i=0; i<6; i++ )
        md[i] = 31;
    
    for( int i=6; i<11; i++ )
        md[i] = 30;
    
    md[11] = (char)( daysInYear[1] - daysInYear[0] - 336L );
    md[12] = 0;      /* always a twelve-month/year calendar */
}
/* End:  Persian Modenr calendar */

/* Begin:  Chinese calendar */


/* The Chinese calendar poses some particularly sticky problems,  and is
a real mess to compute.  Therefore,  I wrote a separate program to
"pre-compile" a calendar and store it in a file,  CHINESE.DAT.  To handle
the Chinese calendar,  one must load that file into a buffer,
chinese_calendar_data.  The following code then simply "un-crunches" the
data.

   NOTE:  The odd arrangement of intercalary months in the Chinese calendar
means that one must be careful about month numbering.  The solution I've
used in this code is to return a twelve or thirteen-month calendar,  much
as with the Chinese calendar;  the external int chinese_intercalary_month
is used to indicate which of the 13 is intercalary (it's left at zero if
there is no intercalary month.)  See JD.CPP for an example of its use.

   NOTE:  There is no 'official' numbering scheme for the Chinese calendar.
Traditionally,  it just ran on a sixty-year cycle.  There are three
different schemes in use.  I've used one in which the Gregorian year 2000
corresponds to a Chinese year 4637.  Some people think the Chinese
calendar was created sixty years earlier,  and that therefore 2000 (Greg.)
= 4397 (Chinese).  Just to make things still more confusing,  some people
add another year to this: 2000 (Greg.) = 4398 (Chinese).  So you may see
Chinese "year" numbers that are 60 or 61 greater than those used in this
code.

   NOTE:  Some parts of the following code extract integer data without
proper regard to byte-order.  If you compile this on "wrong-endian"
machines,  this will have to be fixed!  (Not a big deal;  three lines,
marked below,  need changes.)

   The "packing" works as follows.  The data for each year is stored in
24 bits.  The 13 least significant bits are set (or unset) to indicate
30-day (or 29-day) months.  Shifting the "packed value" down by 13 thus
leaves an 11-bit quantity.

   That value,  modulo 14,  tells you which month in that year is the
intercalary one.  (If it's zero,  there is no intercalary month,  and
this is a twelve-month year.)  Dividing the value by 14 gives you an
"offset",  and the JD of the Chinese New Year is then given as

jd = 365 * year + year / 4 + E_CHINESE + offset

Thus,  the offset can be a value from 0 to (2^11 / 14) = 186. */

static char *chinese_calendar_data = NULL;
static int chinese_intercalary_month = 0;

void TimeOps::getChineseYearData ( const long year, long& daysInYear, MonthDays& md ) {
   int32_t packed_val = 0;

   if( !chinese_calendar_data )
      return;

   int index = (int)year - *(int16_t *)( chinese_calendar_data + 2);
   int n_years = *(const int16_t *)chinese_calendar_data;
         /* Above lines should involve byte-swapping */

   if( index < 0 || index >= n_years) {
      return;
   }

   memcpy( &packed_val, chinese_calendar_data + 4 + 3 * index, 3);
         /* Swap 'packed_val' on non-Intel byte order machines */
   for( int i = 0; i < 13; i++) {
      md[i] = (char)(((packed_val >> i) & 1L) ? 30 : 29);
   }
   chinese_intercalary_month = (int)( (packed_val >> 13) % 14L);

   if( chinese_intercalary_month) {
      chinese_intercalary_month++;
   }
   else {             /* if there's no intercalary month... */
      md[12] = 0;     /* ...then the '13th month' has zero days */
    }

   daysInYear = year * 365L + year / 4L + E_CHINESE + (packed_val >> 13) / 14L;
}
/* End:  Chinese calendar */

#endif  /* #if defined( CALENDARS_OF_THE_WORLD ) */


