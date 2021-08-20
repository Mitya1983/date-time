#include "date.hpp"
#include <ctime>
#include <algorithm>

tristan::date::Date::Date() : m_durations(){
    auto time_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::duration(std::chrono::system_clock::now().time_since_epoch())
    );
    uint16_t year = 1970;
    auto seconds_current_year = time_since_epoch;
    while (true){
        if (seconds_current_year < non_leap_year_seconds){
            break;
        }
        if (is_leap_year(year)){
            seconds_current_year -= leap_year_seconds;
        }
        else{
            seconds_current_year -= non_leap_year_seconds;
        }
        ++year;
    }
    m_durations.m_years_since_1900 = tristan::date::Years(year - start_year);
    m_durations.m_weeks_since_1900 = std::chrono::duration_cast<tristan::date::Weeks>(time_since_epoch);
    m_durations.m_days_since_1900 = std::chrono::duration_cast<tristan::date::Days>(time_since_epoch);
    m_durations.m_days_since_year_start = std::chrono::duration_cast<tristan::date::Days>(seconds_current_year);
    m_durations.m_days_since_month_start = m_durations.m_days_since_year_start;
    for (m_month = 1; m_month <= 12; ++m_month){
        if (m_month == 1 || m_month == 3 || m_month == 5 || m_month == 7 || m_month == 8 || m_month == 10 || m_month == 12){
            if (m_durations.m_days_since_month_start.count() <= 31){
                break;
            }
            m_durations.m_days_since_month_start -= tristan::date::Days(31);
        }
        else if (m_month == 4 || m_month == 6 || m_month == 9 || m_month == 11){
            if (m_durations.m_days_since_month_start.count() <= 30){
                break;
            }
            m_durations.m_days_since_month_start -= tristan::date::Days(30);
        }
        else{
            bool leap_year = isLeapYear(year);
            if ((!leap_year && m_durations.m_days_since_month_start.count() <= 28 || (leap_year && m_durations.m_days_since_month_start.count() <= 29))){
                break;
            }
            if (!leap_year){
                m_durations.m_days_since_month_start -= tristan::date::Days(28);
            }
            else{
                m_durations.m_days_since_month_start -= tristan::date::Days(29);
            }
        }
    }
    //That increment is needed as month variable holds current month number and not month passed since start of the year
    ++m_month;
}

tristan::date::Date::Date(int day, int month, int year)  : m_durations(){
    if (day < 1 || day > 31)
    {
        std::string message = "tristan::date::Date(int year, int month, int day): bad [day] value was provided - "
                              + std::to_string(day) + " the value between 0 and 31 is expected";
        throw std::range_error(message);
    }
    if (month < 1 || month > 12)
    {
        std::string message = "tristan::date::Date(int year, int month, int day): bad [month] value was provided - "
                              + std::to_string(month) + " the value between 0 and 12 is expected";
        throw std::range_error(message);
    }
    if ((m_month == 4 || m_month == 6 || m_month == 9 || m_month == 11) && day == 31){
        std::string message = "tristan::date::Date(int year, int month, int day): date 31 is not possible for provided month " + std::to_string(m_month);
        throw std::range_error(message);
    }
    else if (m_month == 2){
        bool leap_year = tristan::date::Date::isLeapYear(year);
        if ((leap_year && day > 29) || (!leap_year && day > 28)){
            std::string message =
                    "tristan::date::Date(int year, int month, int day): date " + std::to_string(day) + " is not possible for provided month " +
                    std::to_string(m_month);
            throw std::range_error(message);
        }
    }
    if (year < 1900)
    {
        std::string message = "tristan::date::Date(int year, int month, int day): bad [year] value was provided - "
                              + std::to_string(year) + " the value more or equal to 1900 is expected";
        throw std::range_error(message);
    }
    m_durations.m_days_since_month_start = tristan::date::Days(day - 1);
    for (int l_month = 1; l_month < month; ++l_month){
        if (m_month == 1 || m_month == 3 || m_month == 5 || m_month == 7 || m_month == 8 || m_month == 10 || m_month == 12){
            m_durations.m_days_since_year_start += tristan::date::Days(31);
        }
        else if (m_month == 4 || m_month == 6 || m_month == 9 || m_month == 11){
            m_durations.m_days_since_year_start += tristan::date::Days(30);
        }
        else{
            bool leap_year = isLeapYear(year);
            if (!leap_year){
                m_durations.m_days_since_year_start += tristan::date::Days(28);
            }
            else{
                m_durations.m_days_since_year_start += tristan::date::Days(29);
            }
        }
    }
    m_month = month;
    m_durations.m_years_since_1900 = tristan::date::Years(year - start_year);
    m_durations.m_days_since_1900 = std::chrono::duration_cast<tristan::date::Days>(m_durations.m_years_since_1900) + m_durations.m_days_since_year_start;
    m_durations.m_weeks_since_1900 = std::chrono::duration_cast<tristan::date::Weeks>(m_durations.m_days_since_1900);
}

tristan::date::Date::Date(const std::string &iso_date){
    auto _length = iso_date.length();
    if (_length != 8 && _length != 10)
    {
        throw std::invalid_argument("Provided date has invalid format. "
                                    "Value should be provided as either 8 (YYYYMMDD) or 10 (YYYY-MM-DD) characters string. "
                                    "But " + std::to_string(_length) + " characters string was provided");
    }
    if (std::find_if(iso_date.begin(), iso_date.end(), [](const char c){if (c < '0' || c > '9')
        if (c != '-') return true;
        return false;}) != iso_date.end())
    {
        std::string message = "tristan::date::Date::Date(const std::string &iso_date): Bad [iso_date] string format. String should contain only numbers and hyphen";
        throw std::invalid_argument(message);
    }
    uint16_t year = static_cast<uint16_t>(std::stoi(iso_date.substr(0, 4)));
    uint8_t month = 0;
    uint8_t day = 0;
    
    switch (_length) {
        case 8:
        {
            month = std::stoi(iso_date.substr(4, 2));
            day = std::stoi(iso_date.substr(6, 2));
            break;
        }
        case 10:
        {
            month = std::stoi(iso_date.substr(5, 2));
            day = std::stoi(iso_date.substr(8, 2));
            break;
        }
    }
    *this = std::move(tristan::date::Date(day, month, year));
}

auto tristan::date::Date::operator==(const tristan::date::Date &r) const -> bool{
    if (m_durations.m_days_since_year_start != r.m_durations.m_days_since_year_start)
        return false;
    if (m_durations.m_years_since_1900 != r.m_durations.m_years_since_1900)
        return false;
    
    return true;
}

auto tristan::date::Date::operator<(const tristan::date::Date &r) const -> bool{
    if (m_durations.m_years_since_1900 > r.m_durations.m_years_since_1900) {
        return false;
    }
    if (m_durations.m_years_since_1900 == r.m_durations.m_years_since_1900) {
        if (m_durations.m_days_since_year_start >= r.m_durations.m_days_since_year_start) {
            return false;
        }
    }
    return true;
}

void tristan::date::Date::operator+=(const tristan::date::Date& r){
    //TODO
}

void tristan::date::Date::operator-=(const tristan::date::Date& r){
    //TODO
}

void tristan::date::Date::addDays(uint64_t days){
    if (days == 0){
        return;
    }
    m_durations.m_days_since_1900 += tristan::date::Days(days);
    m_durations.m_years_since_1900 = std::chrono::duration_cast<tristan::date::Years>(m_durations.m_days_since_1900);
    m_durations.m_days_since_year_start = m_durations.m_days_since_1900 - std::chrono::duration_cast<tristan::date::Days>(m_durations.m_years_since_1900);
    m_durations.m_days_since_month_start = m_durations.m_days_since_year_start;
    for (m_month = 1; m_month <= 12; ++m_month){
        if (m_month == 1 || m_month == 3 || m_month == 5 || m_month == 7 || m_month == 8 || m_month == 10 || m_month == 12){
            if (m_durations.m_days_since_month_start.count() <= 31){
                break;
            }
            m_durations.m_days_since_month_start -= tristan::date::Days(31);
        }
        else if (m_month == 4 || m_month == 6 || m_month == 9 || m_month == 11){
            if (m_durations.m_days_since_month_start.count() <= 30){
                break;
            }
            m_durations.m_days_since_month_start -= tristan::date::Days(30);
        }
        else{
            bool leap_year = isLeapYear(m_durations.m_years_since_1900.count() + start_year);
            if ((!leap_year && m_durations.m_days_since_month_start.count() <= 28 || (leap_year && m_durations.m_days_since_month_start.count() <= 29))){
                break;
            }
            if (!leap_year){
                m_durations.m_days_since_month_start -= tristan::date::Days(28);
            }
            else{
                m_durations.m_days_since_month_start -= tristan::date::Days(29);
            }
        }
    }
    //That increment is needed as month variable holds current month number and not month passed since start of the year
    ++m_month;
}

void tristan::date::Date::addMonths(uint64_t months){
    //TODO
}

void tristan::date::Date::addYears(uint64_t years){
    //TODO
}

void tristan::date::Date::substractDays(uint64_t days){
    //TODO
}

void tristan::date::Date::substractMonths(uint64_t months){
    //TODO
}

void tristan::date::Date::substractYears(uint64_t years){
    //TODO
}

auto tristan::date::Date::dayOfTheMonth() const -> uint8_t{
    return m_durations.m_days_since_month_start.count() + 1;
}

auto tristan::date::Date::dayOfTheWeek() const -> uint8_t{
    return (m_durations.m_days_since_1900.count() + 1) % 7 + 1;
}

auto tristan::date::Date::month() const -> uint8_t{
    return m_month;
}

auto tristan::date::Date::year() const -> uint16_t{
    return m_durations.m_years_since_1900.count() + start_year;
}

auto tristan::date::Date::is_weekend() const -> bool{
    return this->dayOfTheWeek() > 5;
}

std::string tristan::date::Date::toString() const {
    std::string result;
    result += std::to_string(m_durations.m_years_since_1900.count() + start_year);
    result += '-';
    if (m_month < 10)
        result += '0';
    result += std::to_string(m_month);
    result += '-';
    uint8_t day = m_durations.m_days_since_month_start.count() + 1;
    if (day < 10)
        result += '0';
    result += std::to_string(day);
    
    return result;
}

bool tristan::date::Date::isLeapYear(uint16_t year){
    if (year % 4 == 0){
        if (year % 100 == 0){
            if (year % 400 == 0){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return true;
        }
    }
    else{
        return false;
    }
}

bool tristan::date::operator !=(const tristan::date::Date &l, const tristan::date::Date &r) {
    return !(l == r);
}

bool tristan::date::operator >(const tristan::date::Date &l, const tristan::date::Date &r) {
    return !(l < r);
}

bool tristan::date::operator <=(const tristan::date::Date &l, const tristan::date::Date &r) {
    return l < r || l == r;
}

bool tristan::date::operator >=(const tristan::date::Date &l, const tristan::date::Date &r) {
    return l > r || l == r;
}

auto tristan::date::operator-(const tristan::date::Date& l, const tristan::date::Date& r) -> tristan::date::Date{
    //TODO
}

auto tristan::date::operator+(const tristan::date::Date& l, const tristan::date::Date& r) -> tristan::date::Date{
    //TODO
}

std::ostream &tristan::date::operator<<(std::ostream &out, const tristan::date::Date &date) {
    out << date.toString();
    return out;
}
