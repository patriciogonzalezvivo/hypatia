#pragma once

#include <time.h>
#include <stdio.h>

// uncomment the following line to include an assortment of non-western
// calendars:
// #define CALENDARS_OF_THE_WORLD

typedef int MonthDays[13];
typedef long YearEndDays[2];

enum CalendarType {
    T_GREGORIAN = 0,
    T_JULIAN = 1
#if defined(CALENDARS_OF_THE_WORLD)
    ,T_HEBREW = 2,
    T_ISLAMIC = 3,
    T_REVOLUTIONARY = 4,
    T_PERSIAN = 5
#endif
};

// date format specifier.
// examples provided are for J2000 (January 1, 2000 at noon)

enum DATE_FMT {
    Y_MON_D,      // 2000 Jan 01
    MON_D_Y,      // Jan 01 2000
    MON_D,        // Jan 01
    Y_M_D,        // 2000-01-01
    M_D_Y,        // 01/01/2000
    M_D,          // 01/01
    // Note: date only above, date + time below
    //       The code depends on this separation.
    Y_MON_D_HM,   // 2000 Jan 01 12:00
    MON_D_Y_HM,   // Jan 01 2000 12:00
    MON_D_HM,     // Jan 01 12:00
    Y_M_D_HM,     // 2000-01-01 12:00
    M_D_Y_HM,     // 01/01/2000 12:00
    M_D_HM        // 01/01 12:00
};

struct TimeOps {
    static const double DAYS_PER_HOUR;
    static const double HOURS_PER_DAY;
    static const double SECONDS_PER_DAY;
    static const double SECONDS_PER_MINUTE;
    static const double MINUTES_PER_HOUR;
    static const double SECONDS_PER_HOUR;
    static const double MINUTES_PER_DAY;
    
    static const int    IHOURS_PER_DAY;
    static const int    ISECONDS_PER_MINUTE;
    static const int    IMINUTES_PER_HOUR;
    static const int    ISECONDS_PER_HOUR;
    
    static const double TROPICAL_YEAR;
    static const double JULIAN_EPOCH;
    static const double MJULIAN_EPOCH;
    
    static const double DAYS_PER_CENTURY;
    static const double DAYS_PER_MILLENIUM;
    
    static const double J2000;
    static const double JD_DIFF;
    
    // These are initialized in DateOps.cpp
    static const double DST_OFFSET;
    
    /*** TIME ******************************************************************/
    
    /**
     * tzOffsetInDays(): calculate time zone offset from Universal Time
     *                   in days for the spec'd time_t (includes DST)
     *
     * @param tt = time_t to convert
     *
     * @return Offset (-0.5 ... +0.5 )
     */
    static double tzOffsetInDays (time_t _tt);
    
    /**
     * tzOffsetInDays(): calculate time zone offset from Universal Time in days
     *                   at spec'd date/time (includes DST)
     *
     * @param jd - day to do calculation for
     *
     * @return Offset (-0.5 ... +0.5 )
     */
    static double tzOffsetInDays (double _jd);
    
    /**
     * tzOffsetInDays(): calculate time zone offset from Universal Time
     *                   in days using the current time (includes DST)
     *
     * @return Offset (-0.5 ... +0.5 )
     */
    static double tzOffsetInDays ();
    
    /**
     * dstOffsetInDays(): calculate daylight savings time offset in days at
     *                    the spec'd time
     *
     * @param time_t - date to use
     *
     * @return Offset ( 0 or 1/24 )
     */
    static double dstOffsetInDays ( time_t _tt );
    
    /**
     * dstOffsetInDays(): calculate daylight savings time offset in days at
     *                    the spec'd time
     *
     * @param jd - date to use
     *
     * @return Offset ( 0 or 1/24 )
     */
    static double dstOffsetInDays ( double _jd );
    
    /**
     * dstOffsetInDays(): calculate current daylight savings time offset in days
     *
     * @return Offset ( 0 or 1/24 )
     */
    static double dstOffsetInDays ();

    /**
     * formatTime(): format a time into an HH:MM string
     *
     * @param buf - where to put the string
     * @param dayFrac - a fractional day ( >= 0.0, < 1.0 )
     * @param doSecs - true to include seconds (HH:MM:SS)
     *
     * @return 1 if rounding wrapped to 00:00[:00], else 0
     */
    static int formatTime ( char* _buf, double _dayFrac, bool _doSecs = false );

    /**
     * formatTime(): format a time into an HH:MM string
     *
     * @param dayFrac - a fractional day ( >= 0.0, < 1.0 )
     * @param doSecs - true to include seconds (HH:MM:SS)
     *
     * @return formated string
     */
    static char* formatTime ( double _dayFrac, bool _doSecs = false );

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
    static void toHMS ( double _jd,  int& _hrs, int& _min, int& _sec );
    
    /**
     * hourToDay(): convert hour to decimal day, e.g., 12h -> .5d
     *
     * @param hour
     *
     * @return day fraction
     */
    static double hourToDay ( int _hrs );
    
    /**
     * formatDateTime(): format a JD into a text string
     *
     * @param buf - where to put the formatted string
     * @param jd  - the day to format
     * @param fmt - format type (see DateOps::DATE_FMT)
     */
    static void  formatDateTime ( char* buf, double _jd, DATE_FMT _fmt );

    /**
     * formatDateTime(): format a JD into a text string
     *
     * @param jd  - the day to format
     * @param fmt - format type (see DateOps::DATE_FMT)
     *
     * @return formatted string
     */
    static char* formatDateTime ( double _jd, DATE_FMT _fmt );
    
    /**
     * formatMS(): format a fractional minute into a text string (MM:SS.S)
     *
     * @param buf - where to put the formatted string
     * @param m  - the value (in minutes) to format
     */
    static void formatMS( char* _buf, double _min );

    /**
     * formatMS(): format a fractional minute into a text string (MM:SS.S)
     *
     * @param m  - the value (in minutes) to format
     *
     * @return ormatted string
     */
    static char* formatMS( double _min );
    
    /**
     * timeToLDay(): convert a "local" time_t to Julian Day
     *
     * @param time - time_t to convert
     *
     * @return Julian Day
     */
    static double timeToLDay( time_t _time );
    
    /**
     * timeToUDay(): convert UTC time_t to Julian Day
     *
     * @param time_t to convert
     *
     * @return Julian Day
     */
    static double timeToUDay( time_t _time );
    
    /**
     * dayToTime(): convert a JD to "local" time_t (including DST)
     *
     * @param jd - jd to convert
     *
     * @return equivalent time_t
     */
    static time_t toTime( double _jd );
    
    /**
     * now(): Determine the Julian Day value at the present moment
     *
     * @param local - true for local time, false for UTC
     *
     * @return The Julian Day value
     */
    static double now(bool _local = false);
    
    /**
     * greenwichSiderealTime(): convert a "local" time_t to GST. See p 84,  in Meeus
     *
     * @param jd - julian day
     *
     * @return apparent Greenwich sidereal time (in Radians) for the given jd
     */
    static double toGreenwichSiderealTime ( double _jd );
    
    // to Greenwich sidereal time in a range of 0~24hs
    static double toGreenwichSiderealHour ( double _jd );

    // To Local Sidereal Time http://129.79.46.40/~foxd/cdrom/musings/formulas/formulas.htm
    static double toLST ( double _jd, double _lng_deg );

    // Seconds elapsed since epoch (1 January 1970 00:00:00 UTC)
    static unsigned long getCurrentSeconds ();
    
    // to Julian Dates (_milli == 0 means NOW)
    static double julianDates ( unsigned long _sec = 0.0 );
    
    /*** DATE ******************************************************************/
    
    /**
     * toJulianCenturies(): convert a JD to Julian Century referenced to epoch
     *     J2000. This is the number of days since J2000 converted to centuries.
     *
     * @param Julian Day Number
     *
     * @return centuries since J2000 (12 noon on January 1, 2000)
     */
    static double toJulianCenturies ( double _jd );
    
    /**
     * toJulianMillenia(): convert a JD to Julian Millenia referenced to epoch
     *     J2000. This is the number of days since J2000 converted to millenia.
     *
     * @param Julian Day Number
     *
     * @return millenia since J2000 (12 noon on January 1, 2000)
     */
    static double toJulianMillenia ( double _jd );
    
    /**
     * dmyToDay(): convert a day/month/year to a long Julian Day
     *
     * @param day - day of the month
     * @param month - month of the year
     * @param year - year
     * @param calendar - (optional) T_GREGORIAN or T_JULIAN, former is the default
     *
     * @return equivalent Julian Day rounded to a long
     */
    static long DMYtoJD ( int _day, int _month, int _year, CalendarType _calendar = T_GREGORIAN );

    /**
     * dayToDmy(): convert a long Julian Day to day/month/year
     *
     * @param jd - Julian Day to convert
     * @param day& - where to put the day of the month
     * @param month& - where to put the month of the year
     * @param year& - where to put the year
     * @param calendar - (optional) T_GREGORIAN or T_JULIAN, former is the default
     */
    static void toDMY ( long _jd, int& _day, int& _month, int& _year, CalendarType calendar = T_GREGORIAN );
    static void toDMY ( double _jd, int& _day, int& _month, int& _year, CalendarType calendar = T_GREGORIAN );
    static void toYMD ( double _jd, int &_year, int &_month, double &_day );
    //--------------------------------------------------------------------------
    /**
     * dstStart(): Determine the Julian Day in the specified year where Daylight
     *             Savings Time starts
     *
     * @param The year
     *
     * @return The Julian Day value
     */
    static long dstStart ( int _year );
    
    /**
     * dstEnd(): Determine the Julian Day in the specified year where Daylight
     *           Savings Time ends
     *
     * @param The year
     *
     * @return The Julian Day value
     */
    static long dstEnd ( int _year );

    /*** CALENDAR ******************************************************************/

    // Julian Date to Modify Julian Date (http://tycho.usno.navy.mil/mjd.html)
    static double toMJD ( double _jd );

    /*** CALENDAR ******************************************************************/
    static char* getMonth ( int _month );
    static char* getMonthAbbreviation ( int _month );
    static int   getCalendarYear ( long _jd, CalendarType _calendar );
    static void  getJulGregYearData ( int _year, long& _days, MonthDays& _md, bool _julian );
    static int   getCalendarData ( int _year, YearEndDays& _days, MonthDays& _md, CalendarType _calendar );
    
private:
    enum CalendarEpoch {
        E_JULIAN_GREGORIAN = 1721060L
#if defined(CALENDARS_OF_THE_WORLD)
        ,E_ISLAMIC = 1948086L,
        E_HEBREW = 347996L,
        E_REVOLUTIONARY = 2375475L
#endif
    };
    
    // ---------------------------------------------------------------------------
#if defined( CALENDARS_OF_THE_WORLD )
    
    enum {
        HALAKIM_IN_DAY = (24L * 1080L),
        JALALI_ZERO = 1947954L,
        LOWER_PERSIAN_YEAR = -1096,
        UPPER_PERSIAN_YEAR = 2327
    };
    
    static int mod ( int x, int y );
    
    // Islamic calendar
    static void getIslamicYearData ( int year, int& days, MonthDays& md );
    
    // Hebrew calendar
    static int lunationsToTishri1 ( int year );
    static void lunationsToDaysAndHalakim ( int lunations, int& days, int& halakim);
    static void findTishri1 ( int year, int& days, int& halakim );
    static void getHebrewYearData ( int year, YearEndDays& days, MonthDays& md );
    
    // French Revolutionary calendar
    static int jdOfFrenchRevYear ( int year );
    static void getRevolutionaryYearData ( int year, YearEndDays& days, MonthDays& md );
    
    // Persian (Jalaali) calendar
    static int jalaliJd0 ( int jalaliYear );
    static void getJalaliYearData ( const int year, YearEndDays& days, MonthDays& md );
    
#endif  /* #if defined( CALENDARS_OF_THE_WORLD ) */
    
    // ---------------------------------------------------------------------------

    /**
     * roundToNearestMinute(): round a time to the nearest minute
     *
     * if h:m wraps to zero, we roll back to 23:59 to preserve "today"
     *
     * @param h - hour
     * @param m - minute
     * @param s - second
     *
     * @return 1 if rounding wrapped to 00:00, else 0.
     *
     */
    static int roundToNearestMinute( int &_hrs, int &_min, int &_sec );
    
    static double timeToDay( struct tm* pt );
};
