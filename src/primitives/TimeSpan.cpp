//
//  TimeSpan.cpp
//  Solar
//
//  Created by Patricio González Vivo on 2/13/18.
//

#include "TimeSpan.h"

#include <cmath>

const int64_t TimeSpan::TICKS_PER_DAY =  86400000000LL;
const int64_t TimeSpan::TICKS_PER_HOUR =  3600000000LL;
const int64_t TimeSpan::TICKS_PER_MINUTE =  60000000LL;
const int64_t TimeSpan::TICKS_PER_SECOND =   1000000LL;
const int64_t TimeSpan::TICKS_PER_MILLISECOND = 1000LL;
const int64_t TimeSpan::TICKS_PER_MICROSECOND =    1LL;
const int64_t TimeSpan::UNIX_EPOCH = 62135596800000000LL;
const int64_t TimeSpan::GREGORIAN_EPOCH = 49916304000000000LL; // 1582-Oct-15
const int64_t TimeSpan::MAX_VALUE_TICKS = 315537897599999999LL;

TimeSpan::TimeSpan(int64_t ticks) : m_ticks(ticks) {
}

TimeSpan::TimeSpan(int hours, int minutes, int seconds) {
    updateTicks(0, hours, minutes, seconds, 0);
}

TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds) {
    updateTicks(days, hours, minutes, seconds, 0);
}

TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int microseconds) {
    updateTicks(days, hours, minutes, seconds, microseconds);
}

TimeSpan TimeSpan::operator+ (const TimeSpan& ts) const {
    return TimeSpan(m_ticks + ts.getTicks());
}

TimeSpan TimeSpan::operator- (const TimeSpan& ts) const {
     return TimeSpan(m_ticks - ts.getTicks());
}

TimeSpan::~TimeSpan() {
}

int TimeSpan::compare(const TimeSpan& ts) const {
    int ret = 0;
    
    if (m_ticks < ts.m_ticks) {
        ret = -1;
    }
    
    if (m_ticks > ts.m_ticks) {
        ret = 1;
    }
    return ret;
}

bool TimeSpan::equals(const TimeSpan& ts) const {
    return m_ticks == ts.m_ticks;
}

int TimeSpan::getDays() const {
    return static_cast<int>(m_ticks / TICKS_PER_DAY);
}

int TimeSpan::getHours() const {
    return static_cast<int>(m_ticks % TICKS_PER_DAY / TICKS_PER_HOUR);
}

int TimeSpan::getMinutes() const {
    return static_cast<int>(m_ticks % TICKS_PER_HOUR / TICKS_PER_MINUTE);
}

int TimeSpan::getSeconds() const {
    return static_cast<int>(m_ticks % TICKS_PER_MINUTE / TICKS_PER_SECOND);
}

int TimeSpan::getMilliseconds() const {
    return static_cast<int>(m_ticks % TICKS_PER_SECOND / TICKS_PER_MILLISECOND);
}

int TimeSpan::getMicroseconds() const {
    return static_cast<int>(m_ticks % TICKS_PER_SECOND / TICKS_PER_MICROSECOND);
}

int64_t TimeSpan::getTicks() const {
    return m_ticks;
}

double TimeSpan::getTotalDays() const {
    return static_cast<double>(m_ticks) / TICKS_PER_DAY;
}

double TimeSpan::getTotalHours() const {
    return static_cast<double>(m_ticks) / TICKS_PER_HOUR;
}

double TimeSpan::getTotalMinutes() const {
    return static_cast<double>(m_ticks) / TICKS_PER_MINUTE;
}

double TimeSpan::getTotalSeconds() const {
    return static_cast<double>(m_ticks) / TICKS_PER_SECOND;
}

double TimeSpan::getTotalMilliseconds() const {
    return static_cast<double>(m_ticks) / TICKS_PER_MILLISECOND;
}

double TimeSpan::getTotalMicroseconds() const {
    return static_cast<double>(m_ticks) / TICKS_PER_MICROSECOND;
}

void TimeSpan::updateTicks(int days, int hours, int minutes, int seconds, int microseconds) {
    m_ticks = days * TICKS_PER_DAY +
    (hours * 3600LL + minutes * 60LL + seconds) * TICKS_PER_SECOND +
    microseconds * TICKS_PER_MICROSECOND;
}

