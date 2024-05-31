#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP
#include "date.hpp"
#include "time.hpp"

/**
 * \brief Namespace which unites date and time in one DateTime object
 */
namespace tristan::date_time {

    class DateTime;

    /**
     * \brief Class to store date and day time
     * \headerfile date_time.hpp
     */
    class DateTime {
    public:
        /**
         * \brief Default constructor.
         * Creates DateTime based on UTC time zone
         * \param p_precision tristan::time::Precision. Default is set to tristan::time::Precision::SECONDS
         */
        explicit DateTime(tristan::time::Precision p_precision = tristan::time::Precision::SECONDS);
        /**
         * \brief Default constructor.
         * Creates DateTime based on provided time zone
         * \param p_time_zone tristan::TimeZone
         * \param p_precision tristan::time::Precision. Default is set to tristan::time::Precision::SECONDS
         */
        explicit DateTime(tristan::TimeZone p_time_zone, tristan::time::Precision p_precision = tristan::time::Precision::SECONDS);
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
         * \brief Adds seconds to Time object.
         * \note Precision is taken into account. That is in case of MINUTES precision if number of seconds is less then 1 minute operation is meaningless and will not have any effect.
         * \param p_seconds uint64_t.
         */
        void addSeconds(uint64_t p_seconds);
        /**
         * \brief Adds minutes.
         * \param p_minutes uint64_t
         */
        void addMinutes(uint64_t p_minutes);
        /**
         * \brief Adds hours. This is a convenience function which converts hours to minutes and invokes addMinutes(uint64_t minutes) function.
         * \param p_hours uint64_t
         */
        void addHours(uint64_t p_hours);
        /**
         * \brief Adds days
         * \param p_days uint64_t
         */
        void addDays(uint64_t p_days);
        /**
         * \brief Adds months
         * \param p_months uint64_t
         */
        void addMonths(uint64_t p_months);
        /**
         * \brief Adds years
         * \param p_years uint64_t
         */
        void addYears(uint64_t p_years);
        /**
         * \brief subtracts seconds from Time object.
         * \note Precision is taken into account. That is in case of MINUTES precision if number of seconds is less then 1 minute operation is meaningless and will not have any effect.
         * \param p_seconds uint64_t.
         */
        void subtractSeconds(uint64_t p_seconds);
        /**
         * \brief subtracts minutes from Time object.
         * \param p_minutes uint64_t.
         */
        void subtractMinutes(uint64_t p_minutes);
        /**
         * \brief subtracts hours from Time object. This is a convenience function which converts hours to minutes and invokes subtractMinutes(uint64_t minutes) function
         * \param p_hours uint64_t.
         */
        void subtractHours(uint64_t p_hours);
        /**
         * \brief Subtracts days
         * \param p_days uint64_t
         */
        void subtractDays(uint64_t p_days);
        /**
         * \brief Subtracts months
         * \param p_months uint64_t
         */
        void subtractMonths(uint64_t p_months);
        /**
         * \brief Subtracts years
         * \param p_years uint64_t
         */
        void subtractYears(uint64_t p_years);
        /**
         * \brief Returns date
         * \return const date::Date&
         */
        [[nodiscard]] auto date() const -> const date::Date&;
        /**
         * \brief Returns time
         * \return const time::Time&
         */
        [[nodiscard]] auto time() const -> const time::Time&;

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

    protected:
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

}  // namespace tristan::date_time

#endif  // DATE_TIME_HPP
