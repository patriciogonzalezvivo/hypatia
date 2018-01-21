#pragma once

struct TimeOps {
    static const double SECONDS_PER_DAY;
    static const double TROPICAL_YEAR;
    static const double JULIAN_EPOCH;
    static const double MJULIAN_EPOCH;
    static const double SPEED_OF_LIGHT;
    static const double TO_CENTURIES;
    
    static const double HOURS_PER_DAY;
    static const int IHOURS_PER_DAY;
    
    static const double J2000;
    
    // Seconds elapsed since epoch (1 January 1970 00:00:00 UTC)
    static unsigned long getCurrentSeconds();
    
    // Milliseconds elapsed since epoch (1 January 1970 00:00:00 UTC)
    static unsigned long getCurrentMilliseconds();
    
    // to Julian Dates (_milli == 0 means NOW)
    static double julianDates( unsigned long _sec = 0.0 );
    
    // to Modified Julian Dates ( http://scienceworld.wolfram.com/astronomy/ModifiedJulianDate.html )
    static double modifiedJulianDates( unsigned long _sec = 0.0 ); // (_sec == 0 means NOW)
    
    // Julian Date to Modify Julian Date (http://tycho.usno.navy.mil/mjd.html)
    static double JDtoMJD(double _jd);

    /* given the modified Julian date (number of days elapsed since 1900 jan 0.5,),
     * mj, return the calendar date in months, *mn, days, *dy, and years, *yr.
     */
    static void MJDtoMDY(double _mjd, int &_mont, double &_day, int &_year);
    
    /* given an mjd, set *dow to 0..6 according to which day of the week it falls
     * on (0=sunday).
     * return 0 if ok else -1 if can't figure it out.
     */
    static int MJDtoDOW (double _mjd);
    
    /* given the Julian day return the calendar date in months,  days, and years
     *  https://quasar.as.utexas.edu/BillInfo/JulianDatesG.html
     */
    static void JDtoMDY(double _jd, int &_mont, double &_day, int &_year);

    /* given a date in months, mn, days, dy, years, yr,
     * return the modified Julian date (number of days elapsed since 1900 jan 0.5)
     */
    static double MDYtoMJD (int _month, double _day, int _year);

    /* given a mjd, return the year as a double. */
    static double MJDtoYears (double _mjd);
    
    // Modified Julian Dates to Jordan Cosmological Theory
    static double MJDtoJCT( double _mjd );
    static double jordanCosmologicalTheory( unsigned long _sec = 0.0 ); // (_sec == 0 means NOW)
    
    // Modified Julian Dates to Greenwich Mean Sidereal Time ( https://www.cv.nrao.edu/~rfisher/Ephemerides/times.html )
    static double MJDtoGMS( double _mjd );
    static double greenwichMeanSiderealTime( unsigned long _sec = 0.0 ); // (_sec == 0 means NOW)
    
    // to Greenwich sidereal time
    static double greenwichSiderealTime( double _jd );
    
    // to Greenwich sidereal time in a range of 0~24hs
    static double greenwichSiderealHour( double _jd );
    
    // convert hour to decimal day, e.g., 12h -> .5d
    static double toDays( int hours ) { return (double)hours / HOURS_PER_DAY; }
    
    // convert a JD to Julian Millenia referenced to epoch J2000
    static double toMillenia( double _jd ) { return ( _jd - J2000 ) / TO_CENTURIES; }
    
    // To Local Sidereal Time http://129.79.46.40/~foxd/cdrom/musings/formulas/formulas.htm
    static double localSiderealTime( double _jd, double _lng_deg );
};
