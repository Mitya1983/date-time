#ifndef DATE_HPP
#define DATE_HPP
#include <chrono>
#include <string>
#include <ostream>
namespace tristan::date {
    
    using Days = std::chrono::duration<int64_t , std::ratio_divide<std::ratio<86400>, std::chrono::seconds::period>>;
    
    /// \brief Number of days in regular year.
    inline const uint16_t non_leap_year_days{365};
    /// \brief Number of days in leap year.
    inline const uint16_t leap_year_days{366};
    /// \brief Number of days from 1 January 1900 to 1 January 1970.
    inline const uint16_t days_since_1900_to_1970{25567};
    inline const uint16_t start_year = 1900;
    
    class Date
    {
    
    public:
        /// \brief Creates Date object which represent current system date.
        Date();
        /// \brief Creates Date object with specified day, month and year.
        /// \param day uint8_t.
        /// \param month uint8_t.
        /// \param year uint16_t.
        /// \throws std::range_error.
        explicit Date(uint8_t day, uint8_t month, uint16_t year);
        /// \brief Creates Date object from std::string representing the date in [YYYYMMDD] or [YYYY-MM-DD] formats.
        /// \param iso_date - std::string.
        /// \throws std::invalid_argument - if time representation has invalid format.
        /// \throws std::range_error - if time representation has invalid values.
        explicit Date(const std::string &iso_date);
        /// \brief Copy constructor.
        Date(const Date&) = default;
        /// \brief Move constructor.
        Date(Date&&) = default;
         
         /// \brief Copy assignment operator.
        auto operator=(const Date&) -> Date& = default;
        /// \brief Move assignment operator.
        auto operator=(Date&&) -> Date& = default;
        /// \brief Operator equal.
        auto operator==(const Date& r) const -> bool;
        /// \brief Operator less.
        auto operator<(const Date& r) const -> bool;
        
        /// \brief Destructor
        virtual ~Date() = default;
        
        /// \brief Adds days to Date object.
        /// \param days uint64_t.
        void addDays(uint64_t days);
        /// \brief Adds months to Date object.
        /// \param days uint64_t.
        void addMonths(uint64_t months);
        /// \brief Adds years to Date object.
        /// \param days uint64_t.
        void addYears(uint64_t years);
        /// \brief Subtracts days to Date object.
        /// \param days uint64_t.
        void subtractDays(uint64_t days);
        /// \brief Subtracts months to Date object.
        /// \param days uint64_t.
        void subtractMonths(uint64_t months);
        /// \brief Subtracts years to Date object.
        /// \param days uint64_t.
        void subtractYears(uint64_t years);
        
        /// \brief Returns currently set day of the month.
        /// \note This function returns actual, or otherworldly current, day of the months and not the total number of days passed in the month.
        /// \return uint8_t.
        [[nodiscard]] auto dayOfTheMonth() const -> uint8_t;
        /// \brief Returns currently set day of the week.
        /// \note This function returns actual, or otherworldly current, day of the week and not the total number of days passed in the week.
        /// \return uint8_t.
        [[nodiscard]] auto dayOfTheWeek() const -> uint8_t;
        /// \brief Returns currently set month of the year.
        /// \return uint8_t.
        [[nodiscard]] auto month() const -> uint8_t;
        /// \brief Returns currently set year.
        /// \return uint8_t.
        [[nodiscard]] auto year() const -> uint16_t;
    
        /// \brief Returns if currently set day of the week is weekend.
        /// \note Saturday and Sunday are considered as weekend days.
        /// \return bool.
        [[nodiscard]] auto isWeekend() const -> bool;
        /// \brief Return string representation of date in YYYY-MM-DD format.
        /// \return std::string.
        [[nodiscard]] virtual auto toString() const -> std::string;
        /// \brief Checks if year is leap year.
        /// \param year uint16_t.
        /// \return bool.
        [[nodiscard]] static auto isLeapYear(uint16_t year) -> bool;
    
    protected:
    
    private:
        Days m_days_since_1900;
    };
    /// \brief Operator not equal to.
    auto operator != (const Date &l, const Date &r) -> bool;
    /// \brief Operator greater than.
    auto operator > (const Date &l, const Date &r) -> bool;
    /// \brief Operator less than or equal to.
    auto operator <= (const Date &l, const Date &r) -> bool;
    /// \brief Operator greater than or equal to.
    auto operator >= (const Date &l, const Date &r) -> bool;
    /// \brief std::ostream operator is used for convenience. Date::toString() is used.
    auto operator<<(std::ostream &out, const Date &date) -> std::ostream&;
    
}//namespace tristan::date
#endif // DATE_HPP
