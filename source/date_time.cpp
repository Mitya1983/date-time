#include "date_time.hpp"

#if defined __cpp_lib_format
  #include <format>
#endif

mt::date_time::date_time::date_time(const std::chrono::time_point< std::chrono::system_clock > p_time_point) :
    m_date{p_time_point},
    m_time(p_time_point) { }

mt::date_time::date_time::date_time(const mt::TimeZone p_time_zone) :
    m_date(p_time_zone),
    m_time(p_time_zone) { }

mt::date_time::date_time::date_time(const std::string& p_date_time) {
    const auto delimiter_pos = p_date_time.find('T');
    if (delimiter_pos == std::string::npos) {
        throw std::runtime_error("tristan::date_time::DateTime::DateTime(const std::string& time): Invalid time format");
    }
    m_date = mt::date::date(p_date_time.substr(0, delimiter_pos));
    m_time = mt::time::time(p_date_time.substr(delimiter_pos + 1));
}

void mt::date_time::date_time::set_date(const mt::date::date& p_date) { m_date = p_date; }

void mt::date_time::date_time::set_date(mt::date::date&& p_date) { m_date = p_date; }

void mt::date_time::date_time::set_time(const mt::time::time& p_time) { m_time = p_time; }

void mt::date_time::date_time::set_time(mt::time::time&& p_time) { m_time = p_time; }

auto mt::date_time::date_time::date() const -> const mt::date::date& { return m_date; }

auto mt::date_time::date_time::date() -> date::date& { return m_date; }

auto mt::date_time::date_time::time() const -> const mt::time::time& { return m_time; }

auto mt::date_time::date_time::time() -> time::time& { return m_time; }

auto mt::date_time::date_time::to_string(const std::function< std::string(const date_time&) >& formatter) const -> std::string {
    if (formatter) {
        return formatter(*this);
    }

#if defined __cpp_lib_format
    return std::format("{}T{}", m_date.to_string(), m_time.to_string());
#else
    std::string dt;
    dt += m_date.to_string();
    dt += 'T';
    dt += m_time.to_string();
    return dt;
#endif
}

auto mt::date_time::date_time::local_date_time() -> mt::date_time::date_time {
    mt::date_time::date_time l_date_time;
    l_date_time.set_date(mt::date::date::local_date());
    l_date_time.set_time(mt::time::time::local_time());

    return l_date_time;
}

auto mt::date_time::date_time::operator==(const mt::date_time::date_time& other) const -> bool { return m_date == other.m_date && m_time == other.m_time; }

auto mt::date_time::date_time::operator<(const mt::date_time::date_time& other) const -> bool {
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

void mt::date_time::date_time::operator+=(const mt::time::time_duration p_value) { *this = *this + p_value; }

void mt::date_time::date_time::operator+=(const mt::date::date_duration p_value) { *this = *this + p_value; }

void mt::date_time::date_time::operator-=(const mt::time::time_duration p_value) { *this = *this - p_value; }

void mt::date_time::date_time::operator-=(const mt::date::date_duration p_value) { *this = *this - p_value; }

auto mt::date_time::operator!=(const mt::date_time::date_time& l, const mt::date_time::date_time& r) -> bool { return not(l == r); }

auto mt::date_time::operator>(const mt::date_time::date_time& l, const mt::date_time::date_time& r) -> bool { return not(l <= r); }

auto mt::date_time::operator<=(const mt::date_time::date_time& l, const mt::date_time::date_time& r) -> bool { return l < r || l == r; }

auto mt::date_time::operator>=(const mt::date_time::date_time& l, const mt::date_time::date_time& r) -> bool { return l > r || l == r; }

mt::date_time::date_time mt::date_time::operator+(const date_time& l, const mt::time::time_duration p_time_value) {
    std::chrono::days days_to_add{0};
    std::visit(
        [&days_to_add]< typename TimeValueType >(TimeValueType&& value) -> void {
            days_to_add = std::chrono::duration_cast< std::chrono::days >(value);
        },
        p_time_value);
    date_time date_time{l};
    date_time.time() += p_time_value;
    date_time.date() += days_to_add;
    return date_time;
}

mt::date_time::date_time mt::date_time::operator+(const date_time& l, const mt::date::date_duration p_date_value) {
    date_time date_time{l};
    date_time.date() += p_date_value;
    return date_time;
}

mt::date_time::date_time mt::date_time::operator-(const date_time& l, const mt::time::time_duration p_time_value) {
    std::chrono::days days_to_subtract{0};
    std::visit(
        [&days_to_subtract]< typename TimeValueType >(TimeValueType&& value) -> void {
            days_to_subtract = std::chrono::duration_cast< std::chrono::days >(value);
        },
        p_time_value);
    date_time date_time{l};
    date_time.time() -= p_time_value;
    date_time.date() -= days_to_subtract;
    return date_time;
}

mt::date_time::date_time mt::date_time::operator-(const date_time& l, const mt::date::date_duration p_date_value) {
    date_time date_time{l};
    date_time.date() -= p_date_value;
    return date_time;
}

auto mt::date_time::operator<<(std::ostream& out, const mt::date_time::date_time& dt) -> std::ostream& {
    const auto& _string = dt.to_string();
    out.write(_string.data(), std::ssize(_string));
    return out;
}
