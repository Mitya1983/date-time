#ifndef TIME_HPP
#define TIME_HPP
#include <string>
#include <iostream>
#include <chrono>
#include <variant>

/// \brief Namespace which includes time handlers
namespace tristan::time {
    
    /// \brief Enum which represents precisions used in implementation.
    enum class Precision : uint8_t {
        MINUTES,
        SECONDS,
        MILLISECONDS,
        MICROSECONDS,
        NANOSECONDS
    };
    /// \brief enum which represents possible time zone offsets.
    enum class TimeZone : int8_t {
        WEST_12 [[maybe_unused]],
        WEST_11 [[maybe_unused]],
        WEST_10 [[maybe_unused]],
        WEST_9 [[maybe_unused]],
        WEST_8 [[maybe_unused]],
        WEST_7 [[maybe_unused]],
        WEST_6 [[maybe_unused]],
        WEST_5 [[maybe_unused]],
        WEST_4 [[maybe_unused]],
        WEST_3 [[maybe_unused]],
        WEST_2 [[maybe_unused]],
        WEST_1 [[maybe_unused]],
        UTC = 0,
        EAST_1 [[maybe_unused]],
        EAST_2 [[maybe_unused]],
        EAST_3 [[maybe_unused]],
        EAST_4 [[maybe_unused]],
        EAST_5 [[maybe_unused]],
        EAST_6 [[maybe_unused]],
        EAST_7 [[maybe_unused]],
        EAST_8 [[maybe_unused]],
        EAST_9 [[maybe_unused]],
        EAST_10 [[maybe_unused]],
        EAST_11 [[maybe_unused]],
        EAST_12 [[maybe_unused]],
    };
    /// \brief Class to handle time
    /// \headerfile time.hpp
class DayTime
{
    friend auto operator + (const DayTime& l, const DayTime &r) -> DayTime;
    friend auto operator - (const DayTime& l, const DayTime &r) -> DayTime;
    
public:
    /// \brief Creates DayTime object from system clock. UTC time zone is set.
    /// \param precision Precision.
    explicit DayTime(Precision precision = Precision::SECONDS);
    /// \brief Minutes precision constructor.
    /// \param hours uint8_t.
    /// \param minutes uint8_t.
    /// \throws std::range_error.
    explicit DayTime(uint8_t hours, uint8_t minutes);
    /// \brief Seconds precision constructor. UTC time zone is set.
    /// \param hours uint8_t.
    /// \param minutes uint8_t.
    /// \param seconds uint8_t.
    /// \throws std::range_error.
    explicit DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
    /// \brief Milliseconds precision constructor. UTC time zone is set.
    /// \param hours uint8_t.
    /// \param minutes uint8_t.
    /// \param seconds uint8_t.
    /// \param milliseconds uint16_t.
    /// \throws std::range_error.
    explicit DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds);
    /// \brief Microseconds precision constructor. UTC time zone is set.
    /// \param hours uint8_t.
    /// \param minutes uint8_t.
    /// \param seconds uint8_t.
    /// \param milliseconds uint16_t.
    /// \param microseconds uint16_t.
    /// \throws std::range_error.
    explicit DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds);
    /// \brief Nanoseconds precision constructor. UTC time zone is set.
    /// \param hours uint8_t.
    /// \param minutes uint8_t.
    /// \param seconds uint8_t.
    /// \param milliseconds uint16_t.
    /// \param microseconds uint16_t.
    /// \param nanoseconds uint16_t.
    /// \throws std::range_error.
    explicit DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds, uint16_t nanoseconds);
    /// \brief Parses the string provided and create time object.
    /// \param time std::string representing time in following formats.
    /// \format[hours, minutes, seconds] - two digits.
    /// \format[milliseconds, microseconds and nanoseconds] - three digits.
    /// \format[hours:minutes] - Minutes precision.
    /// \format[hours:minutes:seconds] - Seconds precision.
    /// \format[hours:minutes:seconds.milliseconds] - Milliseconds precision.
    /// \format[hours:minutes:seconds.milliseconds.microseconds] - Microseconds precision.
    /// \format[hours:minutes:seconds.milliseconds.microseconds.nanoseconds] - Nanoseconds precision.
    /// \throws std::invali_argument, std::range_error.
    explicit DayTime(const std::string &time);
    /// \brief Copy constructor
    DayTime(const DayTime&) = default;
    /// \brief Move constructor
    DayTime(DayTime&&) = default;
    /// \brief Copy assignment operator
    auto operator=(const DayTime&) -> DayTime& = default;
    /// \brief Move assignment operator
    auto operator=(DayTime&&) -> DayTime& = default;
    /// \brief Checks if DayTimes objects are equal.
    /// \note Precision is taken into account. That is if comparable objects are having different precision - false is returned.
    /// \param r const DayTime&.
    /// \return true if objects are equal and false otherwise.
    auto operator==(const DayTime& r) const -> bool;
    /// \brief Checks if one DayTime object is less then other one.
    /// \note Precision is taken into account. That is if comparable objects are having different precision - false is returned.
    /// \param r const DayTime&.
    /// \return true if left object is less and false otherwise.
    auto operator<(const DayTime& r) const -> bool;
    /// \brief Unary plus operator.
    void operator += (const DayTime& r);
    /// \brief Unary minus operator.
    void operator -= (const DayTime& r);
    /// \brief Destructor
    virtual ~DayTime() = default;
    
    /// \brief Sets timezone offset. Only ISO hour based offsets are considered.
    /// \param offset TimeZone.
    [[maybe_unused]] void setOffset(TimeZone offset) {m_offset = offset;}
    
    /// \brief Adds hours to DayTime object. This is a convenience function which converts hours to minutes and invokes addMinutes(uint64_t minutes) function.
    /// \param hours uint64_t.
    void addHours(uint64_t hours);
    /// \brief Adds minutes to DayTime object.
    /// \param hours uint64_t.
    void addMinutes(uint64_t minutes);
    /// \brief Adds seconds to DayTime object.
    /// \note Precision is taken into account. That is in case of MINUTES precision if number of seconds is less then 1 minute operation is meaningless and will not have any effect.
    /// \param hours uint64_t.
    void addSeconds(uint64_t seconds);
    /// \brief Adds milliseconds to DayTime object.
    /// \note Precision is taken into account. That is in case of SECONDS or MINUTES precision if number of milliseconds is less then 1 second operation is meaningless and will not have any effect.
    /// \param hours uint64_t.
    void addMilliseconds(uint64_t milliseconds);
    /// \brief Adds microseconds to DayTime object.
    /// \note Precision is taken into account. That is in case of SECONDS, MINUTES or MILLISECONDS precision if number of microseconds is less then 1 millisecond operation is meaningless and will not have any effect.
    /// \param hours uint64_t.
    void addMicroseconds(uint64_t microseconds);
    /// \brief Adds nanoseconds to DayTime object.
    /// \note Precision is taken into account. That is in case of SECONDS, MINUTES, MILLISECONDS or MICROSECONDS precision if number of nanosecond is less then 1 microsecond operation is meaningless and will not have any effect.
    /// \param hours uint64_t.
    void addNanoseconds(uint64_t nanoseconds);
    /// \brief subtracts hours from DayTime object. This is a convenience function which converts hours to minutes and invokes subtractMinutes(uint64_t minutes) function
    /// \param hours uint64_t.
    void subtractHours(uint64_t hours);
    /// \brief subtracts minutes from DayTime object.
    /// \param hours uint64_t.
    void subtractMinutes(uint64_t minutes);
    /// \brief subtracts seconds from DayTime object.
    /// \note Precision is taken into account. That is in case of MINUTES precision if number of seconds is less then 1 minute operation is meaningless and will not have any effect.
    /// \param hours uint64_t.
    void subtractSeconds(uint64_t seconds);
    /// \brief subtracts milliseconds from DayTime object.
    /// \note Precision is taken into account. That is in case of SECONDS or MINUTES precision if number of milliseconds is less then 1 second operation is meaningless and will not have any effect.
    /// \param hours uint64_t.
    void subtractMilliseconds(uint64_t milliseconds);
    /// \brief subtracts microseconds from DayTime object.
    /// \note Precision is taken into account. That is in case of SECONDS, MINUTES, MILLISECONDS or MICROSECONDS precision if number of nanosecond is less then 1 microsecond operation is meaningless and will not have any effect.
    /// \param hours uint64_t.
    void subtractMicroseconds(uint64_t microseconds);
    /// \brief subtracts nanoseconds from DayTime object.
    /// \note Precision is taken into account. That is in case of SECONDS, MINUTES, MILLISECONDS or MICROSECONDS precision if number of nanosecond is less then 1 microsecond operation is meaningless and will not have any effect.
    /// \param hours uint64_t.
    void subtractNanoseconds(uint64_t nanoseconds);
    
    /// \brief Returns number of hours passed since day start.
    /// \return uint8_t.
    [[nodiscard]] auto hours() const  -> uint8_t;
    /// \brief Returns number of minutes passed since hour start.
    /// \return uint8_t.
    [[nodiscard]] auto minutes() const -> uint8_t;
    /// \brief Returns number of seconds passed since minute start.
    /// \return uint8_t.
    [[nodiscard]] auto seconds() const -> uint8_t;
    /// \brief Returns number of milliseconds passed since second start.
    /// \return uint16_t.
    [[nodiscard]] auto milliseconds() const -> uint16_t;
    /// \brief Returns number of microseconds passed since millisecond start.
    /// \return uint16_t.
    [[nodiscard]] auto microseconds() const -> uint16_t;
    /// \brief Returns number of nanoseconds passed since microsecond start.
    /// \return uint16_t.
    [[nodiscard]] auto nanoseconds() const -> uint16_t;
    
    /// \brief Returns precision of DayTime object.
    /// \return Precision.
    [[nodiscard]] auto precision() const -> Precision {return m_precision;}
    
    /// \brief Creates DayTime object which represents localtime.
    /// \param precision Precision::SECONDS.
    /// \return DayTime.
    [[nodiscard]] static auto localTime(Precision precision = Precision::SECONDS) -> DayTime;
    /// \brief Generates string representation of time. By default return ISO standard representation in formats represented below. If show_precision is set to true each format will be suffixed by offset in form of +(-)hh.
    /// \return std::string to represent the time.
    /// \format[hours:minutes] - Minutes precision.
    /// \format[hours:minutes:seconds] - Seconds precision.
    /// \format[hours:minutes:seconds.milliseconds] - Milliseconds precision.
    /// \format[hours:minutes:seconds.milliseconds.microseconds] - Microseconds precision.
    /// \format[hours:minutes:seconds.milliseconds.microseconds.nanoseconds] - Nanoseconds precision.
    [[nodiscard]] virtual auto toString(bool show_offset) const -> std::string;

protected:

private:
    std::variant<std::chrono::minutes, std::chrono::seconds, std::chrono::milliseconds, std::chrono::microseconds, std::chrono::nanoseconds> m_time_since_day_start;
    
    TimeZone m_offset;
    
    Precision m_precision;
    
    void _addMinutes(uint64_t minutes);
    void _addSeconds(uint64_t seconds);
    void _addMilliseconds(uint64_t milliseconds);
    void _addMicroseconds(uint64_t microseconds);
    void _addNanoseconds(uint64_t nanoseconds);
    void _subtractMinutes(uint64_t minutes);
    void _subtractSeconds(uint64_t seconds);
    void _subtractMilliseconds(uint64_t milliseconds);
    void _subtractMicroseconds(uint64_t microseconds);
    void _subtractNanoseconds(uint64_t nanoseconds);
};
    /// \brief Operator not equal to.
    auto operator != (const DayTime& l, const DayTime& r) -> bool;
    /// \brief Operator greater than.
    auto operator > (const DayTime& l, const DayTime& r) -> bool;
    /// \brief Operator less than or equal to.
    auto operator <= (const DayTime& l, const DayTime& r) -> bool;
    /// \brief Operator greater than or equal to.
    auto operator >= (const DayTime& l, const DayTime& r) -> bool;
    
    /// \brief Addition operator.
    auto operator + (const DayTime& l, const DayTime &r) -> DayTime;
    /// \brief Subtraction operator.
    auto operator - (const DayTime& l, const DayTime &r) -> DayTime;
    /// \brief std::ostream operator.  Date::toString() is used.
    auto operator<<(std::ostream &out, const DayTime &time) -> std::ostream&;
}//namespace tristan::time

#endif // TIME_HPP
