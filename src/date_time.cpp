#include "date_time.hpp"

MT::date_time::Date_Time::Date_Time() : MT::date::Date(), MT::time::Time()
{

}

std::string MT::date_time::Date_Time::to_string() const
{
    std::string dt;
    dt += Date::to_string();
    dt+= 'T';
    dt += Time::to_string();
    return dt;
}

std::ostream &MT::date_time::operator<<(std::ostream &out, const MT::date_time::Date_Time &dt)
{
    out << dt.to_string();
    return out;
}
