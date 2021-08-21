#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP
#include "date.hpp"
#include "time.hpp"

namespace tristan::date_time{

class Date_Time
{
public:
    Date_Time();
    Date_Time(const Date_Time&) = default;
    Date_Time(Date_Time&&) = default;
    
    Date_Time& operator=(const Date_Time&) = default;
    Date_Time& operator=(Date_Time&&) = default;
    
    auto operator==(const Date_Time& other) -> bool;
    auto operator<(const Date_Time& other) -> bool;
    
    ~Date_Time() = default;
    
    void setDate(const date::Date& date) {m_date = date;}
    void setDate(date::Date&& date) {m_date = date;}
    void setTime(const time::DayTime& time) {m_day_time = time;}
    void setTime(time::DayTime&& time) {m_day_time = time;}
    
    [[nodiscard]] auto date() const -> const date::Date& {return m_date;}
    [[nodiscard]] auto time() const -> const time::DayTime& {return m_day_time;}
    
    [[nodiscard]] virtual auto toString() const -> std::string;

protected:

private:
    date::Date m_date;
    time::DayTime m_day_time;
};

auto operator<<(std::ostream &out, const Date_Time &dt) -> std::ostream&;

}// namespace tristan::date_time

#endif // DATE_TIME_HPP
