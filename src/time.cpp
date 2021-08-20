#include "time.hpp"
#include <chrono>
#include <algorithm>

namespace
{
    auto checkTimeFormat(const std::string& time) -> bool;
} //End of unnamed namespace

tristan::time::DayTime::DayTime(tristan::time::Precision precision) :
        m_precision(precision),
        m_offset(tristan::time::TimeZone::UTC),
        m_durations()
{
    auto time_point = std::chrono::system_clock::now();
    auto duration_in_hours = std::chrono::duration_cast<std::chrono::hours>(time_point.time_since_epoch());
    m_durations.m_hours = std::chrono::hours(duration_in_hours.count() % 24);
    auto duration_in_minutes = std::chrono::duration_cast<std::chrono::minutes>(time_point.time_since_epoch());
    m_durations.m_minutes = std::chrono::minutes(duration_in_minutes.count() % 60);
    if (m_precision <= tristan::time::Precision::SECONDS) {
        auto duration_in_seconds = std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch());
        m_durations.m_seconds = std::chrono::seconds(duration_in_seconds.count() % 60);
    }
    if (m_precision <= tristan::time::Precision::MILLISECONDS){
        auto duration_in_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch());
        m_durations.m_milliseconds = std::chrono::milliseconds(duration_in_milliseconds.count() % 1000);
    }
    if (m_precision <= tristan::time::Precision::MICROSECONDS){
        auto duration_in_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(time_point.time_since_epoch());
        m_durations.m_microseconds = std::chrono::microseconds(duration_in_microseconds.count() % 1000);
    }
    if (m_precision == tristan::time::Precision::NANOSECONDS){
        auto duration_in_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time_point.time_since_epoch());
        m_durations.m_nanoseconds = std::chrono::nanoseconds(duration_in_nanoseconds.count() % 1000);
    }
}

tristan::time::DayTime::DayTime(uint8_t hours, uint8_t minutes) noexcept(false) :
        m_precision(tristan::time::Precision::MINUTES),
        m_offset(tristan::time::TimeZone::UTC),
        m_durations()
{
    if (hours > 23){
        std::string message = "tristan::time::DayTime(int hours, int minutes, int seconds): bad [hour] value was provided - "
                              + std::to_string(hours) + ". The value from 0 to 23 is expected";
        throw std::range_error(message);
    }
    if (minutes > 59){
        std::string message = "tristan::time::DayTime(int hours, int minutes, int seconds): bad [minutes] value was provided - "
                              + std::to_string(minutes) + ". The value from 0 to 59 is expected";
        throw std::range_error(message);
    }
    m_durations.m_hours = std::chrono::hours(hours);
    m_durations.m_minutes = std::chrono::hours(minutes);
}

tristan::time::DayTime::DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds) :
        tristan::time::DayTime(hours, minutes)
{
    if (seconds > 59){
        std::string message = "tristan::time::DayTime(int hours, int minutes, int seconds): bad [seconds] value was provided - "
                              + std::to_string(seconds) + ". The value from 0 to 59 is expected";
        throw std::range_error(message);
    }
    m_durations.m_seconds = std::chrono::hours(seconds);
    
    m_precision = tristan::time::Precision::SECONDS;
}

tristan::time::DayTime::DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) :
        tristan::time::DayTime(hours, minutes, seconds)
{
    if (milliseconds > 1000){
        std::string message = "tristan::time::DayTime(int hours, int minutes, int seconds, uint16_t milliseconds): bad [milliseconds] value was provided - "
                              + std::to_string(seconds) + ". The value from 0 to 1000 is expected";
        throw std::range_error(message);
    }
    m_durations.m_milliseconds = std::chrono::milliseconds(milliseconds);
    
    m_precision = tristan::time::Precision::MILLISECONDS;
}

tristan::time::DayTime::DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds,
                                uint16_t microseconds) :
        tristan::time::DayTime(hours, minutes, seconds, milliseconds)
{
    if (microseconds > 1000){
        std::string message = "tristan::time::DayTime(int hours, int minutes, int seconds, uint16_t milliseconds): bad [microseconds] value was provided - "
                              + std::to_string(seconds) + ". The value from 0 to 1000 is expected";
        throw std::range_error(message);
    }
    m_durations.m_microseconds = std::chrono::microseconds(microseconds);
    
    m_precision = tristan::time::Precision::MICROSECONDS;
}

tristan::time::DayTime::DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds,
                                uint16_t nanoseconds) :
        tristan::time::DayTime(hours, minutes, seconds, milliseconds, microseconds)
{
    if (nanoseconds > 1000){
        std::string message = "tristan::time::DayTime(int hours, int minutes, int seconds, uint16_t milliseconds): bad [nanoseconds] value was provided - "
                              + std::to_string(seconds) + ". The value from 0 to 1000 is expected";
        throw std::range_error(message);
    }
    m_durations.m_nanoseconds = std::chrono::nanoseconds(nanoseconds);
    
    m_precision = tristan::time::Precision::NANOSECONDS;
}

tristan::time::DayTime::DayTime(const std::string& time) :
        m_offset(tristan::time::TimeZone::UTC),
        m_durations()
{
    auto size = time.length();
    
    if (!checkTimeFormat(time)){
        throw std::invalid_argument("tristan::time::DayTime::DayTime(const std::string& time): Invalid time format");
    }
    
    const uint8_t hours_pos = 0;
    const uint8_t minutes_pos = 3;
    const uint8_t seconds_pos = 6;
    const uint8_t milliseconds_pos = 9;
    const uint8_t microseconds_pos = 13;
    const uint8_t nanoseconds_pos = 17;
    m_precision = tristan::time::Precision::MINUTES;
    switch (size){
        case 20:{
            m_durations.m_nanoseconds = std::chrono::nanoseconds (std::stoi(time.substr(nanoseconds_pos, 3)));
            m_precision = tristan::time::Precision::NANOSECONDS;
            [[fallthrough]];
        }
        case 16:{
            m_durations.m_microseconds = std::chrono::microseconds (std::stoi(time.substr(microseconds_pos, 3)));
            if (m_precision == tristan::time::Precision::MINUTES){
                m_precision = tristan::time::Precision::MICROSECONDS;
            }
            [[fallthrough]];
        }
        case 12:{
            m_durations.m_milliseconds = std::chrono::milliseconds (std::stoi(time.substr(milliseconds_pos, 3)));
            if (m_precision == tristan::time::Precision::MINUTES){
                m_precision = tristan::time::Precision::MILLISECONDS;
            }
            [[fallthrough]];
        }
        case 8:{
            m_durations.m_seconds = std::chrono::seconds (std::stoi(time.substr(seconds_pos, 2)));
            if (m_precision == tristan::time::Precision::MINUTES){
                m_precision = tristan::time::Precision::SECONDS;
            }
            [[fallthrough]];
        }
        case 5:{
            m_durations.m_hours = std::chrono::hours(std::stoi(time.substr(hours_pos, 2)));
            m_durations.m_minutes = std::chrono::minutes (std::stoi(time.substr(minutes_pos, 2)));
            break;
        }
        default:{
            throw std::invalid_argument("tristan::time::DayTime::DayTime(const std::string& time): Invalid time format");
        }
    }
}

auto tristan::time::DayTime::operator==(const tristan::time::DayTime &r) const -> bool{
    
    auto comparable_precision = std::min(this->m_precision, r.m_precision);
    
    switch (comparable_precision){
        case tristan::time::Precision::NANOSECONDS:{
            if (this->m_durations.m_nanoseconds != r.m_durations.m_nanoseconds){
                return false;
            }
            [[fallthrough]];
        }
        case tristan::time::Precision::MICROSECONDS:{
            if (this->m_durations.m_microseconds != r.m_durations.m_microseconds){
                return false;
            }
            [[fallthrough]];
        }
        case tristan::time::Precision::MILLISECONDS:{
            if (this->m_durations.m_milliseconds != r.m_durations.m_milliseconds){
                return false;
            }
            [[fallthrough]];
        }
        case tristan::time::Precision::SECONDS:{
            if (this->m_durations.m_seconds != r.m_durations.m_seconds){
                return false;
            }
            [[fallthrough]];
        }
        case tristan::time::Precision::MINUTES:{
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

auto tristan::time::DayTime::operator<(const tristan::time::DayTime &r) const -> bool
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
    if (comparable_precision == tristan::time::Precision::SECONDS){
        if (this->m_durations.m_seconds < r.m_durations.m_seconds){
            return true;
        }
        if (this->m_durations.m_seconds > r.m_durations.m_seconds){
            return false;
        }
    }
    //Seconds are equal
    if (comparable_precision == tristan::time::Precision::MILLISECONDS){
        if (this->m_durations.m_milliseconds < r.m_durations.m_milliseconds){
            return true;
        }
        if (this->m_durations.m_milliseconds > r.m_durations.m_milliseconds){
            return false;
        }
    }
    //Milliseconds are equal
    if (comparable_precision == tristan::time::Precision::MICROSECONDS){
        if (this->m_durations.m_microseconds < r.m_durations.m_microseconds){
            return true;
        }
        if (this->m_durations.m_microseconds > r.m_durations.m_microseconds){
            return false;
        }
    }
    //Microseconds are equal
    if (comparable_precision == tristan::time::Precision::NANOSECONDS){
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

void tristan::time::DayTime::operator+=(const tristan::time::DayTime& r){
    *this = *this + r;
}

void tristan::time::DayTime::operator-=(const tristan::time::DayTime& r){
    *this = *this - r;
}

void tristan::time::DayTime::addHours(uint64_t hours){
    if (hours == 0){
        return;
    }
    uint8_t hours_to_add;
    if (hours > 23){
        hours_to_add = hours % 24;
    }
    else {
        hours_to_add = hours;
    }
    
    if (hours_to_add == 0){
        return;
    }
    m_durations.m_hours += std::chrono::hours(hours_to_add);
    if (m_durations.m_hours > std::chrono::hours(23)){
        m_durations.m_hours %= 24;
    }
}

void tristan::time::DayTime::addMinutes(uint64_t minutes){
    if (minutes == 0){
        return;
    }
    uint8_t minutes_to_add;
    uint8_t hours_to_add = 0;
    if (minutes > 59){
        hours_to_add = minutes / 60;
        minutes_to_add = minutes % 60;
    }
    else{
        minutes_to_add = minutes;
    }
    
    this->addHours(hours_to_add);
    if (minutes_to_add == 0){
        return;
    }
    m_durations.m_minutes += std::chrono::minutes(minutes_to_add);
    if (m_durations.m_minutes > std::chrono::minutes(59)){
        this->addHours(1);
        m_durations.m_minutes %= 60;
    }
}

void tristan::time::DayTime::addSeconds(uint64_t seconds){
    if (seconds == 0){
        return;
    }
    uint8_t seconds_to_add;
    uint8_t minutes_to_add = 0;
    if (seconds > 59){
        minutes_to_add = seconds / 60;
        seconds_to_add = seconds % 60;
    }
    else{
        seconds_to_add = seconds;
    }
    this->addMinutes(minutes_to_add);
    if (seconds_to_add == 0){
        return;
    }
    m_durations.m_seconds += std::chrono::seconds(seconds_to_add);
    
    if (m_durations.m_seconds > std::chrono::seconds(59)){
        this->addMinutes(1);
        m_durations.m_seconds %= 60;
    }
}

void tristan::time::DayTime::addMilliseconds(uint64_t milliseconds){
    if (milliseconds == 0){
        return;
    }
    uint8_t milliseconds_to_add;
    uint8_t seconds_to_add = 0;
    if (milliseconds > 999){
        seconds_to_add = milliseconds / 1000;
        milliseconds_to_add = milliseconds % 1000;
    }
    else{
        milliseconds_to_add = milliseconds;
    }
    this->addSeconds(seconds_to_add);
    if (milliseconds_to_add == 0){
        return;
    }
    m_durations.m_milliseconds += std::chrono::milliseconds(milliseconds_to_add);
    if (m_durations.m_milliseconds > std::chrono::milliseconds (1000)){
        this->addSeconds(1);
        m_durations.m_milliseconds %= 1000;
    }
}

void tristan::time::DayTime::addMicroseconds(uint64_t microseconds){
    if (microseconds == 0){
        return;
    }
    uint8_t microseconds_to_add;
    uint8_t milliseconds_to_add = 0;
    if (microseconds > 999){
        milliseconds_to_add = microseconds / 1000;
        microseconds_to_add = microseconds % 1000;
    }
    else{
        microseconds_to_add = microseconds;
    }
    this->addMilliseconds(milliseconds_to_add);
    if (microseconds_to_add == 0){
        return;
    }
    m_durations.m_microseconds += std::chrono::microseconds(microseconds_to_add);
    if (m_durations.m_microseconds > std::chrono::microseconds (1000)){
        this->addMilliseconds(1);
        m_durations.m_microseconds %= 1000;
    }
}

void tristan::time::DayTime::addNanoseconds(uint64_t nanoseconds){
    if (nanoseconds == 0){
        return;
    }
    uint8_t nanoseconds_to_add;
    uint8_t microseconds_to_add = 0;
    if (nanoseconds > 999){
        microseconds_to_add = nanoseconds / 1000;
        nanoseconds_to_add = nanoseconds % 1000;
    }
    else{
        nanoseconds_to_add = nanoseconds;
    }
    this->addMicroseconds(microseconds_to_add);
    if (nanoseconds_to_add == 0){
        return;
    }
    m_durations.m_nanoseconds += std::chrono::nanoseconds(nanoseconds_to_add);
    if (m_durations.m_nanoseconds > std::chrono::nanoseconds (1000)){
        this->addMicroseconds(1);
        m_durations.m_nanoseconds %= 1000;
    }
}

void tristan::time::DayTime::substractHours(uint64_t hours){
    if (hours == 0){
        return;
    }
    uint8_t hours_to_substract;
    if (hours > 23){
        hours_to_substract = hours % 24;
    }
    else {
        hours_to_substract = hours;
    }
    
    if (hours_to_substract == 0){
        return;
    }
    m_durations.m_hours -= std::chrono::hours(hours_to_substract);
    if (m_durations.m_hours < std::chrono::hours(0)){
        m_durations.m_hours += std::chrono::hours(24);
    }
    
}

void tristan::time::DayTime::substractMinutes(uint64_t minutes){
    if (minutes == 0){
        return;
    }
    uint8_t minutes_to_substract;
    uint8_t hours_to_substract = 0;
    if (minutes > 59){
        hours_to_substract = minutes / 60;
        minutes_to_substract = minutes % 60;
    }
    else{
        minutes_to_substract = minutes;
    }
    
    this->substractHours(hours_to_substract);
    if (minutes_to_substract == 0){
        return;
    }
    m_durations.m_minutes -= std::chrono::minutes(minutes_to_substract);
    if (m_durations.m_minutes < std::chrono::minutes(0)){
        this->substractHours(1);
        m_durations.m_minutes += std::chrono::minutes(60);
    }
}

void tristan::time::DayTime::substractSeconds(uint64_t seconds){
    if (seconds == 0){
        return;
    }
    uint8_t seconds_to_substract;
    uint8_t minutes_to_substract = 0;
    if (seconds > 59){
        minutes_to_substract = seconds / 60;
        seconds_to_substract = seconds % 60;
    }
    else{
        seconds_to_substract = seconds;
    }
    this->substractMinutes(minutes_to_substract);
    if (seconds_to_substract == 0){
        return;
    }
    m_durations.m_seconds -= std::chrono::seconds(seconds_to_substract);
    if (m_durations.m_seconds < std::chrono::seconds(0)){
        this->substractMinutes(1);
        m_durations.m_seconds += std::chrono::seconds(60);
    }
}

void tristan::time::DayTime::substractMilliseconds(uint64_t milliseconds){
    if (milliseconds == 0){
        return;
    }
    uint8_t milliseconds_to_substract;
    uint8_t seconds_to_substract = 0;
    if (milliseconds > 999){
        seconds_to_substract = milliseconds / 1000;
        milliseconds_to_substract = milliseconds % 1000;
    }
    else{
        milliseconds_to_substract = milliseconds;
    }
    this->addSeconds(seconds_to_substract);
    if (milliseconds_to_substract == 0){
        return;
    }
    m_durations.m_milliseconds -= std::chrono::milliseconds(milliseconds_to_substract);
    if (m_durations.m_milliseconds < std::chrono::milliseconds(0)){
        this->substractSeconds(1);
        m_durations.m_milliseconds += std::chrono::milliseconds(1000);
    }
}

void tristan::time::DayTime::substractMicroseconds(uint64_t microseconds){
    if (microseconds == 0){
        return;
    }
    uint8_t microseconds_to_substract;
    uint8_t milliseconds_to_substract = 0;
    if (microseconds > 999){
        milliseconds_to_substract = microseconds / 1000;
        microseconds_to_substract = microseconds % 1000;
    }
    else{
        microseconds_to_substract = microseconds;
    }
    this->substractMilliseconds(milliseconds_to_substract);
    if (microseconds_to_substract == 0){
        return;
    }
    m_durations.m_microseconds -= std::chrono::microseconds(microseconds_to_substract);
    if (m_durations.m_microseconds < std::chrono::microseconds(0)){
        this->substractMilliseconds(1);
        m_durations.m_microseconds += std::chrono::microseconds(1000);
    }
}

void tristan::time::DayTime::substractNanoseconds(uint64_t nanoseconds){
    if (nanoseconds == 0){
        return;
    }
    uint8_t nanoseconds_to_substract;
    uint8_t microseconds_to_substract = 0;
    if (nanoseconds > 999){
        microseconds_to_substract = nanoseconds / 1000;
        nanoseconds_to_substract = nanoseconds % 1000;
    }
    else{
        nanoseconds_to_substract = nanoseconds;
    }
    this->addMicroseconds(microseconds_to_substract);
    if (nanoseconds_to_substract == 0){
        return;
    }
    m_durations.m_nanoseconds -= std::chrono::nanoseconds(nanoseconds_to_substract);
    if (m_durations.m_nanoseconds < std::chrono::nanoseconds(0)){
        this->substractMicroseconds(1);
        m_durations.m_nanoseconds += std::chrono::nanoseconds(1000);
    }
}

auto tristan::time::DayTime::localTime(Precision precision) -> tristan::time::DayTime{
    tristan::time::DayTime l_time(precision);
    
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    
    tm local_time = *std::localtime(&time);
    tm utc_time = *std::gmtime(&time);
    
    auto hours = static_cast<int8_t>(local_time.tm_hour - utc_time.tm_hour);
    auto minutes = static_cast<int8_t>(local_time.tm_min - utc_time.tm_min);
    
    if (hours > 14 || hours < -14 || minutes > 59 || minutes < -59){
        throw std::range_error("tristan::time::DayTime::localTime(Precision precision): Timezone value is out of possible range");
    }
    
    if (hours >= 0) {
        l_time.addHours(hours);
    }
    else{
        l_time.substractHours(std::abs(hours));
    }
    if (minutes >= 0) {
        l_time.addMinutes(minutes);
    }
    else{
        l_time.substractMinutes(std::abs(minutes));
    }
    
    l_time.m_offset = static_cast<tristan::time::TimeZone>(hours);
    
    return l_time;
}

std::string tristan::time::DayTime::toString(bool show_offset) const{
    std::string l_time;
    
    if (m_durations.m_hours.count() < 10){
        l_time += '0';
    }
    l_time += std::to_string(m_durations.m_hours.count());
    l_time += ':';
    if (m_durations.m_minutes.count() < 10){
        l_time += '0';
    }
    l_time += std::to_string(m_durations.m_minutes.count());
    
    if (m_precision >= tristan::time::Precision::SECONDS){
        l_time += ':';
        if (m_durations.m_seconds.count() < 10){
            l_time += '0';
        }
        l_time += std::to_string(m_durations.m_seconds.count());
    }
    
    if (m_precision >= tristan::time::Precision::MILLISECONDS){
        l_time += '.';
        if (m_durations.m_milliseconds.count() < 100){
            l_time += '0';
        }
        if (m_durations.m_milliseconds.count() < 10){
            l_time += '0';
        }
        l_time += std::to_string(m_durations.m_milliseconds.count());
    }
    
    if (m_precision >= tristan::time::Precision::MICROSECONDS){
        l_time += '.';
        if (m_durations.m_microseconds.count() < 100){
            l_time += '0';
        }
        if (m_durations.m_microseconds.count() < 10){
            l_time += '0';
        }
        l_time += std::to_string(m_durations.m_microseconds.count());
    }
    
    if (m_precision == tristan::time::Precision::NANOSECONDS){
        l_time += '.';
        if (m_durations.m_nanoseconds.count() < 100){
            l_time += '0';
        }
        if (m_durations.m_nanoseconds.count() < 10){
            l_time += '0';
        }
        l_time += std::to_string(m_durations.m_nanoseconds.count());
    }
    if (show_offset){
        if (m_offset >= tristan::time::TimeZone::UTC){
            l_time += '+';
            if (m_offset < tristan::time::TimeZone::EAST_10){
                l_time += '0';
            }
        }
        l_time += std::to_string(static_cast<int8_t>(m_offset));
        if (m_offset > tristan::time::TimeZone::WEST_10){
            l_time = '0';
        }
    }
    return l_time;
}

bool tristan::time::operator !=(const tristan::time::DayTime &l, const tristan::time::DayTime &r){
    return !(l==r);
}

bool tristan::time::operator >(const tristan::time::DayTime &l, const tristan::time::DayTime &r){
    return !(l <= r);
}

bool tristan::time::operator <=(const tristan::time::DayTime &l, const tristan::time::DayTime &r){
    return (l < r || l == r);
}

bool tristan::time::operator >=(const tristan::time::DayTime &l, const tristan::time::DayTime &r){
    return (l > r || l == r);
}

std::ostream &tristan::time::operator<<(std::ostream &out, const tristan::time::DayTime &Time){
    out << Time.toString(true);
    return out;
}

auto tristan::time::operator+(const tristan::time::DayTime& l, const tristan::time::DayTime& r) -> tristan::time::DayTime{
    auto time = l;
    time.m_precision = std::max(l.precision(), r.precision());
    switch (time.m_precision){
        case tristan::time::Precision::NANOSECONDS:{
            time.addNanoseconds(r.nanoseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MICROSECONDS:{
            time.addMicroseconds(r.microseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MILLISECONDS:{
            time.addMilliseconds(r.milliseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::SECONDS:{
            time.addSeconds(r.seconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MINUTES:{
            time.addHours(r.hours());
            time.addMinutes(r.minutes());
            break;
        }
    }
    return time;
}

auto tristan::time::operator-(const tristan::time::DayTime& l, const tristan::time::DayTime& r) -> tristan::time::DayTime{
    auto time = l;
    time.m_precision = std::max(l.precision(), r.precision());
    switch (time.m_precision){
        case tristan::time::Precision::NANOSECONDS:{
            time.substractNanoseconds(r.nanoseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MICROSECONDS:{
            time.substractMicroseconds(r.microseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MILLISECONDS:{
            time.substractMilliseconds(r.milliseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::SECONDS:{
            time.substractSeconds(r.seconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MINUTES:{
            time.substractHours(r.hours());
            time.substractMinutes(r.minutes());
            break;
        }
    }
    return time;
    
}

namespace
{
    auto checkTimeFormat(const std::string& time) -> bool{
        
        auto length = time.length();
        if (length < 5 || length > 20) {
            return false;
        }
        for (size_t i = 0; i < length; ++i) {
            if ((time[i] < '0' || time[i] > '9') && (time[i] != ':' && time[i] != '.')) {
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
            if (i == 8 && time[i] != '.') {
                return false;
            }
            if ((i > 8 && i < 12) && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
            if (i == 13 && time[i] != '.') {
                return false;
            }
            if ((i > 13 && i < 16) && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
            if (i == 18 && time[i] != '.') {
                return false;
            }
            if ((i > 18 && i < 20) && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
        }
        return true;
    }
} //End of unnamed namespace