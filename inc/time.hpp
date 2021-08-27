#ifndef TIME_HPP
#define TIME_HPP
#include <string>
#include <iostream>
#include <chrono>
#include <variant>


namespace tristan::time {
    
    enum class Precision : uint8_t {
        MINUTES,
        SECONDS,
        MILLISECONDS,
        MICROSECONDS,
        NANOSECONDS
    };
    
    enum class TimeZone : int8_t {
        WEST_12,
        WEST_11,
        WEST_10,
        WEST_9,
        WEST_8,
        WEST_7,
        WEST_6,
        WEST_5,
        WEST_4,
        WEST_3,
        WEST_2,
        WEST_1,
        UTC = 0,
        EAST_1,
        EAST_2,
        EAST_3,
        EAST_4,
        EAST_5,
        EAST_6,
        EAST_7,
        EAST_8,
        EAST_9,
        EAST_10,
        EAST_11,
        EAST_12,
    };
    
class DayTime
{
    friend auto operator + (const DayTime& l, const DayTime &r) -> DayTime;
    friend auto operator - (const DayTime& l, const DayTime &r) -> DayTime;
    
public:
    
    explicit DayTime(Precision precision = Precision::SECONDS);
    /// \brief Minutes precision constructor
    /// \param hours
    /// \param minutes
    /// \throws std::range_error
    explicit DayTime(uint8_t hours, uint8_t minutes);
    /// \brief Seconds precision constructor
    /// \param hours
    /// \param minutes
    /// \throws std::range_error
    explicit DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
    /// \brief Milliseconds precision constructor
    /// \param hours
    /// \param minutes
    /// \throws std::range_error
    explicit DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds);
    /// \brief Microseconds precision constructor
    /// \param hours
    /// \param minutes
    /// \throws std::range_error
    explicit DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds);
    /// \brief Nanoseconds precision constructor
    /// \param hours
    /// \param minutes
    /// \throws std::range_error
    explicit DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds, uint16_t nanoseconds);
    
    /// \brief Parses the string provided and create time object
    /// \param time std::string representing time in following formats
    /// \formats[hours, minutes, seconds] - two digits
    /// \formats[milliseconds, microseconds and nanoseconds] - three digits
    /// \formats[hours:minutes] - Minutes precision
    /// \formats[hours:minutes:seconds] - Seconds precision
    /// \formats[hours:minutes:seconds.milliseconds] - Milliseconds precision
    /// \formats[hours:minutes:seconds.milliseconds.microseconds] - Microseconds precision
    /// \formats[hours:minutes:seconds.milliseconds.microseconds.nanoseconds] - Nanoseconds precision
    /// \throws std::invali_argument, std::range_error
    explicit DayTime(const std::string &time);
    
    DayTime(const DayTime&) = default;
    DayTime(DayTime&&) = default;
    //OPERATORS
    auto operator=(const DayTime&) -> DayTime& = default;
    auto operator=(DayTime&&) -> DayTime& = default;
    /// \brief Checks if DayTimes objects are equal.
    /// \note Precision is taken into account. That is if comparable objects are having different precision - false is returned
    /// \param r const DayTime&
    /// \return true if objects are equal and false otherwise
    auto operator==(const DayTime& r) const -> bool;
    /// \brief Checks if one DayTime object is less then other one
    /// \note Precision is taken into account. That is if comparable objects are having different precision - false is returned
    /// \param r const DayTime&
    /// \return true if left object is less and false otherwise
    auto operator<(const DayTime& r) const -> bool;
    
    void operator += (const DayTime& r);
    void operator -= (const DayTime& r);
    
    virtual ~DayTime() = default;
    
    void setOffset(TimeZone offset) {m_offset = offset;}
    
    void addHours(uint64_t hours);
    void addMinutes(uint64_t minutes);
    void addSeconds(uint64_t seconds);
    void addMilliseconds(uint64_t milliseconds);
    void addMicroseconds(uint64_t microseconds);
    void addNanoseconds(uint64_t nanoseconds);
    void substractHours(uint64_t hours);
    void substractMinutes(uint64_t minutes);
    void substractSeconds(uint64_t seconds);
    void substractMilliseconds(uint64_t milliseconds);
    void substractMicroseconds(uint64_t microseconds);
    void substractNanoseconds(uint64_t nanoseconds);
    
    [[nodiscard]] auto hours() const  -> uint8_t;
    [[nodiscard]] auto minutes() const -> uint8_t;
    [[nodiscard]] auto seconds() const -> uint8_t;
    [[nodiscard]] auto milliseconds() const -> uint16_t;
    [[nodiscard]] auto microseconds() const -> uint16_t;
    [[nodiscard]] auto nanoseconds() const -> uint16_t;
    
    [[nodiscard]] auto precision() const -> Precision {return m_precision;}
    
    [[nodiscard]] static auto localTime(Precision precision = Precision::SECONDS) -> DayTime;
    /// \brief Generates string representation of time. By default return ISO standard representation in formats represented below. If show_precision is set to true each format will be suffixed by offset in form of +(-)hh
    /// \return std::string to represent the time.
    /// \formats[hours:minutes] - Minutes precision
    /// \formats[hours:minutes:seconds] - Seconds precision
    /// \formats[hours:minutes:seconds.milliseconds] - Milliseconds precision
    /// \formats[hours:minutes:seconds.milliseconds.microseconds] - Microseconds precision
    /// \formats[hours:minutes:seconds.milliseconds.microseconds.nanoseconds] - Nanoseconds precision
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
    void _substractMinutes(uint64_t minutes);
    void _substractSeconds(uint64_t seconds);
    void _substractMilliseconds(uint64_t milliseconds);
    void _substractMicroseconds(uint64_t microseconds);
    void _substractNanoseconds(uint64_t nanoseconds);
};

    auto operator != (const DayTime& l, const DayTime& r) -> bool;
    auto operator > (const DayTime& l, const DayTime& r) -> bool;
    auto operator <= (const DayTime& l, const DayTime& r) -> bool;
    auto operator >= (const DayTime& l, const DayTime& r) -> bool;
    
    auto operator + (const DayTime& l, const DayTime &r) -> DayTime;
    auto operator - (const DayTime& l, const DayTime &r) -> DayTime;
    
    auto operator<<(std::ostream &out, const DayTime &time) -> std::ostream&;
}//namespace Tristan

#endif // TIME_HPP
