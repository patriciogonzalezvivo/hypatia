#pragma once

#include "TimeSpan.h"

/**
 *  @brief Represents an instance in time.
 *  Default contructor initialise to 0001/01/01 00:00:00.000000
 */
class DateTime {
public:

    DateTime();
    DateTime(int64_t ticks);
    DateTime(unsigned int year, double doy);
    DateTime(int year, int month, int day);
    DateTime(int year, int month, int day, int hour, int minute, int second);
    virtual ~DateTime();
    
    virtual TimeSpan getTimeOfDay() const;
    virtual int64_t getTicks() const;
    
    virtual int getYear() const;
    virtual int getMonth() const;
    virtual int getDay() const;
    virtual int getHour() const;
    virtual int getMinute() const;
    virtual int getSecond() const;
    virtual int getMicrosecond() const;
    
    static DateTime now(bool microseconds = false);

    virtual DateTime add(const TimeSpan& t) const;    
    virtual DateTime addYears(const int years) const;
    virtual DateTime addMonths(const int months) const;
    virtual DateTime addDays(const double days) const;
    virtual DateTime addHours(const double hours) const;
    virtual DateTime addMinutes(const double minutes) const;
    virtual DateTime addSeconds(const double seconds) const;
    virtual DateTime addMicroseconds(const double microseconds) const;
    virtual DateTime addTicks(int64_t ticks) const;
    
    virtual DateTime operator+ (const TimeSpan& _ts) const;
    virtual DateTime operator+ (const DateTime& _dt) const;
    virtual DateTime operator- (const TimeSpan& _ts) const;
    virtual DateTime operator- (const DateTime& _dt) const;
    
    virtual bool operator==(const DateTime& _dt) const;
    virtual bool operator!=(const DateTime& _dt) const;
    virtual bool operator>(const DateTime& dt) const;
    virtual bool operator>=(const DateTime& _dt) const;
    virtual bool operator<(const DateTime& _dt) const;
    virtual bool operator<=(const DateTime& _dt) const;
    
protected:
    void initialise(int year, int month, int day, int hour, int minute, int second, int microsecond);
    
    int64_t m_encoded;
};

inline std::ostream& operator<<(std::ostream& strm, const DateTime& _dt) {
    strm << std::right << std::setfill('0');
    strm << std::setw(4) << _dt.getYear() << "-";
    strm << std::setw(2) << _dt.getMonth() << "-";
    strm << std::setw(2) << _dt.getDay() << " ";
    strm << std::setw(2) << _dt.getHour() << ":";
    strm << std::setw(2) << _dt.getMinute() << ":";
    strm << std::setw(2) << _dt.getSecond() << ".";
    strm << std::setw(6) << _dt.getMicrosecond() << " UTC";
    return strm;
}
