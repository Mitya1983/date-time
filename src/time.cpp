#include "time.hpp"
#include <algorithm>
#include <chrono>

namespace {
    auto checkTimeFormat(const std::string& time) -> bool;
    constexpr uint64_t microseconds_in_day = 86400000000;
    constexpr uint64_t nanoseconds_in_day = 86400000000000;
    constexpr uint64_t nanoseconds_in_hour = 3600000000000;
    constexpr uint64_t microseconds_in_minute = 60000000;
    constexpr uint64_t microseconds_in_hour = 3600000000;
    constexpr uint64_t nanoseconds_in_minute = 60000000000;
    constexpr uint64_t microseconds_in_second = 1000000;
    constexpr uint64_t nanoseconds_in_second = 1000000000;
    constexpr uint64_t nanoseconds_in_millisecond = 1000000;
    constexpr uint32_t milliseconds_in_minute = 60000;
    constexpr uint32_t seconds_in_day = 86400;
    constexpr uint32_t milliseconds_in_day = 86400000;
    constexpr uint32_t milliseconds_in_hour = 3600000;
    constexpr uint16_t g_seconds_in_hour = 3600;
    constexpr uint16_t minutes_in_day = 1440;
    constexpr uint16_t microseconds_in_millisecond = 1000;
    constexpr uint16_t nanoseconds_in_microsecond = 1000;
    constexpr uint16_t milliseconds_in_second = 1000;
    constexpr uint8_t g_minutes_in_hour = 60;
    constexpr uint8_t g_seconds_in_minute = 60;
    using Days = std::chrono::duration< int64_t, std::ratio_divide< std::ratio< seconds_in_day >, std::chrono::seconds::period > >;

    auto g_default_global_formatter = [](const tristan::time::Time& p_time) -> std::string {
        std::string l_time;

        auto hours = p_time.hours();
        if (hours < 10) {
            l_time += '0';
        }
        l_time += std::to_string(hours);
        l_time += ':';
        auto minutes = p_time.minutes();
        if (minutes < 10) {
            l_time += '0';
        }
        l_time += std::to_string(minutes);

        if (p_time.precision() >= tristan::time::Precision::SECONDS) {
            l_time += ':';
            auto seconds = p_time.seconds();
            if (seconds < 10) {
                l_time += '0';
            }
            l_time += std::to_string(seconds);
        }

        if (p_time.precision() >= tristan::time::Precision::MILLISECONDS) {
            l_time += '.';
            auto milliseconds = p_time.milliseconds();
            if (milliseconds < 100) {
                l_time += '0';
            }
            if (milliseconds < 10) {
                l_time += '0';
            }
            l_time += std::to_string(milliseconds);
        }

        if (p_time.precision() >= tristan::time::Precision::MICROSECONDS) {
            l_time += '.';
            auto microseconds = p_time.microseconds();
            if (microseconds < 100) {
                l_time += '0';
            }
            if (microseconds < 10) {
                l_time += '0';
            }
            l_time += std::to_string(microseconds);
        }

        if (p_time.precision() == tristan::time::Precision::NANOSECONDS) {
            l_time += '.';
            auto nanoseconds = p_time.nanoseconds();
            if (nanoseconds < 100) {
                l_time += '0';
            }
            if (nanoseconds < 10) {
                l_time += '0';
            }
            l_time += std::to_string(nanoseconds);
        }
        if (p_time.offset() >= tristan::TimeZone::UTC) {
            l_time += '+';
            if (p_time.offset() < tristan::TimeZone::EAST_10) {
                l_time += '0';
            }
        }
        l_time += std::to_string(static_cast< int8_t >(p_time.offset()));
        if (p_time.offset() > tristan::TimeZone::WEST_10 && p_time.offset() < tristan::TimeZone::UTC) {
            l_time += '0';
        }
        return l_time;
    };

}  // End of unnamed namespace

tristan::time::Time::Time(tristan::time::Precision precision) :
    m_offset{tristan::TimeZone::UTC},
    m_precision{precision} {

    auto time_point = std::chrono::system_clock::duration(std::chrono::system_clock::now().time_since_epoch());
    auto days = std::chrono::duration_cast< Days >(time_point);

    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            m_time_since_day_start
                = std::chrono::duration_cast< std::chrono::minutes >(time_point - std::chrono::duration_cast< std::chrono::system_clock::duration >(days));
            break;
        }
        case tristan::time::Precision::SECONDS: {
            m_time_since_day_start
                = std::chrono::duration_cast< std::chrono::seconds >(time_point - std::chrono::duration_cast< std::chrono::system_clock::duration >(days));
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            m_time_since_day_start
                = std::chrono::duration_cast< std::chrono::milliseconds >(time_point - std::chrono::duration_cast< std::chrono::system_clock::duration >(days));
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            m_time_since_day_start
                = std::chrono::duration_cast< std::chrono::microseconds >(time_point - std::chrono::duration_cast< std::chrono::system_clock::duration >(days));
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            m_time_since_day_start
                = std::chrono::duration_cast< std::chrono::nanoseconds >(time_point - std::chrono::duration_cast< std::chrono::system_clock::duration >(days));
            break;
        }
    }
}

tristan::time::Time::Time(tristan::TimeZone p_time_zone, tristan::time::Precision p_precision) :
    m_offset(p_time_zone),
    m_precision(p_precision) {
    auto time_point = std::chrono::system_clock::duration(std::chrono::system_clock::now().time_since_epoch());
    time_point += std::chrono::duration_cast< std::chrono::system_clock::duration >(std::chrono::hours(static_cast< int8_t >(m_offset)));
    auto days = std::chrono::duration_cast< Days >(time_point);

    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            m_time_since_day_start
                = std::chrono::duration_cast< std::chrono::minutes >(time_point - std::chrono::duration_cast< std::chrono::system_clock::duration >(days));
            break;
        }
        case tristan::time::Precision::SECONDS: {
            m_time_since_day_start
                = std::chrono::duration_cast< std::chrono::seconds >(time_point - std::chrono::duration_cast< std::chrono::system_clock::duration >(days));
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            m_time_since_day_start
                = std::chrono::duration_cast< std::chrono::milliseconds >(time_point - std::chrono::duration_cast< std::chrono::system_clock::duration >(days));
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            m_time_since_day_start
                = std::chrono::duration_cast< std::chrono::microseconds >(time_point - std::chrono::duration_cast< std::chrono::system_clock::duration >(days));
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            m_time_since_day_start
                = std::chrono::duration_cast< std::chrono::nanoseconds >(time_point - std::chrono::duration_cast< std::chrono::system_clock::duration >(days));
            break;
        }
    }
}

tristan::time::Time::Time(uint8_t hours, uint8_t minutes) noexcept(false) :
    m_offset{tristan::TimeZone::UTC},
    m_precision{tristan::time::Precision::MINUTES} {
    if (hours > 23) {
        std::string message = "tristan::time::Time(int hours, int minutes, int "
                              "seconds): bad [hour] value was provided - "
                              + std::to_string(hours) + ". The value from 0 to 23 is expected";
        throw std::range_error{message};
    }
    if (minutes > 59) {
        std::string message = "tristan::time::Time(int hours, int minutes, int "
                              "seconds): bad [minutes] value was provided - "
                              + std::to_string(minutes) + ". The value from 0 to 59 is expected";
        throw std::range_error{message};
    }
    m_time_since_day_start = std::chrono::minutes{hours * 60 + minutes};
}

tristan::time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds) :
    tristan::time::Time{hours, minutes} {
    if (seconds > 59) {
        std::string message = "tristan::time::Time(int hours, int minutes, int "
                              "seconds): bad [seconds] value was provided - "
                              + std::to_string(seconds) + ". The value from 0 to 59 is expected";
        throw std::range_error{message};
    }
    auto l_minutes = std::get< std::chrono::minutes >(m_time_since_day_start);
    m_time_since_day_start = std::chrono::duration_cast< std::chrono::seconds >(l_minutes) + std::chrono::seconds{seconds};

    m_precision = tristan::time::Precision::SECONDS;
}

tristan::time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) :
    tristan::time::Time{hours, minutes, seconds} {
    if (milliseconds > 999) {
        std::string message = "tristan::time::Time(int hours, int minutes, int seconds, uint16_t "
                              "milliseconds): bad [milliseconds] value was provided - "
                              + std::to_string(seconds) + ". The value from 0 to 999 is expected";
        throw std::range_error{message};
    }
    auto l_seconds = std::get< std::chrono::seconds >(m_time_since_day_start);
    m_time_since_day_start = std::chrono::duration_cast< std::chrono::milliseconds >(l_seconds) + std::chrono::milliseconds{milliseconds};

    m_precision = tristan::time::Precision::MILLISECONDS;
}

tristan::time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds) :
    tristan::time::Time{hours, minutes, seconds, milliseconds} {
    if (microseconds > 999) {
        std::string message = "tristan::time::Time(int hours, int minutes, int seconds, uint16_t "
                              "milliseconds): bad [microseconds] value was provided - "
                              + std::to_string(seconds) + ". The value from 0 to 999 is expected";
        throw std::range_error{message};
    }
    auto l_milliseconds = std::get< std::chrono::milliseconds >(m_time_since_day_start);
    m_time_since_day_start = std::chrono::duration_cast< std::chrono::microseconds >(l_milliseconds) + std::chrono::microseconds{microseconds};

    m_precision = tristan::time::Precision::MICROSECONDS;
}

tristan::time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds, uint16_t nanoseconds) :
    tristan::time::Time{hours, minutes, seconds, milliseconds, microseconds} {
    if (nanoseconds > 999) {
        std::string message = "tristan::time::Time(int hours, int minutes, int seconds, uint16_t "
                              "milliseconds): bad [nanoseconds] value was provided - "
                              + std::to_string(seconds) + ". The value from 0 to 999 is expected";
        throw std::range_error{message};
    }
    auto l_microseconds = std::get< std::chrono::microseconds >(m_time_since_day_start);
    m_time_since_day_start = std::chrono::duration_cast< std::chrono::nanoseconds >(l_microseconds) + std::chrono::nanoseconds{nanoseconds};

    m_precision = tristan::time::Precision::NANOSECONDS;
}

tristan::time::Time::Time(const std::string& time) :
    m_offset{tristan::TimeZone::UTC},
    m_precision(tristan::time::Precision::MINUTES) {

    auto l_time = time;

    auto offset_pos = l_time.find_first_of("-+");
    auto offset = tristan::TimeZone::UTC;
    if (offset_pos != std::string::npos && offset_pos == l_time.size() - 3) {
        offset = static_cast< tristan::TimeZone >(std::stoi(l_time.substr(offset_pos)));
        l_time.erase(offset_pos);
    }
    if (!checkTimeFormat(l_time)) {
        throw std::invalid_argument{"tristan::time::Time::Time(const std::string& "
                                    "time): Invalid time format"};
    }
    auto size = l_time.length();

    const uint8_t hours_pos = 0;
    const uint8_t minutes_pos = 3;
    const uint8_t seconds_pos = 6;
    const uint8_t milliseconds_pos = 9;
    const uint8_t microseconds_pos = 13;
    const uint8_t nanoseconds_pos = 17;

    switch (size) {
        case 5: {
            auto hours = std::stoi(l_time.substr(hours_pos, 2));
            auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            *this = tristan::time::Time(hours, minutes);
            break;
        }
        case 8: {
            auto hours = std::stoi(l_time.substr(hours_pos, 2));
            auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            auto seconds = std::stoi(l_time.substr(seconds_pos, 2));
            *this = tristan::time::Time(hours, minutes, seconds);
            break;
        }
        case 12: {
            auto hours = std::stoi(l_time.substr(hours_pos, 2));
            auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            auto seconds = std::stoi(l_time.substr(seconds_pos, 2));
            auto milliseconds = std::stoi(l_time.substr(milliseconds_pos, 3));
            *this = tristan::time::Time(hours, minutes, seconds, milliseconds);
            break;
        }
        case 16: {
            auto hours = std::stoi(l_time.substr(hours_pos, 2));
            auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            auto seconds = std::stoi(l_time.substr(seconds_pos, 2));
            auto milliseconds = std::stoi(l_time.substr(milliseconds_pos, 3));
            auto microseconds = std::stoi(l_time.substr(microseconds_pos, 3));
            *this = tristan::time::Time(hours, minutes, seconds, milliseconds, microseconds);
            break;
        }
        case 20: {
            auto hours = std::stoi(l_time.substr(hours_pos, 2));
            auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            auto seconds = std::stoi(l_time.substr(seconds_pos, 2));
            auto milliseconds = std::stoi(l_time.substr(milliseconds_pos, 3));
            auto microseconds = std::stoi(l_time.substr(microseconds_pos, 3));
            auto nanoseconds = std::stoi(l_time.substr(nanoseconds_pos, 3));
            *this = tristan::time::Time(hours, minutes, seconds, milliseconds, microseconds, nanoseconds);
            break;
        }
        default: {
            throw std::invalid_argument{"tristan::time::Time::Time(const std::string& "
                                        "time): Invalid time format"};
        }
    }
    if (m_offset != offset) {
        m_offset = offset;
    }
}

auto tristan::time::Time::operator==(const tristan::time::Time& other) const -> bool {

    if (m_precision != other.m_precision) {
        return false;
    }

    return m_time_since_day_start == other.m_time_since_day_start;
}

auto tristan::time::Time::operator<(const tristan::time::Time& other) const -> bool {
    if (m_precision != other.m_precision) {
        return false;
    }

    return m_time_since_day_start < other.m_time_since_day_start;
}

void tristan::time::Time::operator+=(const tristan::time::Time& other) { *this = *this + other; }

void tristan::time::Time::operator-=(const tristan::time::Time& other) { *this = *this - other; }

void tristan::time::Time::setOffset(tristan::TimeZone p_offset) { m_offset = p_offset; }

void tristan::time::Time::addHours(uint64_t p_hours) {

    if (p_hours == 0) {
        return;
    }
    if (p_hours > 23) {
        p_hours %= 24;
    }
    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            _addMinutes(p_hours * g_minutes_in_hour);
            break;
        }
        case tristan::time::Precision::SECONDS: {
            _addSeconds(p_hours * g_seconds_in_hour);
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            _addMilliseconds(p_hours * milliseconds_in_hour);
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            _addMicroseconds(p_hours * microseconds_in_hour);
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            _addNanoseconds(p_hours * nanoseconds_in_hour);
            break;
        }
    }
}

void tristan::time::Time::addMinutes(uint64_t p_minutes) {
    if (p_minutes == 0) {
        return;
    }
    if (p_minutes > minutes_in_day) {
        p_minutes %= minutes_in_day;
    }
    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            _addMinutes(p_minutes);
            break;
        }
        case tristan::time::Precision::SECONDS: {
            _addSeconds(p_minutes * g_seconds_in_minute);
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            _addMilliseconds(p_minutes * milliseconds_in_minute);
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            _addMicroseconds(p_minutes * microseconds_in_minute);
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            _addNanoseconds(p_minutes * nanoseconds_in_minute);
            break;
        }
    }
}

void tristan::time::Time::addSeconds(uint64_t p_seconds) {
    if (p_seconds == 0) {
        return;
    }
    if (p_seconds > seconds_in_day) {
        p_seconds %= seconds_in_day;
    }
    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            if (p_seconds < g_seconds_in_minute) {
                return;
            }
            _addMinutes(p_seconds % g_seconds_in_minute);
        }
        case tristan::time::Precision::SECONDS: {
            _addSeconds(p_seconds);
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            _addMilliseconds(p_seconds * milliseconds_in_second);
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            _addMicroseconds(p_seconds * microseconds_in_second);
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            _addNanoseconds(p_seconds * nanoseconds_in_second);
            break;
        }
    }
}

void tristan::time::Time::addMilliseconds(uint64_t p_milliseconds) {
    if (p_milliseconds == 0) {
        return;
    }
    if (p_milliseconds > milliseconds_in_day) {
        p_milliseconds %= milliseconds_in_day;
    }
    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            if (p_milliseconds < milliseconds_in_minute) {
                return;
            }
            _addMinutes(p_milliseconds % milliseconds_in_minute);
        }
        case tristan::time::Precision::SECONDS: {
            if (p_milliseconds < milliseconds_in_second) {
                return;
            }
            _addSeconds(p_milliseconds % milliseconds_in_second);
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            _addMilliseconds(p_milliseconds);
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            _addMicroseconds(p_milliseconds * microseconds_in_millisecond);
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            _addNanoseconds(p_milliseconds * nanoseconds_in_millisecond);
            break;
        }
    }
}

void tristan::time::Time::addMicroseconds(uint64_t p_microseconds) {
    if (p_microseconds == 0) {
        return;
    }
    if (p_microseconds > microseconds_in_day) {
        p_microseconds %= microseconds_in_day;
    }
    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            if (p_microseconds < microseconds_in_minute) {
                return;
            }
            _addMinutes(p_microseconds % microseconds_in_minute);
        }
        case tristan::time::Precision::SECONDS: {
            if (p_microseconds < microseconds_in_second) {
                return;
            }
            _addSeconds(p_microseconds % microseconds_in_second);
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            if (p_microseconds < microseconds_in_millisecond) {
                return;
            }
            _addMilliseconds(p_microseconds % microseconds_in_millisecond);
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            _addMicroseconds(p_microseconds);
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            _addNanoseconds(p_microseconds * nanoseconds_in_microsecond);
            break;
        }
    }
}

void tristan::time::Time::addNanoseconds(uint64_t p_nanoseconds) {
    if (p_nanoseconds == 0) {
        return;
    }
    if (p_nanoseconds > nanoseconds_in_day) {
        p_nanoseconds %= nanoseconds_in_day;
    }
    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            if (p_nanoseconds < nanoseconds_in_minute) {
                return;
            }
            _addMinutes(p_nanoseconds % nanoseconds_in_minute);
        }
        case tristan::time::Precision::SECONDS: {
            if (p_nanoseconds < nanoseconds_in_second) {
                return;
            }
            _addSeconds(p_nanoseconds % nanoseconds_in_second);
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            if (p_nanoseconds < nanoseconds_in_millisecond) {
                return;
            }
            _addMilliseconds(p_nanoseconds % nanoseconds_in_millisecond);
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            if (p_nanoseconds < nanoseconds_in_microsecond) {
                return;
            }
            _addMicroseconds(p_nanoseconds % nanoseconds_in_microsecond);
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            _addNanoseconds(p_nanoseconds);
            break;
        }
    }
}

void tristan::time::Time::subtractHours(uint64_t p_hours) {
    if (p_hours == 0) {
        return;
    }
    if (p_hours > 23) {
        p_hours %= 24;
    }
    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            _subtractMinutes(p_hours * g_minutes_in_hour);
            break;
        }
        case tristan::time::Precision::SECONDS: {
            _subtractSeconds(p_hours * g_seconds_in_hour);
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            _subtractMilliseconds(p_hours * milliseconds_in_hour);
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            _subtractMicroseconds(p_hours * microseconds_in_hour);
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            _subtractNanoseconds(p_hours * nanoseconds_in_hour);
            break;
        }
    }
}

void tristan::time::Time::subtractMinutes(uint64_t p_minutes) {
    if (p_minutes == 0) {
        return;
    }
    if (p_minutes > minutes_in_day) {
        p_minutes %= minutes_in_day;
    }
    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            _subtractMinutes(p_minutes);
            break;
        }
        case tristan::time::Precision::SECONDS: {
            _subtractSeconds(p_minutes * g_seconds_in_minute);
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            _subtractMilliseconds(p_minutes * milliseconds_in_minute);
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            _subtractMicroseconds(p_minutes * microseconds_in_minute);
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            _subtractNanoseconds(p_minutes * nanoseconds_in_minute);
            break;
        }
    }
}

void tristan::time::Time::subtractSeconds(uint64_t p_seconds) {
    if (p_seconds == 0) {
        return;
    }
    if (p_seconds > seconds_in_day) {
        p_seconds %= seconds_in_day;
    }
    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            if (p_seconds < g_seconds_in_minute) {
                return;
            }
            _subtractMinutes(p_seconds % g_seconds_in_minute);
        }
        case tristan::time::Precision::SECONDS: {
            _subtractSeconds(p_seconds);
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            _subtractMilliseconds(p_seconds * milliseconds_in_second);
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            _subtractMicroseconds(p_seconds * microseconds_in_second);
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            _subtractNanoseconds(p_seconds * nanoseconds_in_second);
            break;
        }
    }
}

void tristan::time::Time::subtractMilliseconds(uint64_t p_milliseconds) {
    if (p_milliseconds == 0) {
        return;
    }
    if (p_milliseconds > milliseconds_in_day) {
        p_milliseconds %= milliseconds_in_day;
    }
    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            if (p_milliseconds < milliseconds_in_minute) {
                return;
            }
            _subtractMinutes(p_milliseconds % milliseconds_in_minute);
        }
        case tristan::time::Precision::SECONDS: {
            if (p_milliseconds < milliseconds_in_second) {
                return;
            }
            _subtractSeconds(p_milliseconds % milliseconds_in_second);
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            _subtractMilliseconds(p_milliseconds);
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            _subtractMicroseconds(p_milliseconds * microseconds_in_millisecond);
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            _subtractNanoseconds(p_milliseconds * nanoseconds_in_millisecond);
            break;
        }
    }
}

void tristan::time::Time::subtractMicroseconds(uint64_t p_microseconds) {
    if (p_microseconds == 0) {
        return;
    }
    if (p_microseconds > microseconds_in_day) {
        p_microseconds %= microseconds_in_day;
    }
    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            if (p_microseconds < microseconds_in_minute) {
                return;
            }
            _subtractMinutes(p_microseconds % microseconds_in_minute);
        }
        case tristan::time::Precision::SECONDS: {
            if (p_microseconds < microseconds_in_second) {
                return;
            }
            _subtractSeconds(p_microseconds % microseconds_in_second);
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            if (p_microseconds < microseconds_in_millisecond) {
                return;
            }
            _subtractMilliseconds(p_microseconds % microseconds_in_millisecond);
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            _subtractMicroseconds(p_microseconds);
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            _subtractNanoseconds(p_microseconds * nanoseconds_in_microsecond);
            break;
        }
    }
}

void tristan::time::Time::subtractNanoseconds(uint64_t p_nanoseconds) {
    if (p_nanoseconds == 0) {
        return;
    }
    if (p_nanoseconds > nanoseconds_in_day) {
        p_nanoseconds %= nanoseconds_in_day;
    }
    switch (m_precision) {
        case tristan::time::Precision::MINUTES: {
            if (p_nanoseconds < nanoseconds_in_minute) {
                return;
            }
            _subtractMinutes(p_nanoseconds % nanoseconds_in_minute);
        }
        case tristan::time::Precision::SECONDS: {
            if (p_nanoseconds < nanoseconds_in_second) {
                return;
            }
            _subtractSeconds(p_nanoseconds % nanoseconds_in_second);
            break;
        }
        case tristan::time::Precision::MILLISECONDS: {
            if (p_nanoseconds < nanoseconds_in_millisecond) {
                return;
            }
            _subtractMilliseconds(p_nanoseconds % nanoseconds_in_millisecond);
            break;
        }
        case tristan::time::Precision::MICROSECONDS: {
            if (p_nanoseconds < nanoseconds_in_microsecond) {
                return;
            }
            _subtractMicroseconds(p_nanoseconds % nanoseconds_in_microsecond);
            break;
        }
        case tristan::time::Precision::NANOSECONDS: {
            _subtractNanoseconds(p_nanoseconds);
            break;
        }
    }
}

auto tristan::time::Time::hours() const -> uint8_t {
    return std::visit(
        [](const auto& value) {
            return static_cast< uint8_t >(std::chrono::duration_cast< std::chrono::hours >(value).count());
        },
        this->m_time_since_day_start);
}

auto tristan::time::Time::minutes() const -> uint8_t {
    return std::visit(
        [](const auto& value) {
            auto hours = std::chrono::duration_cast< std::chrono::hours >(value);
            auto minutes = std::chrono::duration_cast< std::chrono::minutes >(value) - std::chrono::duration_cast< std::chrono::minutes >(hours);
            return static_cast< uint8_t >(minutes.count());
        },
        this->m_time_since_day_start);
}

auto tristan::time::Time::seconds() const -> uint8_t {
    if (m_precision < tristan::time::Precision::SECONDS) {
        return 0;
    }

    return std::visit(
        [](const auto& value) {
            auto minutes = std::chrono::duration_cast< std::chrono::minutes >(value);
            auto seconds = std::chrono::duration_cast< std::chrono::seconds >(value) - std::chrono::duration_cast< std::chrono::seconds >(minutes);
            return static_cast< uint8_t >(seconds.count());
        },
        this->m_time_since_day_start);
}

auto tristan::time::Time::milliseconds() const -> uint16_t {
    if (m_precision < tristan::time::Precision::MILLISECONDS) {
        return 0;
    }

    return std::visit(
        [](const auto& value) {
            auto seconds = std::chrono::duration_cast< std::chrono::seconds >(value);
            auto milliseconds
                = std::chrono::duration_cast< std::chrono::milliseconds >(value) - std::chrono::duration_cast< std::chrono::milliseconds >(seconds);
            return static_cast< uint16_t >(milliseconds.count());
        },
        this->m_time_since_day_start);
}

auto tristan::time::Time::microseconds() const -> uint16_t {
    if (m_precision < tristan::time::Precision::MICROSECONDS) {
        return 0;
    }

    return std::visit(
        [](const auto& value) {
            auto milliseconds = std::chrono::duration_cast< std::chrono::milliseconds >(value);
            auto microseconds
                = std::chrono::duration_cast< std::chrono::microseconds >(value) - std::chrono::duration_cast< std::chrono::microseconds >(milliseconds);
            return static_cast< uint16_t >(microseconds.count());
        },
        this->m_time_since_day_start);
}

auto tristan::time::Time::nanoseconds() const -> uint16_t {
    if (m_precision < tristan::time::Precision::NANOSECONDS) {
        return 0;
    }

    return std::visit(
        [](const auto& value) {
            auto microseconds = std::chrono::duration_cast< std::chrono::microseconds >(value);
            auto nanoseconds
                = std::chrono::duration_cast< std::chrono::nanoseconds >(value) - std::chrono::duration_cast< std::chrono::nanoseconds >(microseconds);
            return static_cast< uint16_t >(nanoseconds.count());
        },
        this->m_time_since_day_start);
}

auto tristan::time::Time::localTime(Precision p_precision) -> tristan::time::Time {

    auto tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto offset = std::localtime(&tm)->tm_gmtoff;

    return tristan::time::Time(static_cast< tristan::TimeZone >(offset / 3600), p_precision);
}

void tristan::time::Time::setGlobalFormatter(tristan::time::Formatter&& p_formatter) { m_formatter_global = std::move(p_formatter); }

void tristan::time::Time::setLocalFormatter(tristan::time::Formatter&& p_formatter) { m_formatter_local = std::move(p_formatter); }

std::string tristan::time::Time::toString() const {
    if (not m_formatter_global) {
        tristan::time::Time::m_formatter_global = g_default_global_formatter;
    }
    if (m_formatter_local) {
        return m_formatter_local(*this);
    }
    return m_formatter_global(*this);
}

bool tristan::time::operator!=(const tristan::time::Time& l, const tristan::time::Time& r) { return !(l == r); }

bool tristan::time::operator>(const tristan::time::Time& l, const tristan::time::Time& r) { return !(l <= r); }

bool tristan::time::operator<=(const tristan::time::Time& l, const tristan::time::Time& r) { return (l < r || l == r); }

bool tristan::time::operator>=(const tristan::time::Time& l, const tristan::time::Time& r) { return (l > r || l == r); }

std::ostream& tristan::time::operator<<(std::ostream& out, const tristan::time::Time& time) {
    out << time.toString();
    return out;
}

auto tristan::time::operator+(const tristan::time::Time& l, const tristan::time::Time& r) -> tristan::time::Time {
    auto time = l;
    time.m_precision = std::max(l.precision(), r.precision());
    switch (time.m_precision) {
        case tristan::time::Precision::NANOSECONDS: {
            time.addNanoseconds(r.nanoseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MICROSECONDS: {
            time.addMicroseconds(r.microseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MILLISECONDS: {
            time.addMilliseconds(r.milliseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::SECONDS: {
            time.addSeconds(r.seconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MINUTES: {
            time.addMinutes(r.minutes());
            time.addHours(r.hours());
            break;
        }
    }
    return time;
}

auto tristan::time::operator-(const tristan::time::Time& l, const tristan::time::Time& r) -> tristan::time::Time {
    auto time = l;
    time.m_precision = std::max(l.precision(), r.precision());
    switch (time.m_precision) {
        case tristan::time::Precision::NANOSECONDS: {
            time.subtractNanoseconds(r.nanoseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MICROSECONDS: {
            time.subtractMicroseconds(r.microseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MILLISECONDS: {
            time.subtractMilliseconds(r.milliseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::SECONDS: {
            time.subtractSeconds(r.seconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MINUTES: {
            time.subtractMinutes(r.minutes());
            time.subtractHours(r.hours());
            break;
        }
    }
    return time;
}

void tristan::time::Time::_addMinutes(uint64_t minutes) {
    auto& l_minutes = std::get< std::chrono::minutes >(m_time_since_day_start);
    l_minutes += std::chrono::minutes{minutes};
    if (l_minutes.count() > minutes_in_day) {
        l_minutes %= minutes_in_day;
    }
}

void tristan::time::Time::_addSeconds(uint64_t seconds) {
    auto& l_seconds = std::get< std::chrono::seconds >(m_time_since_day_start);
    l_seconds += std::chrono::seconds{seconds};
    if (l_seconds.count() > seconds_in_day) {
        l_seconds %= seconds_in_day;
    }
}

void tristan::time::Time::_addMilliseconds(uint64_t milliseconds) {
    auto& l_milliseconds = std::get< std::chrono::milliseconds >(m_time_since_day_start);
    l_milliseconds += std::chrono::milliseconds{milliseconds};
    if (l_milliseconds.count() > milliseconds_in_day) {
        l_milliseconds %= milliseconds_in_day;
    }
}

void tristan::time::Time::_addMicroseconds(uint64_t microseconds) {
    auto& l_microseconds = std::get< std::chrono::microseconds >(m_time_since_day_start);
    l_microseconds += std::chrono::microseconds{microseconds};
    if (static_cast< uint64_t >(l_microseconds.count()) > microseconds_in_day) {
        l_microseconds %= microseconds_in_day;
    }
}

void tristan::time::Time::_addNanoseconds(uint64_t nanoseconds) {
    auto& l_nanoseconds = std::get< std::chrono::nanoseconds >(m_time_since_day_start);
    l_nanoseconds += std::chrono::nanoseconds{nanoseconds};
    if (static_cast< uint64_t >(l_nanoseconds.count()) > nanoseconds_in_day) {
        l_nanoseconds %= nanoseconds_in_day;
    }
}

void tristan::time::Time::_subtractMinutes(uint64_t minutes) {
    auto& l_minutes = std::get< std::chrono::minutes >(m_time_since_day_start);
    l_minutes -= std::chrono::minutes{minutes};
    if (l_minutes.count() < 0) {
        l_minutes += std::chrono::minutes{minutes_in_day};
    }
}

void tristan::time::Time::_subtractSeconds(uint64_t seconds) {
    auto& l_seconds = std::get< std::chrono::seconds >(m_time_since_day_start);
    l_seconds -= std::chrono::seconds{seconds};
    if (l_seconds.count() < 0) {
        l_seconds += std::chrono::seconds{seconds_in_day};
    }
}

void tristan::time::Time::_subtractMilliseconds(uint64_t milliseconds) {
    auto& l_milliseconds = std::get< std::chrono::milliseconds >(m_time_since_day_start);
    l_milliseconds -= std::chrono::milliseconds{milliseconds};
    if (l_milliseconds.count() < 0) {
        l_milliseconds += std::chrono::milliseconds{milliseconds_in_day};
    }
}

void tristan::time::Time::_subtractMicroseconds(uint64_t microseconds) {
    auto& l_microseconds = std::get< std::chrono::microseconds >(m_time_since_day_start);
    l_microseconds -= std::chrono::microseconds{microseconds};
    if (l_microseconds.count() < 0) {
        l_microseconds += std::chrono::microseconds{microseconds_in_day};
    }
}

void tristan::time::Time::_subtractNanoseconds(uint64_t nanoseconds) {
    auto& l_nanoseconds = std::get< std::chrono::nanoseconds >(m_time_since_day_start);
    l_nanoseconds -= std::chrono::nanoseconds{nanoseconds};
    if (l_nanoseconds.count() < 0) {
        l_nanoseconds += std::chrono::nanoseconds{nanoseconds_in_day};
    }
}

namespace {
    auto checkTimeFormat(const std::string& time) -> bool {

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
            if (i == 12 && time[i] != '.') {
                return false;
            }
            if ((i > 12 && i < 16) && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
            if (i == 16 && time[i] != '.') {
                return false;
            }
            if ((i > 16 && i < 20) && (time[i] < '0' || time[i] > '9')) {
                return false;
            }
        }
        return true;
    }
}  // End of unnamed namespace