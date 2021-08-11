#ifndef MT_DATE_TIME_HPP
#define MT_DATE_TIME_HPP
#include "date.hpp"
#include "time.hpp"

namespace MT{

namespace date_time {

class Date_Time : public date::Date, public time::Time
{
private:

protected:

public:
    //CONSTRUCTORS
    Date_Time();
    Date_Time(const Date_Time&) = default;
    Date_Time(Date_Time&&) = default;
    //OPERATORS
    Date_Time& operator=(const Date_Time&) = default;
    Date_Time& operator=(Date_Time&&) = default;
    //SETTERS

    //GETTERS

    //FUNCTIONS
    std::string to_string() const override;
    //DESTRUCTOR
    ~Date_Time() = default;
};

std::ostream& operator<<(std::ostream &out, const Date_Time &dt);

} // namespace date_time
}// namespace MT

#endif // MT_DATE_TIME_HPP
