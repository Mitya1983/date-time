#ifndef TIME_HPP
#define TIME_HPP

#include "time_zones.hpp"

#include <string>
#include <iostream>
#include <chrono>
#include <variant>
#include <functional>

/**
 * \brief Namespace which includes time handlers
 */
namespace mt::time {

    /**
     * \brief Type alias used for Time modifications
     */
    using TimeDuration
        = std::variant< std::chrono::hours, std::chrono::minutes, std::chrono::seconds, std::chrono::milliseconds, std::chrono::microseconds, std::chrono::nanoseconds >;

    /**
     * \brief Class to handle time
     * \headerfile time.hpp
     */
    class Time {

        friend auto operator+(Time l, Time r) -> Time;
        friend auto operator+(Time p_time, TimeDuration p_value) -> Time;
        friend auto operator-(Time l, Time r) -> Time;
        friend auto operator-(Time p_time, TimeDuration p_value) -> Time;
        friend struct std::formatter< Time >;

    public:
        /**
         * \brief Default constructor.
         * Creates time based on UTC time zone
         */
        explicit Time();
        /**
         * \overload
         * \brief Overloaded constructor.
         * Creates time based on provided time zone
         * \param p_time_zone tristan::TimeZone
         */
        explicit Time(TimeZone p_time_zone);
        /**
         * \overload
         * \brief Overloaded constructor.
         * Creates time based on provided time zone
         * \param p_time_duration TimeValue
         */
        explicit Time(TimeDuration p_time_duration);
        explicit Time(std::chrono::time_point< std::chrono::system_clock > p_time_point);
        /**
         * \overload
         * \brief Overloaded constructor.
         * \param p_hours std::chrono::hours.
         * \param p_minutes std::chrono::minutes.
         * \param p_seconds std::chrono::seconds. Default is 0.
         * \param p_milliseconds std::chrono::milliseconds. Default is 0.
         * \param p_microseconds std::chrono::microseconds. Default is 0.
         * \param p_nanoseconds std::chrono::nanoseconds. Default is 0.
         */
        explicit Time(std::chrono::hours p_hours,
                      std::chrono::minutes p_minutes,
                      std::chrono::seconds p_seconds = std::chrono::seconds{0},
                      std::chrono::milliseconds p_milliseconds = std::chrono::milliseconds{0},
                      std::chrono::microseconds p_microseconds = std::chrono::microseconds{0},
                      std::chrono::nanoseconds p_nanoseconds = std::chrono::nanoseconds{0});
        /**
         * \overload
         * \brief Parses the string provided and create time object.
         * \param time std::string representing time in following <b>formats</b>:
         * \li [HH:MM].
         * \li [HH:MM+(-)HH].
         * \li [HH:MM:SS].
         * \li [HH:MM:SS+(-)HH].
         * \li [HH:MM:SS.mmm].
         * \li [HH:MM:SS.mmm+(-)HH].
         * \li [HH:MM:SS.mmm.mmm].
         * \li [HH:MM:SS.mmm.mmm+(-)HH].
         * \li [HH:MM:SS.mmm.mmm.nnn].
         * \li [HH:MM:SS.mmm.mmm.nnn+(-)HH].
         * \throws std::invali_argument, std::range_error.
         */
        explicit Time(const std::string& time);

        /**
         * \brief Copy constructor
         */
        Time(const Time&) = default;
        /**
         * \brief Move constructor
         */
        Time(Time&&) = default;
        /**
         * \brief Copy assignment operator
         * \return Time&
         */
        auto operator=(const Time&) -> Time& = default;
        /**
         * \brief Move assignment operator
         * \return Time&
         */
        auto operator=(Time&&) -> Time& = default;
        /**
         * \brief Operator ==
         * \param other const Time&
         * \return bool
         * \note Precision is taken into account. That is if comparable objects are having different precision - false is returned
         */
        auto operator==(const Time& other) const -> bool;
        /**
         * \brief Operator <
         * \param other const Time&
         * \return bool
         * \note Precision is taken into account. That is if comparable objects are having different precision - false is returned.
         */
        auto operator<(const Time& other) const -> bool;
        /**
         * \brief Operator +=
         * \param other const Time&
         */
        void operator+=(const Time& other);
        /**
         * \brief Operator +=
         * \param p_value const TimeValue
         */
        void operator+=(TimeDuration p_value);
        /**
         * \brief Operator -=
         * \param other const Time&
         */
        void operator-=(const Time& other);
        /**
         * \brief Operator -=
         * \param p_value const TimeValue&
         */
        void operator-=(TimeDuration p_value);
        /**
         * \brief Destructor
         */
        ~Time() = default;

        /**
         * \brief Sets timezone offset. Only ISO hour based offsets are considered.
         * \param p_offset TimeZone
         */
        [[maybe_unused]] void setOffset(TimeZone p_offset);
        /**
         * \brief Returns number of hours passed since day start.
         * \return std::chrono::hours
         */
        [[nodiscard]] auto hours() const -> std::chrono::hours;
        /**
         * \brief Returns number of minutes passed since hour start.
         * \return std::chrono::minutes
         */
        [[nodiscard]] auto minutes() const -> std::chrono::minutes;
        /**
         * \brief Returns
         * \return std::chrono::seconds
         */
        [[nodiscard]] auto seconds() const -> std::chrono::seconds;
        /**
         * \brief Returns number of milliseconds passed since second start.
         * \return std::chrono::milliseconds
         */
        [[nodiscard]] auto milliseconds() const -> std::chrono::milliseconds;
        /**
         * \brief Returns number of microseconds passed since millisecond start.
         * \return std::chrono::microseconds
         */
        [[nodiscard]] auto microseconds() const -> std::chrono::microseconds;
        /**
         * \brief Returns number of nanoseconds passed since microsecond start.
         * \return std::chrono::nanoseconds
         */
        [[nodiscard]] auto nanoseconds() const -> std::chrono::nanoseconds;

        // /**
        //  * \brief Returns precision of Time object.
        //  * \return Precision
        //  */
        // [[nodiscard]] auto precision() const -> Precision { return m_precision; }

        /**
         * \brief Returns current offset
         * \return
         */
        [[nodiscard]] auto offset() const -> mt::TimeZone { return m_offset; }

        /**
         * \brief Creates Time object which represents localtime.
         * \return Time.
         */
        [[nodiscard]] static auto localTime() -> Time;

        /**
         * \brief Generates string representation of time which is ISO standard representation in format represented below. Or by formatter provided.
         * \return std::string.
         * \par Default format:
         * \li [hours:minutes:seconds.milliseconds.microseconds.nanoseconds].
         */
        [[nodiscard]] auto toString(const std::function< std::string(const Time&) >& formatter = {}) const -> std::string;

    private:
        std::chrono::nanoseconds m_nanoseconds_since_day_start{};
        TimeZone m_offset{TimeZone::UTC};
    };

    /**
     * \brief Operator !=
     * \param l const Time&
     * \param r const Time&
     * \return bool
     */
    auto operator!=(const Time& l, const Time& r) -> bool;
    /**
     * \brief Operator >
     * \param l const Time&
     * \param r const Time&
     * \return bool
     */
    auto operator>(const Time& l, const Time& r) -> bool;
    /**
     * \brief Operator <=
     * \param l const Time&
     * \param r const Time&
     * \return bool
     */
    auto operator<=(const Time& l, const Time& r) -> bool;
    /**
     * \brief Operator >=
     * \param l const Time&
     * \param r const Time&
     * \return bool
     */
    auto operator>=(const Time& l, const Time& r) -> bool;

    /**
     * \brief Operator +
     * \param l Time
     * \param r Time
     * \return Time
     */
    auto operator+(Time l, Time r) -> Time;
    /**
     *
     * @param p_time Time
     * @param p_value TimeValue
     * @return Time
     */
    auto operator+(Time p_time, mt::time::TimeDuration p_value) -> Time;
    /**
     * \brief Operator -
     * \param l Time
     * \param r Time
     * \return Time
     */
    auto operator-(Time l, Time r) -> Time;
    /**
     * \brief Operator <<
     * \param out std::ostream&
     * \param time const Time&
     * \return std::ostream&
     * \note Method toString() is used here
     */

    auto operator<<(std::ostream& out, const Time& time) -> std::ostream&;
}  // namespace mt::time

#if defined __cpp_lib_format
template <> struct std::formatter< mt::TimeZone > : std::formatter< std::string > {
    auto format(const mt::TimeZone time_zone, std::format_context& context) const {
        std::string result;
        if (time_zone == mt::TimeZone::UTC) {
            result += 'Z';
        } else {
            time_zone > mt::TimeZone::UTC ? result += '+' : result += '-';
            if (time_zone > mt::TimeZone::WEST_10 && time_zone < mt::TimeZone::EAST_10) {
                result += "0";
            }
            result += std::to_string(std::abs(static_cast< int8_t >(time_zone)));
            result += ":00";
        }
        return std::formatter< std::string >::format(result, context);
    }
};

template <> struct std::formatter< mt::time::Time > : std::formatter< std::chrono::hh_mm_ss< std::chrono::nanoseconds > > {
    auto format(const mt::time::Time time, std::format_context& context) const {
        return std::formatter< std::chrono::hh_mm_ss< std::chrono::nanoseconds > >::format(std::chrono::hh_mm_ss<std::chrono::nanoseconds>{time.m_nanoseconds_since_day_start}, context);
    }
};
#endif
#endif  // TIME_HPP
