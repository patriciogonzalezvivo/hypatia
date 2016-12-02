#pragma once

struct TimeOps {
    static const double SECONDS_PER_DAY;
    static const double TROPICAL_YEAR;
    static const double JULIAN_EPOCH;
    static const double SPEED_OF_LIGHT;
    static const double TO_CENTURIES;
    
    static const double HOURS_PER_DAY;
    static const int IHOURS_PER_DAY;
    
    static const double J2000;
    
    // Seconds elapsed since epoch (1 January 1970 00:00:00 UTC)
    static unsigned long getCurrentSeconds();
    
    // Milliseconds elapsed since epoch (1 January 1970 00:00:00 UTC)
    static unsigned long getCurrentMilliseconds();
    
    // Return UT Hours
    static double getUT( double _tz_offset = 0.0 );
    
    // to Julian Dates (_milli == 0 means NOW)
    static double julianDates( unsigned long _sec = 0.0 );
    
    // to Modified Julian Dates ( http://scienceworld.wolfram.com/astronomy/ModifiedJulianDate.html )
    static double modifiedJulianDates( unsigned long _sec = 0.0 ); // (_sec == 0 means NOW)
    
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
