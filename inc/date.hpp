#ifndef DATE_HPP
#define DATE_HPP
#include <chrono>
#include <string>
#include <ostream>
namespace tristan::date {
    
    using Days = std::chrono::duration<int64_t , std::ratio_divide<std::ratio<86400>, std::chrono::seconds::period>>;
    using Weeks = std::chrono::duration<int64_t , std::ratio_divide<std::ratio<604800>, std::chrono::seconds::period>>;
    using Years = std::chrono::duration<int64_t , std::ratio_divide<std::ratio<31556926>, std::chrono::seconds::period>>;
    
    inline const auto non_leap_year_seconds = std::chrono::seconds(31536000);
    inline const auto leap_year_seconds = std::chrono::seconds(31622400);
    inline const uint16_t start_year = 1900;
    
    class Date
    {
        friend auto operator + (const Date& l, const Date &r) -> Date;
        friend auto operator - (const Date& l, const Date &r) -> Date;
        
        class Durations {
            friend class Date;
        public:
            [[nodiscard]] auto yearsSince1900() const -> const Years& {return m_years_since_1900;}
            [[nodiscard]] auto weeksFrom1900() const -> const Weeks& {return m_weeks_since_1900;}
            [[nodiscard]] auto daysSince1900() const -> const Days& {return m_days_since_1900;}
            [[nodiscard]] auto daysSinceYearStart() const -> const Days& {return m_days_since_year_start;}
            [[nodiscard]] auto daysSinceMonthStart() const -> const Days& {return m_days_since_month_start;}
        private:
            Years m_years_since_1900;
            Weeks m_weeks_since_1900;
            Days m_days_since_1900;
            Days m_days_since_year_start;
            Days m_days_since_month_start;
        };
    public:
        Date();
        explicit Date(int day, int month, int year);
        explicit Date(const std::string &iso_date);
        Date(const Date&) = default;
        Date(Date&&) = default;
        
        auto operator=(const Date&) -> Date& = default;
        auto operator=(Date&&) -> Date& = default;
        auto operator==(const Date& r) const -> bool;
        auto operator<(const Date& r) const -> bool;
        
        void operator+=(const Date& r);
        void operator-=(const Date& r);
        
        virtual ~Date() = default;
    
        void addDays(uint64_t days);
        void addMonths(uint64_t months);
        void addYears(uint64_t years);
        void substractDays(uint64_t days);
        void substractMonths(uint64_t months);
        void substractYears(uint64_t years);
        
        auto dayOfTheMonth() const -> uint8_t;
        auto dayOfTheWeek() const -> uint8_t;
        auto month() const -> uint8_t;
        auto year() const -> uint16_t;
        
        auto is_weekend() const -> bool;
        [[nodiscard]] virtual auto toString() const -> std::string;
        
        static auto isLeapYear(uint16_t year) -> bool;
    
    
    protected:
    
    private:
        Durations m_durations;
        uint8_t m_month;
    };
    
    auto operator != (const Date &l, const Date &r) -> bool;
    auto operator > (const Date &l, const Date &r) -> bool;
    auto operator <= (const Date &l, const Date &r) -> bool;
    auto operator >= (const Date &l, const Date &r) -> bool;
    auto operator + (const Date& l, const Date &r) -> Date;
    auto operator - (const Date& l, const Date &r) -> Date;
    auto operator<<(std::ostream &out, const Date &date) -> std::ostream&;
    
}//namespace tristan::date
#endif // DATE_HPP
