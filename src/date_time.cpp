#include "date_time.hpp"

#if defined __cpp_lib_format
  #include <format>
#endif

mt::date_time::DateTime::DateTime(const mt::TimeZone p_time_zone) :
    m_date(p_time_zone),
    m_time(p_time_zone) { }

mt::date_time::DateTime::DateTime(const std::string& p_date_time) {
    const auto delimiter_pos = p_date_time.find('T');
    if (delimiter_pos == std::string::npos) {
        throw std::runtime_error("tristan::date_time::DateTime::DateTime(const std::string& time): Invalid time format");
    }
    m_date = mt::date::Date(p_date_time.substr(0, delimiter_pos));
    m_time = mt::time::Time(p_date_time.substr(delimiter_pos + 1));
}

void mt::date_time::DateTime::setDate(const mt::date::Date& p_date) { m_date = p_date; }

void mt::date_time::DateTime::setDate(mt::date::Date&& p_date) { m_date = p_date; }

void mt::date_time::DateTime::setTime(const mt::time::Time& p_time) { m_time = p_time; }

void mt::date_time::DateTime::setTime(mt::time::Time&& p_time) { m_time = p_time; }

auto mt::date_time::DateTime::date() const -> const mt::date::Date& { return m_date; }

auto mt::date_time::DateTime::date() -> date::Date& { return m_date; }

auto mt::date_time::DateTime::time() const -> const mt::time::Time& { return m_time; }

auto mt::date_time::DateTime::time() -> time::Time& { return m_time; }

auto mt::date_time::DateTime::toString(const std::function< std::string(const DateTime&) >& formatter) const -> std::string {
    if (formatter) {
        return formatter(*this);
    }

#if defined __cpp_lib_format
    return std::format("{}T{}", m_date.toString(), m_time.toString());
#else
    std::string dt;
    dt += p_date.date().toString();
    dt += 'T';
    dt += p_date.time().toString();
    return dt;
#endif
}

auto mt::date_time::DateTime::localDateTime() -> mt::date_time::DateTime {
    mt::date_time::DateTime l_date_time;
    l_date_time.setDate(mt::date::Date::localDate());
    l_date_time.setTime(mt::time::Time::localTime());

    return l_date_time;
}

auto mt::date_time::DateTime::operator==(const mt::date_time::DateTime& other) const -> bool { return m_date == other.m_date && m_time == other.m_time; }

auto mt::date_time::DateTime::operator<(const mt::date_time::DateTime& other) const -> bool {
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

void mt::date_time::DateTime::operator+=(const mt::time::TimeDuration p_value) { *this = *this + p_value; }

void mt::date_time::DateTime::operator+=(const mt::date::DateDuration p_value) { *this = *this + p_value; }

void mt::date_time::DateTime::operator-=(const mt::time::TimeDuration p_value) { *this = *this + p_value; }

void mt::date_time::DateTime::operator-=(const mt::date::DateDuration p_value) { *this = *this + p_value; }

auto mt::date_time::operator!=(const mt::date_time::DateTime& l, const mt::date_time::DateTime& r) -> bool { return not(l == r); }

auto mt::date_time::operator>(const mt::date_time::DateTime& l, const mt::date_time::DateTime& r) -> bool { return not(l <= r); }

auto mt::date_time::operator<=(const mt::date_time::DateTime& l, const mt::date_time::DateTime& r) -> bool { return l < r || l == r; }

auto mt::date_time::operator>=(const mt::date_time::DateTime& l, const mt::date_time::DateTime& r) -> bool { return l > r || l == r; }

mt::date_time::DateTime mt::date_time::operator+(const DateTime& l, const mt::time::TimeDuration p_time_value) {
    std::chrono::days days_to_add{0};
    std::visit(
        [&days_to_add]< typename TimeValueType >(TimeValueType&& value) -> void {
            days_to_add = std::chrono::duration_cast< std::chrono::days >(value);
        },
        p_time_value);
    DateTime date_time{l};
    date_time.time() += p_time_value;
    date_time.date() += days_to_add;
    return date_time;
}

mt::date_time::DateTime mt::date_time::operator+(const DateTime& l, const mt::date::DateDuration p_date_value) {
    DateTime date_time{l};
    date_time.date() += p_date_value;
    return date_time;
}

mt::date_time::DateTime mt::date_time::operator-(const DateTime& l, const mt::time::TimeDuration p_time_value) {
    std::chrono::days days_to_subtract{0};
    std::visit(
        [&days_to_subtract]< typename TimeValueType >(TimeValueType&& value) -> void {
            days_to_subtract = std::chrono::duration_cast< std::chrono::days >(value);
        },
        p_time_value);
    DateTime date_time{l};
    date_time.time() -= p_time_value;
    date_time.date() -= days_to_subtract;
    return date_time;
}

mt::date_time::DateTime mt::date_time::operator-(const DateTime& l, const mt::date::DateDuration p_date_value) {
    DateTime date_time{l};
    date_time.date() -= p_date_value;
    return date_time;
}

auto mt::date_time::operator<<(std::ostream& out, const mt::date_time::DateTime& dt) -> std::ostream& {
    const auto& _string = dt.toString();
    out.write(_string.data(), std::ssize(_string));
    return out;
}
