#include "date_time.hpp"

tristan::date_time::Date_Time::Date_Time() :
    m_date(),
    m_day_time()
{

}

auto tristan::date_time::Date_Time::toString() const -> std::string{
    std::string dt;
    dt += m_date.toString();
    dt+= 'T';
    dt += m_day_time.toString(true);
    return dt;
}

auto tristan::date_time::Date_Time::operator==(const tristan::date_time::Date_Time& other) -> bool{
    return m_date == other.m_date && m_day_time == other.m_day_time;
}

auto tristan::date_time::Date_Time::operator<(const tristan::date_time::Date_Time& other) -> bool{
    if (m_date > other.m_date){
        return false;
    }
    if (m_date == other.m_date){
        if (m_day_time >= other.m_day_time){
            return false;
        }
    }
    return true;
}

auto tristan::date_time::operator<<(std::ostream &out, const tristan::date_time::Date_Time &dt) -> std::ostream&{
    out << dt.toString();
    return out;
}
