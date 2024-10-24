#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP
#include "date.hpp"
#include "time.hpp"

/**
 * \brief Namespace which unites date and time in one DateTime object
 */
namespace mt::date_time {

    class DateTime;

    /**
     * \brief Class to store date and day time
     * \headerfile date_time.hpp
     */
    class DateTime {
        friend auto operator+(const DateTime& l, mt::time::TimeDuration) -> DateTime;
        friend auto operator+(const DateTime& l, mt::date::DateDuration) -> DateTime;
        friend auto operator-(const DateTime& l, mt::time::TimeDuration) -> DateTime;
        friend auto operator-(const DateTime& l, mt::date::DateDuration) -> DateTime;
    public:
        /**
         * \brief Default constructor.
         * Creates DateTime based on UTC time zone
         */
        DateTime() = default;
        /**
         * \brief Default constructor.
         * Creates DateTime based on provided time zone
         * \param p_time_zone tristan::TimeZone
         */
        explicit DateTime(mt::TimeZone p_time_zone);
        /**
         * \brief String constructor.
         * \param p_date_time Date and time string representation
         * \li [YYYYMMDDTHH:MM:SS]
         * \li [YYYY-MM-DDTHH:MM:SS]
         * \li [YYYYMMDDTHH:MM:SS+(-)HH]
         * \li [YYYY-MM-DDTHH:MM:SS+(-)HH]
         * \li [YYYYMMDDTHH:MM:SS.mmm]
         * \li [YYYY-MM-DDTHH:MM:SS.mmm]
         * \li [YYYYMMDDTHH:MM:SS.mmm+(-)HH]
         * \li [YYYY-MM-DDTHH:MM:SS.mmm+(-)HH]
         * \li [YYYYMMDDTHH:MM:SS.mmm.mmm]
         * \li [YYYY-MM-DDTHH:MM:SS.mmm.mmm]
         * \li [YYYYMMDDTHH:MM:SS.mmm.mmm+(-)HH]
         * \li [YYYY-MM-DDTHH:MM:SS.mmm.mmm+(-)HH]
         * \li [YYYYMMDDTHH:MM:SS.mmm.mmm.nnn]
         * \li [YYYY-MM-DDTHH:MM:SS.mmm.mmm.nnn]
         * \li [YYYYMMDDTHH:MM:SS.mmm.mmm.nnn+(-)HH]
         * \li [YYYY-MM-DDTHH:MM:SS.mmm.mmm.nnn+(-)HH]
         */
        explicit DateTime(const std::string& p_date_time);
        /**
         * \brief Copy constructor
         */
        DateTime(const DateTime&) = default;
        /**
         * \brief Move constructor
         */
        DateTime(DateTime&&) = default;
        /**
         * \brief Copy assignment operator
         * \return DateTime&
         */
        auto operator=(const DateTime&) -> DateTime& = default;
        /**
         * \brief Move assignment operator
         * \return DateTime&
         */
        auto operator=(DateTime&&) -> DateTime& = default;
        /**
         * \brief Operator ==
         * \param other const DateTime&
         * \return bool
         */
        auto operator==(const DateTime& other) const -> bool;
        /**
         * \brief Operator <
         * \param other const DateTime&
         * \return bool
         */
        auto operator<(const DateTime& other) const -> bool;
        void operator+=(mt::time::TimeDuration);
        void operator+=(mt::date::DateDuration);
        void operator-=(mt::time::TimeDuration);
        void operator-=(mt::date::DateDuration);
        /**
         * \brief Destructor
         */
        ~DateTime() = default;
        /**
         * \brief Copy assignment setter
         * \param p_date const date::Date&
         */
        void setDate(const date::Date& p_date);
        /**
         * \brief Move assignment setter
         * \param p_date date::Date&&
         */
        void setDate(date::Date&& p_date);
        /**
         * \brief Copy assignment setter
         * \param p_time const time::Time&
         */
        void setTime(const time::Time& p_time);
        /**
         * \brief Move assignment setter
         * \param p_time time::Time&&
         */
        void setTime(time::Time&& p_time);
        /**
         * \brief Returns date
         * \return const date::Date&
         */
        [[nodiscard]] auto date() const -> const date::Date&;
        /**
         * \brief Returns date
         * \return date::Date&
         */
        [[nodiscard]] auto date() -> date::Date&;
        /**
         * \brief Returns time
         * \return const time::Time&
         */
        [[nodiscard]] auto time() const -> const time::Time&;
        /**
         * \brief Returns time
         * \return time::Time&
         */
        [[nodiscard]] auto time() -> time::Time&;

        /**
         * \brief Generates string representation of date and time which is ISO standard representation. Or by formatter provided.
         * \param formatter const std::function< std::string(const DateTime&) >
         * \return std::string
         */
        [[nodiscard]] auto toString(const std::function< std::string(const DateTime&) >& formatter = {}) const -> std::string;

        /**
         * \brief Creates Date object which represents local date.
         * \return DateTime.
         */
        [[nodiscard]] static auto localDateTime() -> DateTime;

    private:
        date::Date m_date;
        time::Time m_time;
    };

    /**
     * \brief Operator !=
     * \param l const DateTime &
     * \param r const DateTime &
     * \return bool
     */
    auto operator!=(const DateTime& l, const DateTime& r) -> bool;
    /**
     * \brief Operator >
     * \param l const DateTime &
     * \param r const DateTime &
     * \return bool
     */
    auto operator>(const DateTime& l, const DateTime& r) -> bool;
    /**
     * \brief Operator <=
     * \param l const DateTime &
     * \param r const DateTime &
     * \return bool
     */
    auto operator<=(const DateTime& l, const DateTime& r) -> bool;
    /**
     * \brief Operator >=
     * \param l const DateTime &
     * \param r const DateTime &
     * \return bool
     */
    auto operator>=(const DateTime& l, const DateTime& r) -> bool;
    /**
     * \brief Operator <<
     * \param out std::ostream&
     * \param dt const DateTime&
     * \return std::ostream&
     * \note Method toString() is used here
     */
    auto operator<<(std::ostream& out, const DateTime& dt) -> std::ostream&;

    auto operator+(const DateTime& l, mt::time::TimeDuration) -> DateTime;
    auto operator+(const DateTime& l, mt::date::DateDuration) -> DateTime;
    auto operator-(const DateTime& l, mt::time::TimeDuration) -> DateTime;
    auto operator-(const DateTime& l, mt::date::DateDuration) -> DateTime;

}  // namespace tristan::date_time

#endif  // DATE_TIME_HPP
