#ifndef DATE_HPP
#define DATE_HPP

#include "time_zones.hpp"

#include <chrono>
#include <string>
#include <ostream>
#include <functional>
#include <random>
#include <variant>

/**
 * \brief Namespace which includes date handlers
 */
namespace mt::date {

    // /**
    //  * \brief Days duration
    //  */
    // using Days = std::chrono::duration< int64_t, std::ratio_divide< std::ratio< 86400 >, std::chrono::seconds::period > >;

    /**
     * \brief Class to handle date
     * \headerfile date.hpp
     */

    using DateDuration = std::variant< std::chrono::days, std::chrono::months, std::chrono::years >;

    class Date {
        friend auto operator+(Date p_date, DateDuration p_value) -> Date;
        friend auto operator-(Date p_date, DateDuration p_value) -> Date;

    public:
        /**
         * \brief Default constructor.
         * Creates Date object which represent current date based on UTC time zone
         */
        Date();
        explicit Date(std::chrono::seconds since_epoch);
        /**
         * \overload
         * \brief Overloaded constructor
         * Creates Date object which represent current date based on provided time zone
         * \param p_time_zone
         */
        explicit Date(TimeZone p_time_zone);
        /**
         * \overload
         * \brief Overloaded constructor
         * Creates Date object with specified day, month and year.
         * \param p_year std::chrono::year.
         * \param p_month std::chrono::month.
         * \param p_day std::chrono::day.
         * \throws std::range_error.
         */
        explicit Date(std::chrono::year p_year, std::chrono::month p_month, std::chrono::day p_day);
        /**
         * \overload
         * \brief Overloaded constructor
         * Creates Date object with specified day, month and year.
         * \param p_days std::chrono::days.
         * \param p_months std::chrono::months.
         * \param p_years std::chrono::years.
         * \throws std::range_error.
         */
        explicit Date(std::chrono::years p_years, std::chrono::months p_months, std::chrono::days p_days);
        /**
         * \overload
         * \brief Overloaded constructor
         * Creates Date object from std::string representing the date in [YYYYMMDD] or [YYYY-MM-DD] formats.
         * \param p_iso_date const std::string&.
         * \throws std::range_error - if date representation has invalid values.
         */
        explicit Date(const std::string& p_iso_date);
        /**
         * \brief Copy constructor
         */
        Date(const Date&) = default;
        /**
         * \brief Move constructor
         */
        Date(Date&&) = default;

        /**
         * \brief Copy assignment operator
         * \return Date&
         */
        auto operator=(const Date&) -> Date& = default;
        /**
         * \brief Move assignment operator
         * \return Date&
         */
        auto operator=(Date&&) -> Date& = default;
        /// \brief Operator equal.
        /**
         * \brief Operator ==
         * \param other const Date&
         * \return bool
         */
        auto operator==(const Date& other) const -> bool;
        /**
         * \brief Operator <
         * \param other const Date&
         * \return bool
         */
        auto operator<(const Date& other) const -> bool;
        /**
         * \brief Adds specified value
         * \param p_value DateValue
         */
        void operator+=(DateDuration p_value);
        /**
         * \brief Subtracts specified value
         * \param p_value DateValue
         */
        void operator-=(DateDuration p_value);

        /**
         * \brief Destructor
         */
        ~Date() = default;
        /**
         * \brief Returns currently set date.
         * \return std::chrono::year_month_day
         */
        [[nodiscard]] auto date() const -> std::chrono::year_month_day;
        /**
         * \brief Returns currently set day of the month.
         * \note This function returns actual, or otherworldly current, day of the months and not the total number of days passed in the month.
         * \return uint8_t.
         */
        [[nodiscard]] auto monthDay() const -> std::chrono::day;
        /**
         * \brief Helper function to get month as an integer value (except bool)
         * \tparam OType output type
         * \return OType
         */
        template < class OType >
            requires(std::is_integral_v< OType > && !std::same_as< bool, OType >)
        [[nodiscard]] auto monthDay() const -> OType;
        /**
         * \brief Returns currently set day of the week.
         * \note This function returns actual, or otherworldly current, day of the week and not the total number of days passed in the week.
         * \return uint8_t.
         */
        [[nodiscard]] auto weekDay() const -> std::chrono::weekday;
        /**
         * \brief Helper function to get month as an integer value (except bool)
         * \tparam OType output type
         * \return OType
         */
        template < class OType >
            requires(std::is_integral_v< OType > && !std::same_as< bool, OType >)
        [[nodiscard]] auto weekDay() const -> OType;
        /**
         * \brief Returns if currently set day of the week is weekend.
         * \note Saturday and Sunday are considered as weekend days.
         * \return bool.
         */
        [[nodiscard]] auto isWeekend() const -> bool;
        /**
         * \brief Returns currently set month of the year.
         * \return Months.
         */
        [[nodiscard]] auto month() const -> std::chrono::month;
        /**
         * \brief Helper function to get month as an integer value (except bool)
         * \tparam OType output type
         * \return OType
         */
        template < class OType >
            requires(std::is_integral_v< OType > && !std::same_as< bool, OType >)
        [[nodiscard]] auto month() const -> OType;
        /**
         * \brief Returns currently set year.
         * \return uint8_t.
         */
        [[nodiscard]] auto year() const -> std::chrono::year;
        template < class OType >
            requires(std::is_integral_v< OType > && !std::same_as< bool, OType >)
        [[nodiscard]] auto year() const -> OType;
        /**
         * \brief Generates string representation of date in ISO standard representation format. Or using provided formatter.
         * \return std::string.
         */
        [[nodiscard]] auto toString(const std::function< std::string(const Date&) >& formatter = {}) const -> std::string;
        /**
         * \brief Creates Date object which represents local date.
         * \return Date.
         */
        [[nodiscard]] static auto localDate() -> Date;

    private:
        std::chrono::year_month_day m_date{};
    };

    template < class OType >
        requires(std::is_integral_v< OType > && !std::same_as< bool, OType >)
    auto Date::monthDay() const -> OType {
        if constexpr (std::convertible_to< std::chrono::day, OType >) {
            return static_cast< OType >(m_date.day());
        }
        return static_cast< OType >(static_cast< uint32_t >(m_date.day()));
    }

    template < class OType >
        requires(std::is_integral_v< OType > && !std::same_as< bool, OType >)
    auto Date::weekDay() const -> OType {
        if constexpr (std::is_same_v<OType, uint32_t>) {
            return weekDay().c_encoding();
        }
        if constexpr (std::convertible_to< std::chrono::weekday, OType >) {
            return static_cast< OType >(weekDay());
        }
        return static_cast< OType >(weekDay().c_encoding());
    }

    template < class OType >
        requires(std::is_integral_v< OType > && !std::same_as< bool, OType >)
    auto Date::month() const -> OType {
        if constexpr (std::convertible_to< std::chrono::month, OType >) {
            return static_cast< OType >(m_date.month());
        }
        return static_cast< OType >(static_cast< uint32_t >(m_date.month()));
    }

    template < class OType >
        requires(std::is_integral_v< OType > && !std::same_as< bool, OType >)
    auto Date::year() const -> OType {
        if (std::numeric_limits< OType >::max() <= static_cast<int32_t>(m_date.year()) || std::numeric_limits< OType >::min() >= static_cast<int32_t>(m_date.year())) {
            throw std::range_error("Output type can not represent storable value");
        }
        if constexpr (std::convertible_to< std::chrono::year, OType >) {
            return static_cast< OType >(m_date.year());
        }
        return static_cast< OType >(static_cast< int32_t >(m_date.year()));
    }

    /**
     * \brief Operator !=
     * \param l const Date &
     * \param r const Date &
     * \return bool
     */
    auto operator!=(const Date& l, const Date& r) -> bool;
    /**
     * \brief Operator >
     * \param l const Date &
     * \param r const Date &
     * \return bool
     */
    auto operator>(const Date& l, const Date& r) -> bool;
    /**
     * \brief Operator <=
     * \param l const Date &
     * \param r const Date &
     * \return bool
     */
    auto operator<=(const Date& l, const Date& r) -> bool;
    /**
     * \brief Operator >=
     * \param l const Date &
     * \param r const Date &
     * \return bool
     */
    auto operator>=(const Date& l, const Date& r) -> bool;
    /**
     * \brief Adds value to currently stored date
     * \param p_date Date
     * \param p_value DateValue
     * \return Date
     */
    auto operator+(Date p_date, DateDuration p_value) -> Date;
    /**
     * \brief Subtracts value to currently stored date
     * \param p_date Date
     * \param p_value DateValue
     * \return Date
     */
    auto operator-(Date p_date, DateDuration p_value) -> Date;

    auto operator<<(std::ostream& out, const Date& date) -> std::ostream&;
}  //namespace mt::date

#if defined __cpp_lib_format
template <> struct std::formatter< mt::date::Date > : std::formatter< std::chrono::year_month_day > {
    auto format(const mt::date::Date date, std::format_context& context) const {
        return std::formatter< std::chrono::year_month_day >::format(date.date(), context);
    }
};
#endif
#endif  // DATE_HPP
