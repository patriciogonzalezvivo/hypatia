#pragma once

#include <time.h>
#include <stdio.h>

#include "MathOps.h"
#include "primitives/DateTime.h"

// uncomment the following line to include an assortment of non-western
// calendars:
#define CALENDARS_OF_THE_WORLD

typedef char MonthDays[13];
typedef long YearEndDays[2];

enum TIME_TYPE {
    LOCAL,
    UTC
};

enum CALENDAR_TYPE {
    T_GREGORIAN = 0,
    T_JULIAN = 1
#if defined(CALENDARS_OF_THE_WORLD)
    ,T_HEBREW = 2,
    T_ISLAMIC = 3,
    T_REVOLUTIONARY = 4,
    T_PERSIAN = 5,
    T_PERSINA_MODERN = 6,
    T_CHINESE = 7
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
    static void toHMS ( double _jd, int &_hrs, int &_min, int &_sec );
    
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
     * @param jd  - the day to format
     * @param fmt - format type (see DateOps::DATE_FMT)
     *
     * @return formatted string
     */
    static char* formatDateTime ( double _jd, DATE_FMT _fmt );

    /**
     * formatMS(): format a fractional minute into a text string (MM:SS.S)
     *
     * @param m  - the value (in minutes) to format
     *
     * @return ormatted string
     */
    static char* formatMS( double _min );
    
    
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
    static double now(TIME_TYPE _type);
    
    /**
     * nowSeconds(): current Seconds elapsed since epoch (1 January 1970 00:00:00 UTC)
     *
     * @return current seconds
     */
    static unsigned long nowSeconds();
    
    /**
     * greenwichSiderealTime(): convert a "local" time_t to GST. 
     *                          See p 84,  in Meeus
     *
     * @param jd - julian day
     *
     * @return apparent Greenwich sidereal time (in Radians) for the given jd
     */
    static double toGreenwichSiderealTime ( double _jd );
    
    /**
     * greenwichSiderealTime(): convert a "local" time_t to GST.
     *                          See p 84,  in Meeus
     *
     * @param _dt - DateTime
     *
     * @return apparent Greenwich sidereal time (in Radians) for the given jd
     */
    static double toGreenwichSiderealTime ( const DateTime& _dt );
    
    /**
     * toLMST(): Convert to local mean sidereal time (GMST plus the observer's longitude)
     *
     * @param[in] _dt observers DateTime
     * @param[in] lon observers longitude
     *
     * @returns the local mean sidereal time
     */
    static double toLocalMeanSideralTime(const DateTime& _dt, double _lng, ANGLE_TYPE _type);
    
    /**
     * toLST(): Convert Julian Day and geographic longitud to Local Sideral Time
     *          See p 84,  in Meeus
     *
     *          http://129.79.46.40/~foxd/cdrom/musings/formulas/formulas.htm
     *
     * @param _jd - julian day
     * @param _lng - observer's geographical longitud
     * @param _type - is observer's geographical longitud in RADS or DEGS
     *
     * @return Local Sidereal Time
     */
    static double toLocalSideralTime ( double _jd, double _lng, ANGLE_TYPE _type );
    
    /**
     * toLST(): Convert Julian Day and geographic longitud to Local Sideral Time
     *          See p 84,  in Meeus
     *
     *          http://129.79.46.40/~foxd/cdrom/musings/formulas/formulas.htm
     *
     * @param _dt - DateTime
     * @param _lng - observer's geographical longitud
     * @param _type - is observer's geographical longitud in RADS or DEGS
     *
     * @return Local Sidereal Time
     */
    static double toLocalSideralTime ( const DateTime& _dt, double _lng, ANGLE_TYPE _type );
    
    /**
     * toJD(): convert a seconds to Julian Century
     *
     * @param seconds (time stamp since epoch )
     *
     * @return Julian Century
     */
    static double toJD ( unsigned long _sec );
    
    /**
     * toJD() - convert a struct tm to Julian Day
     *
     * @param tm struct
     *
     * @return Julian Date
     */
    static double toJD( struct tm* pt );
    
    /**
     * toJD(): convert UTC time_t to Julian Day
     *
     * @param time_t to convert
     * @param time type (UTC/LOCAL)
     *
     * @return Julian Day
     */
    static double toJD( time_t _time , TIME_TYPE _type );
    
    /**
     * toJC(): convert a seconds to Julian Century
     *
     * @param seconds (time stamp since epoch )
     *
     * @return Julian Century
     */
    static double toJD ( const DateTime& _dt );
    
    /*** DATE ******************************************************************/
    
    /**
     * toJD(): convert a day/month/year to a long Julian Day
     *
     * @param year - year
     * @param month - month of the year
     * @param day - day of the month
     * @param calendar - (optional) T_GREGORIAN or T_JULIAN, former is the default
     *
     * @return equivalent Julian Day rounded to a long
     */
    static long toJD ( int _year,  int _month, int _day, CALENDAR_TYPE _calendar = T_GREGORIAN );

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
    static double toJD (int _year,  int _month, int _day, 
                        int _hrs, int _min, int _sec, CALENDAR_TYPE _calendar = T_GREGORIAN );

    /**
     * toJC(): convert a JD to Julian Century referenced to epoch
     *     J2000. This is the number of days since J2000 converted to centuries.
     *
     * @param Julian Day Number
     *
     * @return centuries since J2000 (12 noon on January 1, 2000)
     */
    static double toJC ( double _jd );
    
    /**
     * toJM(): convert a JD to Julian Millenia referenced to epoch
     *     J2000. This is the number of days since J2000 converted to millenia.
     *
     * @param Julian Day Number
     *
     * @return millenia since J2000 (12 noon on January 1, 2000)
     */
    static double toJM ( double _jd );

        /**
     * toMJD():  Julian Date to Modify Julian Date 
     *           See p 63,  in Meeus
     *           (http://tycho.usno.navy.mil/mjd.html)
     *
     * @param   The Julian Day value
     *
     * @return  Modify Julian Date 
     */
    static double toMJD ( double _jd );
    
    /**
     * toDOW(): convert a JD to Day Of the Week
     *          See p 65,  in Meeus
     *
     * @param Julian Day Number
     *
     * @return day Of the week (sunday = 0)
     */
    static int toDOW ( double _jd );
    
    /**
     * toDOW(): convert a DateTime to Day Of the Week
     *          See p 65,  in Meeus
     *
     * @param DateTime
     *
     * @return day Of the week (sunday = 0)
     */
    static int toDOW ( const DateTime& _dt );
    
    /**
     * toYMD(): convert a long Julian Day to year/month/day
     *          See p 63,  in Meeus
     *
     * @param jd - Julian Day to convert
     * @param year& - where to put the year (int)
     * @param month& - where to put the month of the year (int)
     * @param day& - where to put the day of the month (double)
     */
    static void toYMD ( double _jd, int &_year, int &_month, double &_day );
    
    /**
     * toDMY(): convert a DateTime to day/month/year
     *
     * @param jd - Julian Day to convert
     * @param day& - where to put the day of the month
     * @param month& - where to put the month of the year
     * @param year& - where to put the year
     * @param calendar - (optional) T_GREGORIAN or T_JULIAN, former is the default
     */
    static void toDMY ( const DateTime& _dt, int& _day, int& _month, int& _year );

    /**
     * toDMY(): convert a Julian Day to day/month/year
     *
     * @param jd[in] - Julian Day to convert
     * @param day& - where to put the day of the month
     * @param month& - where to put the month of the year
     * @param year& - where to put the year
     * @param calendar - (optional) T_GREGORIAN or T_JULIAN, former is the default
     */
    static void toDMY ( long _jd, int &_day, int &_month, int &_year, CALENDAR_TYPE calendar = T_GREGORIAN );
    static void toDMY ( double _jd, int &_day, int &_month, int &_year, CALENDAR_TYPE calendar = T_GREGORIAN );
    
    static DateTime toDateTime ( double _jd );

    /**
     * isValidYear(): Checks whether the given year is valid
     *
     * @param[in] year the year to check
     *
     * @returns whether the year is valid
     */
    static bool isValidYear ( int _year );
    
    /**
     * isLeapYear(): Find whether a year is a leap year
     *
     * @param[in] year the year to check
     *
     * @returns whether the year is a leap year
     */
    static bool isLeapYear ( int _year );
    
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
    static char* getDOW( int _dow );
    static char* getDOWAbbreviation( int _dow );

    static char* getMonth ( int _month );
    static char* getMonthAbbreviation ( int _month );

    static int   getCalendarYear ( long _jd, CALENDAR_TYPE _calendar );
    static void  getJulGregYearData ( int _year, long& _days, MonthDays& _md, bool _julian );
    static int   getCalendarData ( int _year, YearEndDays& _days, MonthDays& _md, CALENDAR_TYPE _calendar );
    
private:
    enum CalendarEpoch {
        E_JULIAN_GREGORIAN  = 1721060L
#if defined(CALENDARS_OF_THE_WORLD)
        ,E_ISLAMIC          = 1948086L,
        E_HEBREW            = 347996L,
        E_REVOLUTIONARY     = 2375475L,
        E_PERSIAN           = 1948320L,
        E_CHINESE           = 757862L
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
    
    // static long mod ( long x, long y );
    
    // Islamic calendar
    static void getIslamicYearData ( long year, long& days, MonthDays& md );
    
    // Hebrew calendar
    static long lunationsToTishri1 ( long year );
    static void lunationsToDaysAndHalakim ( long lunations, long& days, long& halakim);
    static void findTishri1 ( long year, long& days, long& halakim );
    static void getHebrewYearData ( long year, YearEndDays& days, MonthDays& md );
    
    // French Revolutionary calendar
    static long jdOfFrenchRevYear ( long year );
    static void getRevolutionaryYearData ( long year, YearEndDays& days, MonthDays& md );
    
    // Persian (Jalaali) calendar
    static long jalaliJd0 ( long jalaliYear );
    static void getJalaliYearData ( const long year, YearEndDays& days, MonthDays& md );

    static long persianModernJd0 ( long jalaliYear );
    static void getPersianModernYearData ( const long year, YearEndDays& days, MonthDays& md );

    // Chinese
    static void getChineseYearData ( const long year, long& days, MonthDays& md );
    
#endif  /* #if defined( CALENDARS_OF_THE_WORLD ) */
    // ---------------------------------------------------------------------------

};
