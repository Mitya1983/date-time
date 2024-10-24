#include "time.hpp"

#include <chrono>
#if defined __cpp_lib_format
  #include <format>
#endif
namespace {
    auto checkTimeFormat(const std::string& time) -> bool;
}  // End of unnamed namespace

mt::time::Time::Time() {

    const auto time_point = std::chrono::time_point_cast< std::chrono::nanoseconds >(std::chrono::system_clock::now());
    const auto days = std::chrono::time_point_cast< std::chrono::days >(time_point);
    m_nanoseconds_since_day_start = std::chrono::duration_cast< std::chrono::nanoseconds >(time_point - days);
}

mt::time::Time::Time(const mt::TimeZone p_time_zone) :
    m_offset(p_time_zone) {
    auto time_point = std::chrono::time_point_cast< std::chrono::nanoseconds >(std::chrono::system_clock::now());
    time_point += std::chrono::duration_cast< std::chrono::nanoseconds >(std::chrono::hours(static_cast< int8_t >(m_offset)));
    const auto days = std::chrono::time_point_cast< std::chrono::days >(time_point);
    m_nanoseconds_since_day_start = std::chrono::duration_cast< std::chrono::nanoseconds >(time_point - days);
}

mt::time::Time::Time(TimeDuration p_time_duration) {
    std::visit(
        [this]< typename TimeValueTime >(TimeValueTime&& value) -> void {
            if constexpr (std::is_same_v< std::decay_t< TimeValueTime >, std::chrono::nanoseconds >) {
                m_nanoseconds_since_day_start = value;
            } else if constexpr (std::is_same_v< std::decay_t< TimeValueTime >, std::chrono::microseconds >) {
                m_nanoseconds_since_day_start = std::chrono::duration_cast< std::chrono::nanoseconds >(value % 86400000000);
            } else if constexpr (std::is_same_v< std::decay_t< TimeValueTime >, std::chrono::milliseconds >) {
                m_nanoseconds_since_day_start = std::chrono::duration_cast< std::chrono::nanoseconds >(value % 86400000);
            } else if constexpr (std::is_same_v< std::decay_t< TimeValueTime >, std::chrono::seconds >) {
                m_nanoseconds_since_day_start = std::chrono::duration_cast< std::chrono::nanoseconds >(value % 86400);
            } else if constexpr (std::is_same_v< std::decay_t< TimeValueTime >, std::chrono::minutes >) {
                m_nanoseconds_since_day_start = std::chrono::duration_cast< std::chrono::nanoseconds >(value % 1440);
            } else {
                m_nanoseconds_since_day_start = std::chrono::duration_cast< std::chrono::nanoseconds >(value % 24);
            }
        },
        p_time_duration);
}

mt::time::Time::Time(const std::chrono::time_point< std::chrono::system_clock > p_time_point) {
    const auto days = std::chrono::time_point_cast< std::chrono::days >(p_time_point);
    m_nanoseconds_since_day_start = std::chrono::duration_cast< std::chrono::nanoseconds >(p_time_point - days);
}

mt::time::Time::Time(const std::chrono::hours p_hours,
                     const std::chrono::minutes p_minutes,
                     const std::chrono::seconds p_seconds,
                     const std::chrono::milliseconds p_milliseconds,
                     const std::chrono::microseconds p_microseconds,
                     const std::chrono::nanoseconds p_nanoseconds) {
    if (const auto hours = p_hours.count(); hours > 23) {
        const std::string message = "mt::time::Time(int hours, int minutes, int "
                                    "seconds): bad [hour] value was provided - "
                                    + std::to_string(hours) + ". The value from 0 to 23 is expected";
        throw std::range_error{message};
    }
    if (const auto minutes = p_minutes.count(); minutes > 59) {
        const std::string message = "mt::time::Time(int hours, int minutes, int "
                                    "seconds): bad [minutes] value was provided - "
                                    + std::to_string(minutes) + ". The value from 0 to 59 is expected";
        throw std::range_error{message};
    }
    if (const auto seconds = p_seconds.count(); seconds > 59) {
        const std::string message = "mt::time::Time(int hours, int minutes, int "
                                    "seconds): bad [seconds] value was provided - "
                                    + std::to_string(seconds) + ". The value from 0 to 59 is expected";
        throw std::range_error{message};
    }
    if (const auto milliseconds = p_milliseconds.count(); milliseconds > 999) {
        const std::string message = "mt::time::Time(int hours, int minutes, int seconds, uint16_t "
                                    "milliseconds): bad [milliseconds] value was provided - "
                                    + std::to_string(milliseconds) + ". The value from 0 to 999 is expected";
        throw std::range_error{message};
    }
    if (const auto nanoseconds = p_nanoseconds.count() > 999) {
        const std::string message = "mt::time::Time(int hours, int minutes, int seconds, uint16_t "
                                    "milliseconds): bad [nanoseconds] value was provided - "
                                    + std::to_string(nanoseconds) + ". The value from 0 to 999 is expected";
        throw std::range_error{message};
    }
    if (const auto microseconds = p_microseconds.count(); microseconds > 999) {
        const std::string message = "mt::time::Time(int hours, int minutes, int seconds, uint16_t "
                                    "milliseconds): bad [microseconds] value was provided - "
                                    + std::to_string(microseconds) + ". The value from 0 to 999 is expected";
        throw std::range_error{message};
    }
    m_nanoseconds_since_day_start += p_hours;
    m_nanoseconds_since_day_start += p_minutes;
    if (p_seconds != std::chrono::seconds{0}) {
        m_nanoseconds_since_day_start += p_seconds;
    }
    if (p_milliseconds != std::chrono::milliseconds{0}) {
        m_nanoseconds_since_day_start += p_milliseconds;
    }
    if (p_microseconds != std::chrono::microseconds{0}) {
        m_nanoseconds_since_day_start += p_microseconds;
    }
    if (p_nanoseconds != std::chrono::nanoseconds{0}) {
        m_nanoseconds_since_day_start += p_nanoseconds;
    }
}

mt::time::Time::Time(const std::string& time) {

    auto l_time = time;

    const auto offset_pos = l_time.find_first_of("-+");
    auto offset = mt::TimeZone::UTC;
    if (offset_pos != std::string::npos && offset_pos == l_time.size() - 3) {
        offset = static_cast< mt::TimeZone >(std::stoi(l_time.substr(offset_pos)));
        l_time.erase(offset_pos);
    }
    if (not checkTimeFormat(l_time)) {
        throw std::invalid_argument{"mt::time::Time::Time(const std::string& "
                                    "time): Invalid time format"};
    }
    const auto size = l_time.length();

    constexpr uint8_t hours_pos{0};
    constexpr uint8_t minutes_pos{3};
    constexpr uint8_t seconds_pos{6};
    constexpr uint8_t milliseconds_pos{9};
    constexpr uint8_t microseconds_pos{13};
    constexpr uint8_t nanoseconds_pos{17};

    switch (size) {
        case 5: {
            const auto hours = static_cast< uint8_t >(std::stoi(l_time.substr(hours_pos, 2)));
            const auto minutes = static_cast< uint8_t >(std::stoi(l_time.substr(minutes_pos, 2)));
            *this = mt::time::Time(std::chrono::hours{hours}, std::chrono::minutes{minutes});
            break;
        }
        case 8: {
            const auto hours = static_cast< uint8_t >(std::stoi(l_time.substr(hours_pos, 2)));
            const auto minutes = static_cast< uint8_t >(std::stoi(l_time.substr(minutes_pos, 2)));
            const auto seconds = static_cast< uint8_t >(std::stoi(l_time.substr(seconds_pos, 2)));
            *this = mt::time::Time(std::chrono::hours{hours}, std::chrono::minutes{minutes}, std::chrono::seconds{seconds});
            break;
        }
        case 12: {
            const auto hours = std::stoi(l_time.substr(hours_pos, 2));
            const auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            const auto seconds = std::stoi(l_time.substr(seconds_pos, 2));
            const auto milliseconds = std::stoi(l_time.substr(milliseconds_pos, 3));
            *this = mt::time::Time(std::chrono::hours{hours}, std::chrono::minutes{minutes}, std::chrono::seconds{seconds}, std::chrono::milliseconds{milliseconds});
            break;
        }
        case 16: {
            const auto hours = std::stoi(l_time.substr(hours_pos, 2));
            const auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            const auto seconds = std::stoi(l_time.substr(seconds_pos, 2));
            const auto milliseconds = std::stoi(l_time.substr(milliseconds_pos, 3));
            const auto microseconds = std::stoi(l_time.substr(microseconds_pos, 3));
            *this = mt::time::Time(std::chrono::hours{hours},
                                   std::chrono::minutes{minutes},
                                   std::chrono::seconds{seconds},
                                   std::chrono::milliseconds{milliseconds},
                                   std::chrono::microseconds{microseconds});
            break;
        }
        case 20: {
            const auto hours = std::stoi(l_time.substr(hours_pos, 2));
            const auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            const auto seconds = std::stoi(l_time.substr(seconds_pos, 2));
            const auto milliseconds = std::stoi(l_time.substr(milliseconds_pos, 3));
            const auto microseconds = std::stoi(l_time.substr(microseconds_pos, 3));
            const auto nanoseconds = std::stoi(l_time.substr(nanoseconds_pos, 3));
            *this = mt::time::Time(std::chrono::hours{hours},
                                   std::chrono::minutes{minutes},
                                   std::chrono::seconds{seconds},
                                   std::chrono::milliseconds{milliseconds},
                                   std::chrono::microseconds{microseconds},
                                   std::chrono::nanoseconds{nanoseconds});
            break;
        }
        default: {
            throw std::invalid_argument{"mt::time::Time::Time(const std::string& "
                                        "time): Invalid time format"};
        }
    }
    if (m_offset != offset) {
        m_offset = offset;
    }
}

auto mt::time::Time::operator==(const mt::time::Time& other) const -> bool { return m_nanoseconds_since_day_start == other.m_nanoseconds_since_day_start; }

auto mt::time::Time::operator<(const mt::time::Time& other) const -> bool { return m_nanoseconds_since_day_start < other.m_nanoseconds_since_day_start; }

void mt::time::Time::operator+=(const mt::time::Time& other) { *this = *this + other; }

void mt::time::Time::operator+=(const TimeDuration p_value) { *this = *this + p_value; }

void mt::time::Time::operator-=(const mt::time::Time& other) { *this = *this - other; }

void mt::time::Time::operator-=(const TimeDuration p_value) { *this = *this - p_value; }

void mt::time::Time::setOffset(const mt::TimeZone p_offset) { m_offset = p_offset; }

auto mt::time::Time::hours() const -> std::chrono::hours { return std::chrono::duration_cast< std::chrono::hours >(m_nanoseconds_since_day_start); }

auto mt::time::Time::minutes() const -> std::chrono::minutes { return std::chrono::duration_cast< std::chrono::minutes >(m_nanoseconds_since_day_start - this->hours()); }

auto mt::time::Time::seconds() const -> std::chrono::seconds {
    return std::chrono::duration_cast< std::chrono::seconds >(m_nanoseconds_since_day_start - this->hours() - this->minutes());
}

auto mt::time::Time::milliseconds() const -> std::chrono::milliseconds {
    return std::chrono::duration_cast< std::chrono::milliseconds >(m_nanoseconds_since_day_start - this->hours() - this->minutes() - this->seconds());
}

auto mt::time::Time::microseconds() const -> std::chrono::microseconds {
    return std::chrono::duration_cast< std::chrono::microseconds >(m_nanoseconds_since_day_start - this->hours() - this->minutes() - this->seconds() - this->milliseconds());
}

auto mt::time::Time::nanoseconds() const -> std::chrono::nanoseconds {
    return m_nanoseconds_since_day_start - this->hours() - this->minutes() - this->seconds() - this->milliseconds() - this->microseconds();
}

auto mt::time::Time::localTime() -> mt::time::Time {

    const auto tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto offset = std::localtime(&tm)->tm_gmtoff;

    return mt::time::Time(static_cast< mt::TimeZone >(offset / 3600));
}

//
std::string mt::time::Time::toString(const std::function< std::string(const Time&) >& formatter) const {
    if (formatter) {
        return formatter(*this);
    }
#if defined __cpp_lib_format
    return std::format("{0:%T}{1}", *this, m_offset);
#else
    std::string time;
    const auto hours = hours().count();
    if (hours < 10) {
        time += '0';
    }
    time += std::to_string(hours);
    time += ':';
    const auto minutes = minutes().count();
    if (minutes < 10) {
        time += '0';
    }
    time += std::to_string(minutes);
    time += ':';
    const auto seconds = seconds().count();
    if (seconds < 10) {
        time += '0';
    }
    time += std::to_string(seconds);
    time += '.';
    const auto nanoseconds = (m_nanoseconds_since_day_start - std::chrono::duration_cast< std::chrono::seconds >(m_nanoseconds_since_day_start)).count();
    time += std::to_string(nanoseconds);
    if (m_offset == mt::TimeZone::UTC) {
        time += 'Z';
    } else {
        m_offset > mt::TimeZone::UTC ? time += '+' : time += '-';
        if (m_offset > mt::TimeZone::WEST_10 && m_offset < mt::TimeZone::EAST_10) {
            time += "0";
        }
        time += std::to_string(std::abs(static_cast< int8_t >(m_offset)));
        time += ":00";
    }
    return time;
#endif
}

bool mt::time::operator!=(const mt::time::Time& l, const mt::time::Time& r) { return not(l == r); }

bool mt::time::operator>(const mt::time::Time& l, const mt::time::Time& r) { return not(l <= r); }

bool mt::time::operator<=(const mt::time::Time& l, const mt::time::Time& r) { return (l < r || l == r); }

bool mt::time::operator>=(const mt::time::Time& l, const mt::time::Time& r) { return (l > r || l == r); }

auto mt::time::operator+(const mt::time::Time l, mt::time::Time r) -> mt::time::Time { return l + r.m_nanoseconds_since_day_start; }

mt::time::Time mt::time::operator+(Time p_time, const TimeDuration p_value) {
    std::visit(
        [&p_time]< typename TimeValueType >(TimeValueType&& value) -> void {
            p_time.m_nanoseconds_since_day_start += value;
        },
        p_value);
    if (const auto days = std::chrono::duration_cast< std::chrono::days >(p_time.m_nanoseconds_since_day_start); days != std::chrono::hours{0}) {
        p_time.m_nanoseconds_since_day_start -= days;
    }
    return p_time;
}

auto mt::time::operator-(const mt::time::Time l, mt::time::Time r) -> mt::time::Time { return l - r.m_nanoseconds_since_day_start; }

mt::time::Time mt::time::operator-(Time p_time, const TimeDuration p_value) {
    std::visit(
        [&p_time]< typename TimeValueType >(TimeValueType&& value) -> void {
            p_time.m_nanoseconds_since_day_start -= value;
        },
        p_value);
    if (p_time.m_nanoseconds_since_day_start < std::chrono::nanoseconds{0}) {
        p_time.m_nanoseconds_since_day_start = std::chrono::days{1} + p_time.m_nanoseconds_since_day_start;
    }
    return p_time;
}

auto mt::time::operator<<(std::ostream& out, const Time& time) -> std::ostream& {
    const auto& _string = time.toString();
    out.write(_string.data(), std::ssize(_string));
    return out;
}

namespace {
    auto checkTimeFormat(const std::string& time) -> bool {

        const auto length = time.length();
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
            if (i > 2 && i < 5 && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
            if (i == 5 && time[i] != ':') {
                return false;
            }
            if (i > 5 && i < 8 && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
            if (i == 8 && time[i] != '.') {
                return false;
            }
            if (i > 8 && i < 12 && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
            if (i == 12 && time[i] != '.') {
                return false;
            }
            if (i > 12 && i < 16 && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
            if (i == 16 && time[i] != '.') {
                return false;
            }
            if (i > 16 && i < 20 && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
        }
        return true;
    }
}  // End of unnamed namespace