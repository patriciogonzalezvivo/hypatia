#pragma once

#include <stdint.h>
#include <iomanip>

/**
 * @brief Represents a time interval.
 *
 * Represents a time interval (duration/elapsed) that is measured as a positive
 * or negative number of days, hours, minutes, seconds, and fractions
 * of a second.
 */
class TimeSpan{
public:
    TimeSpan(int64_t ticks);
    TimeSpan(int hours, int minutes, int seconds);
    TimeSpan(int days, int hours, int minutes, int seconds);
    TimeSpan(int days, int hours, int minutes, int seconds, int microseconds);
    virtual ~TimeSpan();
    
    virtual TimeSpan operator+ (const TimeSpan& ts) const;
    virtual TimeSpan operator- (const TimeSpan& ts) const;
    
    virtual int compare(const TimeSpan& ts) const;
    virtual bool equals(const TimeSpan& ts) const;
    
    virtual int64_t getTicks() const;
    
    virtual int getDays() const;
    virtual int getHours() const;
    virtual int getMinutes() const;
    virtual int getSeconds() const;
    virtual int getMilliseconds() const;
    virtual int getMicroseconds() const;
    
    virtual double getTotalDays() const;
    virtual double getTotalHours() const;
    virtual double getTotalMinutes() const;
    virtual double getTotalSeconds() const;
    virtual double getTotalMilliseconds() const;
    virtual double getTotalMicroseconds() const;
    
    static const int64_t TICKS_PER_DAY;
    static const int64_t TICKS_PER_HOUR;
    static const int64_t TICKS_PER_MINUTE;
    static const int64_t TICKS_PER_SECOND;
    static const int64_t TICKS_PER_MILLISECOND;
    static const int64_t TICKS_PER_MICROSECOND;
    static const int64_t UNIX_EPOCH;
    static const int64_t GREGORIAN_EPOCH; // 1582-Oct-15
    static const int64_t MAX_VALUE_TICKS;
    
protected:
    int64_t m_ticks;
    
    void updateTicks(int days, int hours, int minutes, int seconds, int microseconds);
};

inline std::ostream& operator<<(std::ostream& strm, const TimeSpan& _ts) {
    strm << std::right << std::setfill('0');
    
    if (_ts.getTicks() < 0) {
        strm << '-';
    }
    
    if (_ts.getDays() != 0) {
        strm << std::setw(2) << std::abs(_ts.getDays()) << '.';
    }
    
    strm << std::setw(2) << std::abs(_ts.getHours()) << ':';
    strm << std::setw(2) << std::abs(_ts.getMinutes()) << ':';
    strm << std::setw(2) << std::abs(_ts.getSeconds());
    
    if (_ts.getMicroseconds() != 0) {
        strm << '.' << std::setw(6) << std::abs(_ts.getMicroseconds());
    }
    
    return strm;
}

inline bool operator==(const TimeSpan& ts1, TimeSpan& ts2) {
    return ts1.equals(ts2);
}

inline bool operator>(const TimeSpan& ts1, const TimeSpan& ts2) {
    return (ts1.compare(ts2) > 0);
}

inline bool operator>=(const TimeSpan& ts1, const TimeSpan& ts2) {
    return (ts1.compare(ts2) >= 0);
}

inline bool operator!=(const TimeSpan& ts1, const TimeSpan& ts2) {
    return !ts1.equals(ts2);
}

inline bool operator<(const TimeSpan& ts1, const TimeSpan& ts2) {
    return (ts1.compare(ts2) < 0);
}

inline bool operator<=(const TimeSpan& ts1, const TimeSpan& ts2) {
    return (ts1.compare(ts2) <= 0);
}

