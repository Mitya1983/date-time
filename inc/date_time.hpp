#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP
#include "date.hpp"
#include "time.hpp"
/// \brief Namespace which unites date and time in one DateTime object
namespace tristan::date_time{
/// \brief Class to store date and day time
/// \headerfile date_time.hpp
    class DateTime
    {
    public:
        /// \brief Default constructor. Invokes default constructors for date and daytime objects.
        explicit DateTime(tristan::time::Precision p_precision = tristan::time::Precision::SECONDS);
        /**
         * \brief String constructor.
         * \param date_time Date and time string representation
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
        explicit DateTime(const std::string& date_time);
        /// \brief Copy constructor
        DateTime(const DateTime&) = default;
        /// \brief Move constructor
        DateTime(DateTime&&) = default;
        
        /// \brief Copy assignment operator
        DateTime& operator=(const DateTime&) = default;
        /// \brief Move assignment operator
        DateTime& operator=(DateTime&&) = default;
        
        /// \brief Operator equal.
        auto operator==(const DateTime& other) const -> bool;
        /// \brief Operator less then.
        auto operator<(const DateTime& other) const -> bool;
        /// \brief Destructor
        ~DateTime() = default;
        /// \brief Setter with copy assignment
        void setDate(const date::Date& date) {m_date = date;}
        /// \brief Setter with move assignment
        void setDate(date::Date&& date) {m_date = date;}
        /// \brief Setter with copy assignment
        void setTime(const time::Time& time) { m_time = time;}
        /// \brief Setter with move assignment
        void setTime(time::Time&& time) { m_time = time;}
        
        /// \brief Getter for date
        /// \return const date::Date&
        [[nodiscard]] auto date() const -> const date::Date& {return m_date;}
        /// \brief Getter for time
        /// \return const time::Time&
        [[nodiscard]] auto time() const -> const time::Time& {return m_time;}
        /// \brief Generates string representation of time. Returns [Date::toString][T][Time::toString]
        /// \return std::string
        [[nodiscard]] virtual auto toString() const -> std::string;
    
    protected:
    
    private:
        date::Date m_date;
        time::Time m_time;
    };
    
    /// \brief Operator not equal to.
    auto operator != (const DateTime &l, const DateTime &r) -> bool;
    /// \brief Operator greater than.
    auto operator > (const DateTime &l, const DateTime &r) -> bool;
    /// \brief Operator less than or equal to.
    auto operator <= (const DateTime &l, const DateTime &r) -> bool;
    /// \brief Operator greater than or equal to.
    auto operator >= (const DateTime &l, const DateTime &r) -> bool;
    /// \brief std::ostream operator.  DateTime::toString() is used.
    auto operator<<(std::ostream &out, const DateTime &dt) -> std::ostream&;
    
}// namespace tristan::date_time

#endif // DATE_TIME_HPP
