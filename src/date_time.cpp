#include "date_time.hpp"

namespace {

    [[maybe_unused]] constexpr uint16_t g_seconds_in_hour = 3600;
    [[maybe_unused]] constexpr uint8_t g_seconds_in_minute = 60;
    [[maybe_unused]] constexpr uint8_t g_minutes_in_hour = 60;
    constexpr uint8_t g_hours_in_day = 24;

}  //End of anonymous namespace

tristan::date_time::DateTime::DateTime(tristan::time::Precision p_precision) :
    m_date(),
    m_time(p_precision) { }

tristan::date_time::DateTime::DateTime(tristan::TimeZone p_time_zone, tristan::time::Precision p_precision) :
    m_date(p_time_zone),
    m_time(p_time_zone, p_precision) { }

tristan::date_time::DateTime::DateTime(const std::string& p_date_time) {
    auto delimiter_pos = p_date_time.find('T');
    if (delimiter_pos == std::string::npos) {
        throw std::runtime_error("tristan::date_time::DateTime::DateTime(const std::string& time): Invalid time format");
    }
    m_date = tristan::date::Date(p_date_time.substr(0, delimiter_pos));
    m_time = tristan::time::Time(p_date_time.substr(delimiter_pos + 1));
}

void tristan::date_time::DateTime::setDate(const tristan::date::Date& p_date) { m_date = p_date; }

void tristan::date_time::DateTime::setDate(tristan::date::Date&& p_date) { m_date = p_date; }

void tristan::date_time::DateTime::setTime(const tristan::time::Time& p_time) { m_time = p_time; }

void tristan::date_time::DateTime::setTime(tristan::time::Time&& p_time) { m_time = p_time; }

void tristan::date_time::DateTime::addSeconds([[maybe_unused]] uint64_t p_seconds) {
    uint64_t l_minutes = 0;
    if (p_seconds >= g_seconds_in_minute) {
        l_minutes = p_seconds / g_seconds_in_minute;
        p_seconds -= l_minutes * g_seconds_in_minute;
    }
    auto l_future_number_of_seconds = m_time.seconds() + p_seconds;
    if (l_future_number_of_seconds >= g_seconds_in_minute) {
        ++l_minutes;
        p_seconds = g_seconds_in_minute - p_seconds;
        m_time.subtractSeconds(p_seconds);
    } else {
        m_time.addSeconds(p_seconds);
    }
    if (l_minutes != 0) {
        addMinutes(l_minutes);
    }
}

void tristan::date_time::DateTime::addMinutes(uint64_t p_minutes) {
    uint64_t l_hours = 0;
    if (p_minutes >= g_minutes_in_hour) {
        l_hours = p_minutes / g_minutes_in_hour;
        p_minutes -= l_hours * g_minutes_in_hour;
    }
    auto l_future_number_of_minutes = m_time.minutes() + p_minutes;
    if (l_future_number_of_minutes >= g_minutes_in_hour) {
        ++l_hours;
        p_minutes = g_minutes_in_hour - p_minutes;
        m_time.subtractMinutes(p_minutes);
    } else {
        m_time.addMinutes(p_minutes);
    }

    if (l_hours != 0) {
        addHours(l_hours);
    }
}

void tristan::date_time::DateTime::addHours(uint64_t p_hours) {
    uint64_t l_days = 0;
    if (p_hours >= g_hours_in_day) {
        l_days = p_hours / g_hours_in_day;
        p_hours -= l_days * g_hours_in_day;
    }
    auto l_future_number_of_hours = m_time.hours() + p_hours;
    if (l_future_number_of_hours >= g_hours_in_day) {
        ++l_days;
    }
    m_time.addHours(p_hours);
    if (l_days != 0) {
        m_date.addDays(l_days);
    }
}

void tristan::date_time::DateTime::addDays(uint64_t p_days) { m_date.addDays(p_days); }

void tristan::date_time::DateTime::addMonths(uint64_t p_months) { m_date.addMonths(p_months); }

void tristan::date_time::DateTime::addYears(uint64_t p_years) { m_date.addYears(p_years); }

void tristan::date_time::DateTime::subtractSeconds([[maybe_unused]] uint64_t p_seconds) {
    uint64_t l_minutes = 0;
    if (p_seconds >= g_seconds_in_minute) {
        l_minutes = p_seconds / g_seconds_in_minute;
        p_seconds -= l_minutes * g_seconds_in_minute;
    }
    if (l_minutes != 0) {
        subtractMinutes(l_minutes);
    }
    auto currentHour = m_time.hours();
    m_time.subtractSeconds(p_seconds);
    if (currentHour < m_time.hours()) {
        m_date.subtractDays(1);
    }
}

void tristan::date_time::DateTime::subtractMinutes(uint64_t p_minutes) {
    uint64_t l_hours = 0;
    if (p_minutes >= g_hours_in_day) {
        l_hours = p_minutes / g_minutes_in_hour;
        p_minutes -= l_hours * g_minutes_in_hour;
    }
    if (l_hours != 0) {
        subtractHours(l_hours);
    }
    auto currentHour = m_time.hours();
    m_time.subtractMinutes(p_minutes);
    if (currentHour < m_time.hours()) {
        m_date.subtractDays(1);
    }
}

void tristan::date_time::DateTime::subtractHours(uint64_t p_hours) {
    uint64_t l_days = 0;
    if (p_hours >= g_hours_in_day) {
        l_days = p_hours / g_hours_in_day;
        p_hours -= l_days * g_hours_in_day;
    }
    auto currentHour = m_time.hours();
    m_time.subtractHours(p_hours);
    if (currentHour + g_hours_in_day - m_time.hours() == p_hours) {
        ++l_days;
    }
    if (l_days != 0) {
        m_date.subtractDays(l_days);
    }
}

void tristan::date_time::DateTime::subtractDays(uint64_t p_days) { m_date.subtractDays(p_days); }

void tristan::date_time::DateTime::subtractMonths(uint64_t p_months) { m_date.subtractMonths(p_months); }

void tristan::date_time::DateTime::subtractYears(uint64_t p_years) { m_date.subtractYears(p_years); }

auto tristan::date_time::DateTime::date() const -> const tristan::date::Date& { return m_date; }

auto tristan::date_time::DateTime::time() const -> const tristan::time::Time& { return m_time; }

auto tristan::date_time::DateTime::toString(const std::function< std::string(const DateTime&) >& formatter) const -> std::string {
    if (not formatter) {
        return formatter(*this);
    }

    auto default_formatter = [](const tristan::date_time::DateTime& p_date) -> std::string {
        std::string dt;
        dt += p_date.date().toString();
        dt += 'T';
        dt += p_date.time().toString();
        return dt;
    };

    return default_formatter(*this);
}

auto tristan::date_time::DateTime::localDateTime() -> tristan::date_time::DateTime {
    tristan::date_time::DateTime l_date_time;
    l_date_time.setDate(tristan::date::Date::localDate());
    l_date_time.setTime(tristan::time::Time::localTime());

    return l_date_time;
}

auto tristan::date_time::DateTime::operator==(const tristan::date_time::DateTime& other) const -> bool {
    return m_date == other.m_date && m_time == other.m_time;
}

auto tristan::date_time::DateTime::operator<(const tristan::date_time::DateTime& other) const -> bool {
    if (m_date > other.m_date) {
        return false;
    }
    if (m_date == other.m_date) {
        if (m_time >= other.m_time) {
            return false;
        }
    }
    return true;
}

auto tristan::date_time::operator<<(std::ostream& out, const tristan::date_time::DateTime& dt) -> std::ostream& {
    out << dt.toString();
    return out;
}

auto tristan::date_time::operator!=(const tristan::date_time::DateTime& l, const tristan::date_time::DateTime& r) -> bool { return !(l == r); }

auto tristan::date_time::operator>(const tristan::date_time::DateTime& l, const tristan::date_time::DateTime& r) -> bool { return !(l <= r); }

auto tristan::date_time::operator<=(const tristan::date_time::DateTime& l, const tristan::date_time::DateTime& r) -> bool { return (l < r || l == r); }

auto tristan::date_time::operator>=(const tristan::date_time::DateTime& l, const tristan::date_time::DateTime& r) -> bool { return (l > r || l == r); }
