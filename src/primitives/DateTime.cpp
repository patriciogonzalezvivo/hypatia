#include "DateTime.h"

#include "../TimeOps.h"

#if defined(_WIN32) || defined(_WIN64)

//#define NOMINMAX
#include <windows.h>

#define BILLION                             (1E9)
#define CLOCK_REALTIME						0

static BOOL g_first_time = 1;
static LARGE_INTEGER g_counts_per_sec;

int clock_gettime(int dummy, struct timespec *ct) {
	LARGE_INTEGER count;

	if (g_first_time) {
		g_first_time = 0;
		if (0 == QueryPerformanceFrequency(&g_counts_per_sec)) {
			g_counts_per_sec.QuadPart = 0;
		}
	}

	if ((NULL == ct) || (g_counts_per_sec.QuadPart <= 0) ||
		(0 == QueryPerformanceCounter(&count))) {
		return -1;
	}

	ct->tv_sec = count.QuadPart / g_counts_per_sec.QuadPart;
	ct->tv_nsec = ((count.QuadPart % g_counts_per_sec.QuadPart) * BILLION) / g_counts_per_sec.QuadPart;

	return 0;
}
#define MIN min
#else
#define MIN std::min
#endif

static int daysInMonth[2][13] = {
    //  1   2   3   4   5   6   7   8   9   10  11  12
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};
static int cumulDaysInMonth[2][13] = {
    //  1  2   3   4   5    6    7    8    9    10   11   12
    {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
    {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
};

// --------------------------------------------------- STATIC

/**
 * Check whether the year/month is valid
 * @param[in] year the year to check
 * @param[in] month the month to check
 * @returns whether the year/month is valid
 */
bool IsValidYearMonth(int year, int month) {
    bool valid = true;
    if (TimeOps::isValidYear(year)) {
        if (month < 1 || month > 12) {
            valid = false;
        }
    }
    else {
        valid = false;
    }
    return valid;
}

/**
 * Find the number of days in a month given the year/month
 * @param[in] year the year
 * @param[in] month the month
 * @returns the days in the given month
 */
int DaysInMonth(int year, int month) {
    if (!IsValidYearMonth(year, month)) {
        throw 1;
    }
    
    const int* daysInMonthPtr;
    
    if (TimeOps::isLeapYear(year)) {
        daysInMonthPtr = daysInMonth[1];
    }
    else {
        daysInMonthPtr = daysInMonth[0];
    }
    
    return daysInMonthPtr[month];
}

/**
 * Check whether the year/month/day is valid
 * @param[in] year the year to check
 * @param[in] month the month to check
 * @param[in] day the day to check
 * @returns whether the year/month/day is valid
 */
bool IsValidYearMonthDay(int year, int month, int day) {
    bool valid = true;
    if (IsValidYearMonth(year, month)) {
        if (day < 1 || day > DaysInMonth(year, month)) {
            valid = false;
        }
    }
    else {
        valid = false;
    }
    return valid;
}

/**
 * Find the day of the year given the year/month/day
 * @param[in] year the year
 * @param[in] month the month
 * @param[in] day the day
 * @returns the day of the year
 */
int DayOfYear(int year, int month, int day) {
    if (!IsValidYearMonthDay(year, month, day)) {
        throw 1;
    }
    
    int daysThisYear = day;
    
    if (TimeOps::isLeapYear(year)) {
        daysThisYear += cumulDaysInMonth[1][month];
    }
    else {
        daysThisYear += cumulDaysInMonth[0][month];
    }
    
    return daysThisYear;
}

double AbsoluteDays(unsigned int year, double doy) {
    int64_t previousYear = year - 1;
    
    /*
     * + days in previous years ignoring leap days
     * + Julian leap days before this year
     * - minus prior century years
     * + plus prior years divisible by 400 days
     */
    int64_t daysSoFar = 365 * previousYear
    + previousYear / 4LL
    - previousYear / 100LL
    + previousYear / 400LL;
    
    return static_cast<double>(daysSoFar) + doy - 1.0;
}

int AbsoluteDays(int year, int month, int day) {
    int previousYear = year - 1;
    
    /*
     * days this year (0 - ...)
     * + days in previous years ignoring leap days
     * + Julian leap days before this year
     * - minus prior century years
     * + plus prior years divisible by 400 days
     */
    int result = DayOfYear(year, month, day) - 1
    + 365 * previousYear
    + previousYear / 4
    - previousYear / 100
    + previousYear / 400;
    
    return result;
}

// ---------------- END of static

/**
 * Default contructor
 * Initialise to 0001/01/01 00:00:00.000000
 */
DateTime::DateTime() {
    initialise(1, 1, 1, 0, 0, 0, 0);
}

/**
 * Constructor
 * @param[in] ticks raw tick value
 */
DateTime::DateTime(int64_t ticks) : m_encoded(ticks) {
}

/**
 * Constructor
 * @param[in] year the year
 * @param[in] doy the day of the year
 */
DateTime::DateTime(unsigned int year, double doy) {
    m_encoded = TimeSpan(static_cast<int64_t>(AbsoluteDays(year, doy) * TimeSpan::TICKS_PER_DAY)).getTicks();
}

/**
 * Constructor
 * @param[in] year the year
 * @param[in] month the month
 * @param[in] day the day
 */
DateTime::DateTime(int year, int month, int day) {
    initialise(year, month, day, 0, 0, 0, 0);
}

/**
 * Constructor
 * @param[in] year the year
 * @param[in] month the month
 * @param[in] day the day
 * @param[in] hour the hour
 * @param[in] minute the minute
 * @param[in] second the second
 */
DateTime::DateTime(int year, int month, int day, int hour, int minute, int second) {
    initialise(year, month, day, hour, minute, second, 0);
}

/**
 * Constructor
 * @param[in] year the year
 * @param[in] month the month
 * @param[in] day the day
 * @param[in] hour the hour
 * @param[in] minute the minute
 * @param[in] second the second
 * @param[in] microsecond the microsecond
 */
void DateTime::initialise(int year, int month, int day, int hour, int minute, int second, int microsecond) {
    if (!IsValidYearMonthDay(year, month, day) ||
        hour < 0 || hour > 23 ||
        minute < 0 || minute > 59 ||
        second < 0 || second > 59 ||
        microsecond < 0 || microsecond > 999999) {
        throw 1;
    }
    m_encoded = TimeSpan(AbsoluteDays(year, month, day),
                         hour,
                         minute,
                         second,
                         microsecond).getTicks();
}

/**
 * Return the current time
 * @param[in] microseconds whether to set the microsecond component
 * @returns a DateTime object set to the current date and time
 */
DateTime DateTime::now(bool microseconds) {
    DateTime dt;
    struct timespec ts;
    
    if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
        if (microseconds) {
            dt = DateTime(TimeSpan::UNIX_EPOCH
                          + ts.tv_sec * TimeSpan::TICKS_PER_SECOND
                          + ts.tv_nsec / 1000LL * TimeSpan::TICKS_PER_MICROSECOND);
        }
        else {
            dt = DateTime(TimeSpan::UNIX_EPOCH
                          + ts.tv_sec * TimeSpan::TICKS_PER_SECOND);
        }
    }
    else {
        throw 1;
    }
    
    return dt;
}

DateTime::~DateTime() {
}

TimeSpan DateTime::getTimeOfDay() const {
    return TimeSpan(getTicks() % TimeSpan::TICKS_PER_DAY);
}

DateTime DateTime::addYears(const int years) const {
    return addMonths(years * 12);
}

DateTime DateTime::addMonths(const int months) const {
    int year, month, day;
    TimeOps::toDMY(*this, day, month, year);
    month += months % 12;
    year += months / 12;
    
    if (month < 1)
    {
        month += 12;
        --year;
    }
    else if (month > 12)
    {
        month -= 12;
        ++year;
    }
    
    int maxday = DaysInMonth(year, month);
    day = MIN(day, maxday);
    
    return DateTime(year, month, day).add(getTimeOfDay());
}

/**
 * Add a TimeSpan to this DateTime
 * @param[in] t the TimeSpan to add
 * @returns a DateTime which has the given TimeSpan added
 */
DateTime DateTime::add(const TimeSpan& t) const {
    return addTicks(t.getTicks());
}

DateTime DateTime::addDays(const double days) const {
    return addMicroseconds(days * 86400000000.0);
}

DateTime DateTime::addHours(const double hours) const {
    return addMicroseconds(hours * 3600000000.0);
}

DateTime DateTime::addMinutes(const double minutes) const {
    return addMicroseconds(minutes * 60000000.0);
}

DateTime DateTime::addSeconds(const double seconds) const {
    return addMicroseconds(seconds * 1000000.0);
}

DateTime DateTime::addMicroseconds(const double microseconds) const {
    int64_t ticks = static_cast<int64_t>(microseconds * TimeSpan::TICKS_PER_MICROSECOND);
    return addTicks(ticks);
}

DateTime DateTime::addTicks(int64_t ticks) const {
    return DateTime(m_encoded + ticks);
}

/**
 * Get the number of ticks
 * @returns the number of ticks
 */
int64_t DateTime::getTicks() const {
    return m_encoded;
}

int DateTime::getYear() const {
    int year, month, day;
    TimeOps::toDMY(*this, day, month, year);
    return year;
}

int DateTime::getMonth() const {
    int year, month, day;
    TimeOps::toDMY(*this, day, month, year);
    return month;
}

int DateTime::getDay() const {
    int year, month, day;
    TimeOps::toDMY(*this, day, month, year);
    return day;
}

/**
 * Hour component
 * @returns the hour component
 */
int DateTime::getHour() const {
    return static_cast<int>(m_encoded % TimeSpan::TICKS_PER_DAY / TimeSpan::TICKS_PER_HOUR);
}

/**
 * Minute component
 * @returns the minute component
 */
int DateTime::getMinute() const {
    return static_cast<int>(m_encoded % TimeSpan::TICKS_PER_HOUR / TimeSpan::TICKS_PER_MINUTE);
}

/**
 * Second component
 * @returns the Second component
 */
int DateTime::getSecond() const {
    return static_cast<int>(m_encoded % TimeSpan::TICKS_PER_MINUTE / TimeSpan::TICKS_PER_SECOND);
}

/**
 * Microsecond component
 * @returns the microsecond component
 */
int DateTime::getMicrosecond() const {
    return static_cast<int>(m_encoded % TimeSpan::TICKS_PER_SECOND / TimeSpan::TICKS_PER_MICROSECOND);
}

DateTime DateTime::operator+ (const TimeSpan& _ts) const {
    return DateTime(getTicks() + _ts.getTicks());
}

DateTime DateTime::operator+ (const DateTime& _dt) const {
    return DateTime(getTicks() + _dt.getTicks());
}

DateTime DateTime::operator- (const TimeSpan& _ts) const {
    return DateTime(getTicks() - _ts.getTicks());
}

DateTime DateTime::operator- (const DateTime& _dt) const {
    return DateTime(getTicks() - _dt.getTicks());
}

bool DateTime::operator==(const DateTime& _dt) const {
    return m_encoded == _dt.getTicks();
}

bool DateTime::operator!=(const DateTime& _dt) const {
    return m_encoded != _dt.getTicks();
}

bool DateTime::operator>(const DateTime& _dt) const {
    return m_encoded > _dt.getTicks();
}

bool DateTime::operator>=(const DateTime& _dt) const {
    return m_encoded >= _dt.getTicks();
}

bool DateTime::operator<(const DateTime& _dt) const {
    return m_encoded < _dt.getTicks();
}

bool DateTime::operator<=(const DateTime& _dt) const {
    return m_encoded <= _dt.getTicks();
}
