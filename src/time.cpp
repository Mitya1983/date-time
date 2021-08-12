#include "time.hpp"
#include <chrono>
#include <algorithm>

namespace
{
    auto checkTimeFormat(const std::string& time) -> bool;
} //End of unnamed namespace

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

Tristan::Time::Time::Time(uint8_t hours, uint8_t minutes) noexcept(false) :
        m_precision(Tristan::Time::Precision::MINUTES),
        m_offset({0, 0})
{
    if (hours > 23){
        std::string message = "Tristan::Time::Time(int hours, int minutes, int seconds): bad [hour] value was provided - "
                              + std::to_string(hours) + ". The value from 0 to 23 is expected";
        throw std::range_error(message);
    }
    if (minutes > 59){
        std::string message = "Tristan::Time::Time(int hours, int minutes, int seconds): bad [minutes] value was provided - "
                              + std::to_string(minutes) + ". The value from 0 to 59 is expected";
        throw std::range_error(message);
    }
    m_durations.m_hours = std::chrono::hours(hours);
    m_durations.m_minutes = std::chrono::hours(minutes);
}

Tristan::Time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds) :
        Tristan::Time::Time(hours, minutes)
{
    if (seconds > 59){
        std::string message = "Tristan::Time::Time(int hours, int minutes, int seconds): bad [seconds] value was provided - "
                              + std::to_string(seconds) + ". The value from 0 to 59 is expected";
        throw std::range_error(message);
    }
    m_durations.m_seconds = std::chrono::hours(seconds);
    
    m_precision = Tristan::Time::Precision::SECONDS;
}

Tristan::Time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) :
        Tristan::Time::Time(hours, minutes, seconds)
{
    if (milliseconds > 1000){
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
    if (microseconds > 1000){
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
    if (nanoseconds > 1000){
        std::string message = "Tristan::Time::Time(int hours, int minutes, int seconds, uint16_t milliseconds): bad [nanoseconds] value was provided - "
                              + std::to_string(seconds) + ". The value from 0 to 1000 is expected";
        throw std::range_error(message);
    }
    m_durations.m_nanoseconds = std::chrono::nanoseconds(nanoseconds);
    
    m_precision = Tristan::Time::Precision::NANOSECONDS;
}

Tristan::Time::Time::Time(const std::string& time){
    auto size = time.length();
    
    if (!checkTimeFormat(time)){
        throw std::invalid_argument("Tristan::Time::Time::Time(const std::string& time): Invalid time format");
    }
    
    const uint8_t hours_pos = 0;
    const uint8_t minutes_pos = 3;
    const uint8_t seconds_pos = 6;
    const uint8_t milliseconds_pos = 9;
    const uint8_t microseconds_pos = 14;
    const uint8_t nanoseconds_pos = 9;
    m_precision = Tristan::Time::Precision::MINUTES;
    switch (size){
        case 23:{
            m_durations.m_nanoseconds = std::chrono::nanoseconds (std::stoi(time.substr(nanoseconds_pos, 4)));
            m_precision = Tristan::Time::Precision::NANOSECONDS;
            [[fallthrough]];
        }
        case 18:{
            m_durations.m_microseconds = std::chrono::microseconds (std::stoi(time.substr(microseconds_pos, 4)));
            if (m_precision == Tristan::Time::Precision::MINUTES){
                m_precision = Tristan::Time::Precision::MICROSECONDS;
            }
            [[fallthrough]];
        }
        case 13:{
            m_durations.m_milliseconds = std::chrono::milliseconds (std::stoi(time.substr(milliseconds_pos, 4)));
            if (m_precision == Tristan::Time::Precision::MINUTES){
                m_precision = Tristan::Time::Precision::MILLISECONDS;
            }
            [[fallthrough]];
        }
        case 8:{
            m_durations.m_seconds = std::chrono::seconds (std::stoi(time.substr(seconds_pos, 2)));
            if (m_precision == Tristan::Time::Precision::MINUTES){
                m_precision = Tristan::Time::Precision::SECONDS;
            }
            [[fallthrough]];
        }
        case 5:{
            m_durations.m_hours = std::chrono::hours(std::stoi(time.substr(hours_pos, 2)));
            m_durations.m_minutes = std::chrono::minutes (std::stoi(time.substr(minutes_pos, 2)));
            break;
        }
        default:{
            throw std::invalid_argument("Tristan::Time::Time::Time(const std::string& time): Invalid time format");
        }
    }
}

bool Tristan::Time::Time::operator==(const Tristan::Time::Time &r) const{
    
    auto comparable_precision = std::min(this->m_precision, r.m_precision);
    
    switch (comparable_precision){
        case Tristan::Time::Precision::NANOSECONDS:{
            if (this->m_durations.m_nanoseconds != r.m_durations.m_nanoseconds){
                return false;
            }
            [[fallthrough]];
        }
        case Tristan::Time::Precision::MICROSECONDS:{
            if (this->m_durations.m_microseconds != r.m_durations.m_microseconds){
                return false;
            }
            [[fallthrough]];
        }
        case Tristan::Time::Precision::MILLISECONDS:{
            if (this->m_durations.m_milliseconds != r.m_durations.m_milliseconds){
                return false;
            }
            [[fallthrough]];
        }
        case Tristan::Time::Precision::SECONDS:{
            if (this->m_durations.m_seconds != r.m_durations.m_seconds){
                return false;
            }
            [[fallthrough]];
        }
        case Tristan::Time::Precision::MINUTES:{
            if (this->m_durations.m_minutes != r.m_durations.m_minutes){
                return false;
            }
            if (this->m_durations.m_hours != r.m_durations.m_hours){
                return false;
            }
            break;
        }
    }
    
    return true;
}

bool Tristan::Time::Time::operator<(const Tristan::Time::Time &r) const
{
    auto comparable_precision = std::min(this->m_precision, r.m_precision);
    
    if (this->m_durations.m_hours < r.m_durations.m_hours){
        return true;
    }
    if (this->m_durations.m_hours > r.m_durations.m_hours){
        return false;
    }
    //Hours are equal
    if (this->m_durations.m_minutes < r.m_durations.m_minutes){
        return true;
    }
    if (this->m_durations.m_minutes > r.m_durations.m_minutes){
        return false;
    }
    //Minutes are equal
    if (comparable_precision == Tristan::Time::Precision::SECONDS){
        if (this->m_durations.m_seconds < r.m_durations.m_seconds){
            return true;
        }
        if (this->m_durations.m_seconds > r.m_durations.m_seconds){
            return false;
        }
    }
    //Seconds are equal
    if (comparable_precision == Tristan::Time::Precision::MILLISECONDS){
        if (this->m_durations.m_milliseconds < r.m_durations.m_milliseconds){
            return true;
        }
        if (this->m_durations.m_milliseconds > r.m_durations.m_milliseconds){
            return false;
        }
    }
    //Milliseconds are equal
    if (comparable_precision == Tristan::Time::Precision::MICROSECONDS){
        if (this->m_durations.m_microseconds < r.m_durations.m_microseconds){
            return true;
        }
        if (this->m_durations.m_microseconds > r.m_durations.m_microseconds){
            return false;
        }
    }
    //Microseconds are equal
    if (comparable_precision == Tristan::Time::Precision::NANOSECONDS){
        if (this->m_durations.m_nanoseconds < r.m_durations.m_nanoseconds){
            return true;
        }
        if (this->m_durations.m_nanoseconds > r.m_durations.m_nanoseconds){
            return false;
        }
    }
    //If got here all values are equal
    return false;
}

void Tristan::Time::Time::operator+=(const Tristan::Time::Time& r){
    //TODO
}

void Tristan::Time::Time::operator-=(const Tristan::Time::Time& r){
    //TODO
}

std::string Tristan::Time::Time::toString() const{
    std::string _Time;

    
    
    return _Time;
}

void Tristan::Time::Time::setOffset(Tristan::Time::TimeOffset offset){
    //TODO
}

void Tristan::Time::Time::addHours(uint64_t hours){
    //TODO
}

void Tristan::Time::Time::addMinutes(uint64_t minutes){
    //TODO
}

void Tristan::Time::Time::addSeconds(uint64_t seconds){
    //TODO
}

void Tristan::Time::Time::addMilliseconds(uint64_t milliseconds){
    //TODO
}

void Tristan::Time::Time::addMicroseconds(uint64_t microseconds){
    //TODO
}

void Tristan::Time::Time::addNanoseconds(uint64_t nanoseconds){
    //TODO
}

auto Tristan::Time::Time::localTime() const -> Tristan::Time::Time{
    //TODO
}

bool Tristan::Time::operator !=(const Tristan::Time::Time &l, const Tristan::Time::Time &r){
    return !(l==r);
}

bool Tristan::Time::operator >(const Tristan::Time::Time &l, const Tristan::Time::Time &r){
    return !(l <= r);
}

bool Tristan::Time::operator <=(const Tristan::Time::Time &l, const Tristan::Time::Time &r){
    return (l < r || l == r);
}

bool Tristan::Time::operator >=(const Tristan::Time::Time &l, const Tristan::Time::Time &r){
    return (l > r || l == r);
}

std::ostream &Tristan::Time::operator<<(std::ostream &out, const Tristan::Time::Time &Time){
    out << Time.toString();
    return out;
}

auto Tristan::Time::operator+(const Tristan::Time::Time& l, const Tristan::Time::Time& r) -> Tristan::Time::Time{
    //TODO
}

auto Tristan::Time::operator-(const Tristan::Time::Time& l, const Tristan::Time::Time& r) -> Tristan::Time::Time{
    //TODO
}

namespace
{
    auto checkTimeFormat(const std::string& time) -> bool{
        
        auto length = time.length();
        if (length < 5 || length > 23) {
            return false;
        }
        for (size_t i = 0; i < length; ++i) {
            if ((time[i] < '0' || time[i] > '9') && time[i] != ':') {
                return false;
            }
            if (i < 2 && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
            if (i == 2 && time[i] != ':') {
                return false;
            }
            if ((i > 2 && i < 5) && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
            if (i == 5 && time[i] != ':') {
                return false;
            }
            if ((i > 5 && i < 8) && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
            if (i == 8 && time[i] != ':') {
                return false;
            }
            if ((i > 8 && i < 13) && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
            if (i == 13 && time[i] != ':') {
                return false;
            }
            if ((i > 13 && i < 18) && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
            if (i == 18 && time[i] != ':') {
                return false;
            }
            if ((i > 18 && i < 23) && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
        }
        return true;
    }
} //End of unnamed namespace