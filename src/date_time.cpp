#include "date_time.hpp"

tristan::date_time::DateTime::DateTime() :
    m_date(),
    m_time() { }

tristan::date_time::DateTime::DateTime(const std::string& date) {
    auto delimiter_pos = date.find('T');
    if (delimiter_pos == std::string::npos) {
        throw std::runtime_error("tristan::date_time::DateTime::DateTime(const std::string& time): Invalid time format");
    }
    m_date = tristan::date::Date(date.substr(0, delimiter_pos));
    m_time = tristan::time::Time(date.substr(delimiter_pos + 1));
}

auto tristan::date_time::DateTime::toString() const -> std::string {
    std::string dt;
    dt += m_date.toString();
    dt += 'T';
    dt += m_time.toString(true);
    return dt;
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
