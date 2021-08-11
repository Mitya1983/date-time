#include "date.hpp"
#include <ctime>
#include <algorithm>


MT::date::Date::Date(int day, int month, int year) {
    if (day < 0 || day > 31)
    {
        std::string message = "MT::date::Date(int year, int month, int day): bad [day] value was provided - "
                + std::to_string(day) + " the value between 0 and 31 is expected";
        throw std::range_error(message);
    }
    if (month < 0 || month > 12)
    {
        std::string message = "MT::date::Date(int year, int month, int day): bad [month] value was provided - "
                + std::to_string(month) + " the value between 0 and 12 is expected";
        throw std::range_error(message);
    }
    if (year != 0 && year < 1900)
    {
        std::string message = "MT::date::Date(int year, int month, int day): bad [year] value was provided - "
                + std::to_string(month) + " the value more or equal to 1900 is expected";
        throw std::range_error(message);
    }
    time_t now = time(0);
    tm time = *localtime(&now);
    year == 0 ? m_year = time.tm_year + 1900 : m_year = year;
    month == 0 ? m_month = time.tm_mon + 1 : m_month = month;
    day == 0 ? m_mday = time.tm_mday : m_mday = day;

    _setWeekDay();
}

MT::date::Date::Date(const std::string &iso_date) {
    auto _length = iso_date.length();
    if (_length != 8 && _length != 10)
    {
        throw std::invalid_argument("Provided date has invalid format. "
                                    "Value should be provided as either 8 (YYYYMMDD) or 10 (YYYY[separator]MM[separator]DD) characters string. "
                                    "But " + std::to_string(_length) + " characters string was provided");
    }
    if (std::find_if(iso_date.begin(), iso_date.end(), [](const char c){if (c < '0' || c > '9')
                                                                        if (c != ':') return true;
                                                                        return false;}) != iso_date.end())
    {
        std::string message = "MT::date::Date::Date(const std::string &iso_date): Bad [iso_date] string format. \
                               String should contain only numbers and hyphen";
        throw std::invalid_argument(message);
    }
    m_year = std::stoi(iso_date.substr(0, 4));
    switch (_length) {
    case 8:
    {
        m_month = std::stoi(iso_date.substr(4, 2));
        m_mday = std::stoi(iso_date.substr(6, 2));
        break;
    }
    case 10:
    {
        m_month = std::stoi(iso_date.substr(5, 2));
        m_mday = std::stoi(iso_date.substr(8, 2));
        break;
    }
    }
    _setWeekDay();
}

bool MT::date::Date::operator==(const MT::date::Date &r) const {
    if (m_mday != r.m_mday)
        return false;
    if (m_month != r.m_month)
        return false;
    if (m_year != r.m_year)
        return false;

    return true;
}

bool MT::date::Date::operator<(const MT::date::Date &r) const {
    if (m_year > r.m_year) {
        return false;
    }
    if (m_month > r.m_month) {
        return false;
    }
    if (m_year == r.m_year && m_month == r.m_month) {
        if (m_mday >= r.m_mday) {
            return false;
        }
    }
    
    return true;
}

MT::date::Date &MT::date::Date::operator++() {
    switch (m_month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    {
        m_mday < 31 ? ++m_mday : (m_mday = 1, ++m_month);
        break;
    }
    case 2:
    {
        isLeapYear() ? (m_mday < 29 ? ++m_mday : (m_mday = 1, ++m_month)) : (m_mday < 28 ? ++m_mday : (m_mday = 1, ++m_month));
        break;
    }
    case 4:
    case 6:
    case 9:
    case 11:
    {
        m_mday < 30 ? ++m_mday : (m_mday = 1, ++m_month);
        break;
    }
    case 12:
    {
        m_mday < 31 ? ++m_mday : (m_mday = 1, m_month = 1, ++m_year);
        break;
    }
    }
    m_wday < 7 ? ++m_wday : m_wday = 1;

    return *this;
}

MT::date::Date MT::date::Date::operator++(int) {
    Date tmp(*this);
    ++*this;
    return tmp;
}

MT::date::Date &MT::date::Date::operator--() {
    switch (m_month)
    {
    case 1:
    {
        m_mday > 1 ? --m_mday : (m_mday = 31, m_month = 12, --m_year);
        break;
    }
    case 2:
    case 4:
    case 6:
    case 8:
    case 9:
    case 11:
    {
        m_mday > 1 ? --m_mday : (m_mday = 31, --m_month);
        break;
    }
    case 3:
    {
        isLeapYear() ? (m_mday > 1 ? ++m_mday : (m_mday = 29, --m_month)) : (m_mday > 1 ? --m_mday : (m_mday = 28, --m_month));
        break;
    }
    case 5:
    case 7:
    case 10:
    case 12:
    {
        m_mday > 1 ? --m_mday : (m_mday = 30, --m_month);
        break;
    }
    }
    m_wday > 1 ? --m_wday : m_wday = 7;

    return *this;
}

MT::date::Date MT::date::Date::operator--(int) {
    Date tmp(*this);
    --*this;
    return tmp;
}

void MT::date::Date::setDay(int day) {
    if (day < 1 || day > 31)
    {
        std::string message = "MT::date::Date::set_day(int mday): bad [mday] value was provided - "
                + std::to_string(day) + ". The value from 1 to 31 is expected";
        throw std::range_error(message);
    }
    m_mday = day;
    _setWeekDay();
}

void MT::date::Date::setMonth(int month) {
    if (month < 1 || month > 12)
    {
        std::string message = "MT::date::Date::set_day(int month): bad [month] value was provided - "
                + std::to_string(month) + ". The value from 1 to 12 is expected";
        throw std::range_error(message);
    }
    m_month = month;
    _setWeekDay();
}

void MT::date::Date::setYear(int year) {
    m_year = year;
    _setWeekDay();
}

int MT::date::Date::monthDay() const {
    return m_mday;
}

int MT::date::Date::weekDay() const {
    return m_wday;
}

int MT::date::Date::month() const {
    return m_month;
}

int MT::date::Date::year() const {
    return m_year;
}

void MT::date::Date::addDays(int days) {
    if (days == 0)
        return;
    for (int i = 0; i < days; ++i)
    {
        ++*this;
    }
}

void MT::date::Date::substractDays(int days) {
    if (days == 0)
        return;
    for (int i = 0; i < days; ++i)
    {
        --*this;
    }
}

void MT::date::Date::addMonths(int months) {
    int current_month = m_month;
    int years = months / 12;
    if (years)
        m_year += years;
    int _months = months - years * 12;
    int month_to_new_year = 12 - (m_month);
    if (month_to_new_year <= _months)
    {
        ++m_year;
    }
    for (int i = 0; i < _months; ++i)
    {
        ++m_month;
        if (m_month > 12)
            m_month = 1;
    }
    switch (m_month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
    {
        break;
    }
    case 4:
    case 6:
    case 9:
    case 11:
    {
        if (m_mday == 31 || (current_month == 2 && m_mday >= 28))
            m_mday = 30;
        break;
    }
    case 2:
    {
        if (m_mday > 28)
            isLeapYear() ? m_mday = 29 : m_mday = 28;
        break;
    }
    }
    _setWeekDay();
}

void MT::date::Date::substractMonths(int months) {
    int current_month = m_month;
    int years = months / 12;
    if (years)
        m_year -= years;
    int _months = months - years * 12;
    if (_months > 0)
    {
        if (current_month <= _months)
        {
            --m_year;
        }
        for (int i = 0; i < _months; ++i)
        {
            --m_month;
            if (m_month < 1)
                m_month = 12;
        }        
    }
    switch (m_month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
    {
        break;
    }
    case 4:
    case 6:
    case 9:
    case 11:
    {
        if (m_mday == 31)
            m_mday = 30;
        break;
    }
    case 2:
    {
        if (m_mday > 28)
            isLeapYear() ? m_mday = 29 : m_mday = 28;
        break;
    }
    }
    _setWeekDay();
}

void MT::date::Date::addYears(int years) {
    m_year += years;
    if (!isLeapYear() && m_month == 2 && m_mday == 29)
        m_mday = 28;

    _setWeekDay();
}

void MT::date::Date::substractYears(int years) {
    m_year -= years;

    if (!isLeapYear() && m_month == 2 && m_mday == 29)
        m_mday = 28;

    _setWeekDay();
}

bool MT::date::Date::isLeapYear() {
    return date::is_leap_year(m_year);
}

bool MT::date::Date::is_weekend() {
    return m_wday > 5 ? true : false;
}

std::string MT::date::Date::toString() const {
    std::string result;
    result += std::to_string(m_year);
    result += '-';
    if (m_month < 10)
        result += '0';
    result += std::to_string(m_month);
    result += '-';
    if (m_mday < 10)
        result += '0';
    result += std::to_string(m_mday);

    return result;
}

void MT::date::Date::_setWeekDay() {
        int a = (14 - m_month) / 12;
        int y = m_year - a;
        int m = m_month + 12*a - 2;
        int d = (m_mday + y + y/4 - y/100 + y/400 + (m * 31)/12)%7;
        d == 0 ? m_wday = 7 : m_wday = d;
    }

bool MT::date::operator !=(const MT::date::Date &l, const MT::date::Date &r) {
    return !(l == r);
}

bool MT::date::operator >(const MT::date::Date &l, const MT::date::Date &r) {
    return !(l < r);
}

bool MT::date::operator <=(const MT::date::Date &l, const MT::date::Date &r) {
    return l < r || l == r;
}

bool MT::date::operator >=(const MT::date::Date &l, const MT::date::Date &r) {
    return l > r || l == r;
}

std::ostream &MT::date::operator<<(std::ostream &out, const MT::date::Date &date) {
    out << date.toString();
    return out;
}

bool MT::date::is_leap_year(int year) {
    return year % 100 == 0 ? year % 400 == 0 : year % 4 == 0;
}