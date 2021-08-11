#include "time.hpp"
#include <chrono>
#include <algorithm>

Tristan::Time::Time::Time(Tristan::Time::Precision precision) :
        m_precision(precision),
        m_offset({0, 0})
{
    auto time_point = std::chrono::system_clock::now();
    auto duration_in_hours = std::chrono::duration_cast<std::chrono::hours>(time_point.time_since_epoch());
    m_durations.m_hours = std::chrono::hours(duration_in_hours.count() % 24);
    auto duration_in_minutes = std::chrono::duration_cast<std::chrono::minutes>(time_point.time_since_epoch());
    m_durations.m_minutes = std::chrono::minutes(duration_in_minutes.count() % 60);
    if (m_precision <= Tristan::Time::Precision::SECONDS) {
        auto duration_in_seconds = std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch());
        m_durations.m_seconds = std::chrono::seconds(duration_in_seconds.count() % 60);
    }
    if (m_precision <= Tristan::Time::Precision::MILLISECONDS){
        auto duration_in_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch());
        m_durations.m_milliseconds = std::chrono::milliseconds(duration_in_milliseconds.count() % 1000);
    }
    if (m_precision <= Tristan::Time::Precision::MICROSECONDS){
        auto duration_in_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(time_point.time_since_epoch());
        m_durations.m_microseconds = std::chrono::microseconds(duration_in_microseconds.count() % 1000);
    }
    if (m_precision == Tristan::Time::Precision::NANOSECONDS){
        auto duration_in_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time_point.time_since_epoch());
        m_durations.m_nanoseconds = std::chrono::nanoseconds(duration_in_nanoseconds.count() % 1000);
    }
}

Tristan::Time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    if (hours > 23)
    {
        std::string message = "Tristan::Time::Time(int hours, int minutes, int seconds): bad [hour] value was provided - "
                + std::to_string(hours) + ". The value from 0 to 23 is expected";
        throw std::range_error(message);
    }
    if (minutes > 59)
    {
        std::string message = "Tristan::Time::Time(int hours, int minutes, int seconds): bad [minutes] value was provided - "
                + std::to_string(minutes) + ". The value from 0 to 59 is expected";
        throw std::range_error(message);
    }
    if (seconds > 59)
    {
        std::string message = "Tristan::Time::Time(int hours, int minutes, int seconds): bad [seconds] value was provided - "
                + std::to_string(seconds) + ". The value from 0 to 59 is expected";
        throw std::range_error(message);
    }
    m_durations.m_hours = std::chrono::hours(hours);
    m_durations.m_minutes = std::chrono::hours(minutes);
    m_durations.m_seconds = std::chrono::hours(seconds);
    
    m_precision = Tristan::Time::Precision::SECONDS;
}

Tristan::Time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) :
        Tristan::Time::Time(hours, minutes, seconds)
{
    if (milliseconds > 1000)
    {
        std::string message = "Tristan::Time::Time(int hours, int minutes, int seconds, uint16_t milliseconds): bad [milliseconds] value was provided - "
                + std::to_string(seconds) + ". The value from 0 to 1000 is expected";
        throw std::range_error(message);
    }
    m_durations.m_milliseconds = std::chrono::milliseconds(milliseconds);
    
    m_precision = Tristan::Time::Precision::MILLISECONDS;
}

Tristan::Time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds,
                          uint16_t microseconds) :
        Tristan::Time::Time(hours, minutes, seconds, milliseconds)
{
    if (microseconds > 1000)
    {
        std::string message = "Tristan::Time::Time(int hours, int minutes, int seconds, uint16_t milliseconds): bad [microseconds] value was provided - "
                + std::to_string(seconds) + ". The value from 0 to 1000 is expected";
        throw std::range_error(message);
    }
    m_durations.m_microseconds = std::chrono::microseconds(microseconds);
    
    m_precision = Tristan::Time::Precision::MICROSECONDS;
}

Tristan::Time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds,
                          uint16_t nanoseconds) :
                          Tristan::Time::Time(hours, minutes, seconds, milliseconds, microseconds)
{
    if (nanoseconds > 1000)
    {
        std::string message = "Tristan::Time::Time(int hours, int minutes, int seconds, uint16_t milliseconds): bad [nanoseconds] value was provided - "
                + std::to_string(seconds) + ". The value from 0 to 1000 is expected";
        throw std::range_error(message);
    }
    m_durations.m_nanoseconds = std::chrono::nanoseconds (nanoseconds);
    
    m_precision = Tristan::Time::Precision::NANOSECONDS;
}

Tristan::Time::Time::Time(const std::string &iso_Time)
{
    auto size = iso_Time.length();
    if (size != 6 && size != 8)
    {
        std::string message = "Tristan::Time::Time(const std::string &iso_Time): Bad [iso Time] string length. \
                               Length supposed to be 6 or 8, but string of length "+ std::to_string(size) + " was provided";
        throw std::length_error(message);
    }
    if (std::find_if(iso_Time.begin(), iso_Time.end(), [](const char c){if (c < '0' || c > '9')
                                                                            if (c != ':') return true;
                                                                        return false;}) != iso_Time.end())
    {
        std::string message = "Tristan::Time::Time(const std::string &iso_Time): Bad [iso Time] string format. \
                               String should contain only numbers and colon";
        throw std::invalid_argument(message);
    }
    m_hour = std::stoi(iso_Time.substr(0, 2));
    switch (size)
    {
    case 6:
    {
        m_min = std::stoi(iso_Time.substr(2, 2));
        m_sec = std::stoi(iso_Time.substr(4, 2));
        break;
    }
    case 8:
    {
        m_min = std::stoi(iso_Time.substr(3, 2));
        m_sec = std::stoi(iso_Time.substr(6, 2));
        break;
    }
    }
}

bool Tristan::Time::Time::operator==(const Tristan::Time::Time &r) const
{
    if (m_min != r.m_min)
        return false;
    if (m_hour != r.m_hour)
        return false;

    return true;
}

bool Tristan::Time::Time::operator<(const Tristan::Time::Time &r) const
{
    if (m_hour < r.m_hour)
        return true;
    if (m_hour == r.m_hour)
        if (m_min < r.m_min)
            return true;

    return false;
}

Tristan::Time::Time &Tristan::Time::Time::operator++()
{
    m_min < 59 ? ++m_min : (m_min = 0, m_hour < 23 ? ++m_hour : m_hour = 0);

    return *this;
}

Tristan::Time::Time Tristan::Time::Time::operator++(int)
{
    Time temp(*this);
    ++*this;
    return temp;
}

Tristan::Time::Time &Tristan::Time::Time::operator--()
{
    m_min > 0 ? --m_min : (m_min = 59, m_hour > 0 ? --m_hour : m_hour = 23);

    return *this;
}

Tristan::Time::Time Tristan::Time::Time::operator--(int)
{
    Time temp(*this);
    --*this;
    return temp;
}

void Tristan::Time::Time::setHour(int hour)
{
    if (hour < 0 || hour > 23)
    {
        std::string message = "Tristan::Time::Time::set_hour(int hour): bad [hour] value was provided - "
                + std::to_string(hour) + ". The value from 0 to 23 is expected";
        throw std::range_error(message);
    }
    m_hour = hour;
}

void Tristan::Time::Time::setMin(int minute)
{
    if (minute < 0 || minute > 60)
    {
        std::string message = "Tristan::Time::Time::set_min(int minute): bad [minutes] value was provided - "
                + std::to_string(minute) + ". The value from 0 to 59 is expected";
        throw std::range_error(message);
    }
    m_min = minute;
}

int Tristan::Time::Time::hours() const
{
    return m_hour;
}

int Tristan::Time::Time::minutes() const
{
    return m_min;
}

int Tristan::Time::Time::seconds() const
{
    return m_sec;
}

std::string Tristan::Time::Time::toString() const
{
    std::string _Time;
    m_hour < 10 ? (_Time += '0' + std::to_string(m_hour) + ':') : _Time += std::to_string(m_hour) + ':';
    m_min < 10 ? (_Time += '0' + std::to_string(m_min) + ':') : _Time += std::to_string(m_min) + ':';
    m_sec < 10 ? (_Time += '0' + std::to_string(m_sec)) : _Time += std::to_string(m_sec);

    return _Time;
}

void Tristan::Time::Time::_setSystemOffset() {
    if (m_time_since_epoch_nanoseconds == std::chrono::duration<uint64_t, std::nano>(0)){
        return;
    }
    
}

bool Tristan::Time::operator !=(const Tristan::Time::Time &l, const Tristan::Time::Time &r)
{
    return !(l==r);
}

bool Tristan::Time::operator >(const Tristan::Time::Time &l, const Tristan::Time::Time &r)
{
    return !(l <= r);
}

bool Tristan::Time::operator <=(const Tristan::Time::Time &l, const Tristan::Time::Time &r)
{
    return (l < r || l == r);
}

bool Tristan::Time::operator >=(const Tristan::Time::Time &l, const Tristan::Time::Time &r)
{
                    return (l > r || l == r);
}

std::ostream &Tristan::Time::operator<<(std::ostream &out, const Tristan::Time::Time &Time)
{
    out << Time.toString();
    return out;
}

bool Tristan::Time::is_equal(const Tristan::Time::Time &l, const Tristan::Time::Time &r)
{
    if (l.minutes() != r.minutes())
        return false;
    if (l.hours() != r.hours())
        return false;
    if (l.seconds() != r.seconds())
        return false;

    return true;
}
