#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP
#include "date.hpp"
#include "time.hpp"

/**
 * \brief Namespace which unites date and time in one DateTime object
 */
namespace mt::date_time {

    class date_time;

    /**
     * \brief Class to store date and day time
     * \headerfile date_time.hpp
     */
    class date_time {
        friend auto operator+(const date_time& l, mt::time::time_duration) -> date_time;
        friend auto operator+(const date_time& l, mt::date::date_duration) -> date_time;
        friend auto operator-(const date_time& l, mt::time::time_duration) -> date_time;
        friend auto operator-(const date_time& l, mt::date::date_duration) -> date_time;
    public:
        /**
         * \brief Default constructor.
         * Creates DateTime based on UTC time zone
         */
        date_time() = default;
        /**
         * \brief TimeZone constructor.
         * Creates DateTime based on provided time zone
         * \param p_time_point std::chrono::time_point< std::chrono::system_clock >
         */
        explicit date_time(std::chrono::time_point< std::chrono::system_clock > p_time_point);
        /**
         * \brief TimeZone constructor.
         * Creates DateTime based on provided time zone
         * \param p_time_zone mt::TimeZone
         */
        explicit date_time(mt::TimeZone p_time_zone);
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
        explicit date_time(const std::string& p_date_time);
        /**
         * \brief Copy constructor
         */
        date_time(const date_time&) = default;
        /**
         * \brief Move constructor
         */
        date_time(date_time&&) = default;
        /**
         * \brief Copy assignment operator
         * \return DateTime&
         */
        auto operator=(const date_time&) -> date_time& = default;
        /**
         * \brief Move assignment operator
         * \return DateTime&
         */
        auto operator=(date_time&&) -> date_time& = default;
        /**
         * \brief Operator ==
         * \param other const DateTime&
         * \return bool
         */
        auto operator==(const date_time& other) const -> bool;
        /**
         * \brief Operator <
         * \param other const DateTime&
         * \return bool
         */
        auto operator<(const date_time& other) const -> bool;
        void operator+=(mt::time::time_duration);
        void operator+=(mt::date::date_duration);
        void operator-=(mt::time::time_duration);
        void operator-=(mt::date::date_duration);
        /**
         * \brief Destructor
         */
        ~date_time() = default;
        /**
         * \brief Copy assignment setter
         * \param p_date const date::Date&
         */
        void set_date(const date::date& p_date);
        /**
         * \brief Move assignment setter
         * \param p_date date::Date&&
         */
        void set_date(date::date&& p_date);
        /**
         * \brief Copy assignment setter
         * \param p_time const time::Time&
         */
        void set_time(const time::time& p_time);
        /**
         * \brief Move assignment setter
         * \param p_time time::Time&&
         */
        void set_time(time::time&& p_time);
        /**
         * \brief Returns date
         * \return const date::Date&
         */
        [[nodiscard]] auto date() const -> const date::date&;
        /**
         * \brief Returns date
         * \return date::Date&
         */
        [[nodiscard]] auto date() -> date::date&;
        /**
         * \brief Returns time
         * \return const time::Time&
         */
        [[nodiscard]] auto time() const -> const time::time&;
        /**
         * \brief Returns time
         * \return time::Time&
         */
        [[nodiscard]] auto time() -> time::time&;

        /**
         * \brief Generates string representation of date and time which is ISO standard representation. Or by formatter provided.
         * \param formatter const std::function< std::string(const DateTime&) >
         * \return std::string
         */
        [[nodiscard]] auto to_string(const std::function< std::string(const date_time&) >& formatter = {}) const -> std::string;

        /**
         * \brief Creates Date object which represents local date.
         * \return DateTime.
         */
        [[nodiscard]] static auto local_date_time() -> date_time;

    private:
        date::date m_date;
        time::time m_time;
    };

    /**
     * \brief Operator !=
     * \param l const DateTime &
     * \param r const DateTime &
     * \return bool
     */
    auto operator!=(const date_time& l, const date_time& r) -> bool;
    /**
     * \brief Operator >
     * \param l const DateTime &
     * \param r const DateTime &
     * \return bool
     */
    auto operator>(const date_time& l, const date_time& r) -> bool;
    /**
     * \brief Operator <=
     * \param l const DateTime &
     * \param r const DateTime &
     * \return bool
     */
    auto operator<=(const date_time& l, const date_time& r) -> bool;
    /**
     * \brief Operator >=
     * \param l const DateTime &
     * \param r const DateTime &
     * \return bool
     */
    auto operator>=(const date_time& l, const date_time& r) -> bool;
    /**
     * \brief Operator <<
     * \param out std::ostream&
     * \param dt const DateTime&
     * \return std::ostream&
     * \note Method toString() is used here
     */
    auto operator<<(std::ostream& out, const date_time& dt) -> std::ostream&;

    auto operator+(const date_time& l, mt::time::time_duration) -> date_time;
    auto operator+(const date_time& l, mt::date::date_duration) -> date_time;
    auto operator-(const date_time& l, mt::time::time_duration) -> date_time;
    auto operator-(const date_time& l, mt::date::date_duration) -> date_time;

}  // namespace tristan::date_time

#endif  // DATE_TIME_HPP
