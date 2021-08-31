#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP
#include "date.hpp"
#include "time.hpp"

namespace tristan::date_time{
/// \brief Class to store date and day time according
/// \headerfile date_time.hpp
    class Date_Time
    {
    public:
        /// \brief Default constructor. Invokes default constructors for date and daytime objects.
        Date_Time();
        /// \brief Copy constructor
        Date_Time(const Date_Time&) = default;
        /// \brief Move constructor
        Date_Time(Date_Time&&) = default;
        
        /// \brief Copy assignment operator
        Date_Time& operator=(const Date_Time&) = default;
        /// \brief Move assignment operator
        Date_Time& operator=(Date_Time&&) = default;
        
        /// \brief Operator equal.
        auto operator==(const Date_Time& other) const -> bool;
        /// \brief Operator less then.
        auto operator<(const Date_Time& other) const -> bool;
        
        ~Date_Time() = default;
        /// \brief Setter with copy assignment
        void setDate(const date::Date& date) {m_date = date;}
        /// \brief Setter with move assignment
        void setDate(date::Date&& date) {m_date = date;}
        /// \brief Setter with copy assignment
        void setTime(const time::DayTime& time) {m_day_time = time;}
        /// \brief Setter with move assignment
        void setTime(time::DayTime&& time) {m_day_time = time;}
        
        /// \brief Getter for date
        /// \return const date::Date&
        [[nodiscard]] auto date() const -> const date::Date& {return m_date;}
        /// \brief Getter for time
        /// \return const time::DayTime&
        [[nodiscard]] auto time() const -> const time::DayTime& {return m_day_time;}
        /// \brief Generates string representation of time. Returns [Date::toString][T][DayTime::toString]
        /// \return std::string
        [[nodiscard]] virtual auto toString() const -> std::string;
    
    protected:
    
    private:
        date::Date m_date;
        time::DayTime m_day_time;
    };
    
    /// \brief Operator not equal to.
    auto operator != (const Date_Time &l, const Date_Time &r) -> bool;
    /// \brief Operator greater than.
    auto operator > (const Date_Time &l, const Date_Time &r) -> bool;
    /// \brief Operator less than or equal to.
    auto operator <= (const Date_Time &l, const Date_Time &r) -> bool;
    /// \brief Operator greater than or equal to.
    auto operator >= (const Date_Time &l, const Date_Time &r) -> bool;
    /// \brief std::ostream operator.  Date_Time::toString() is used.
    auto operator<<(std::ostream &out, const Date_Time &dt) -> std::ostream&;
    
}// namespace tristan::date_time

#endif // DATE_TIME_HPP
