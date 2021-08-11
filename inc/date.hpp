#ifndef MT_DATE_HPP
#define MT_DATE_HPP
#include <ctime>
#include <string>
#include <ostream>
namespace MT {

namespace date {

class Date
{
public:
    
    explicit Date(int day = 0, int month = 0, int year = 0);
    explicit Date(const std::string &iso_date);
    Date(const Date&) = default;
    Date(Date&&) = default;
    
    Date& operator=(const Date&) = default;
    Date& operator=(Date&&) = default;
    bool operator==(const Date &r) const;
    bool operator<(const Date &r) const;
    Date& operator++();
    Date operator++(int);
    Date& operator--();
    Date operator--(int);
    
    virtual ~Date() = default;
    
    void setDay(int day);
    void setMonth(int month);
    void setYear(int year);
    
    auto monthDay() const -> int;
    auto weekDay() const -> int;
    auto month() const -> int;
    auto year() const -> int;
    
    void addDays(int days);
    void substractDays(int days);
    void addMonths(int months);
    void substractMonths(int months);
    void addYears(int years);
    void substractYears(int years);
    auto isLeapYear() -> bool;
    auto is_weekend() -> bool;
    [[nodiscard]] virtual auto toString() const -> std::string;

    
protected:

private:
    int m_year;
    int m_month;
    int m_mday;
    int m_wday;

    void _setWeekDay();
};

    bool operator != (const Date &l, const Date &r);
    bool operator > (const Date &l, const Date &r);
    bool operator <= (const Date &l, const Date &r);
    bool operator >= (const Date &l, const Date &r);
    std::ostream& operator<<(std::ostream &out, const Date &date);

    bool is_leap_year(int year);
}//namespace date

}//namespace MT
#endif // MT_DATE_HPP
