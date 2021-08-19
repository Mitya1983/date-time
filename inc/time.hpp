#ifndef TIME_HPP
#define TIME_HPP
#include <string>
#include <iostream>
#include <chrono>


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
        UTC,
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
    
    class Durations {
        friend class DayTime;
    public:
        [[nodiscard]]auto hours() const noexcept -> const std::chrono::hours& {return m_hours;}
        [[nodiscard]]auto minutes() const noexcept -> const std::chrono::minutes& {return m_minutes;}
        [[nodiscard]]auto seconds() const noexcept -> const std::chrono::seconds& {return m_seconds;}
        [[nodiscard]]auto milliseconds() const noexcept -> const std::chrono::milliseconds& {return m_milliseconds;}
        [[nodiscard]]auto microseconds() const noexcept -> const std::chrono::microseconds& {return m_microseconds;}
        [[nodiscard]]auto nanoseconds() const noexcept -> const std::chrono::nanoseconds& {return m_nanoseconds;}
    private:
        std::chrono::hours m_hours;
        std::chrono::minutes m_minutes;
        std::chrono::seconds m_seconds;
        std::chrono::milliseconds m_milliseconds;
        std::chrono::microseconds m_microseconds;
        std::chrono::nanoseconds m_nanoseconds;
    };
public:
    
    explicit DayTime(Precision precision = Precision::SECONDS);
    explicit DayTime(uint8_t hours, uint8_t minutes) noexcept (false /*std::range_error*/);
    explicit DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds) noexcept (false /*std::range_error*/);
    explicit DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept (false /*std::range_error*/);
    explicit DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds) noexcept (false /*std::range_error*/);
    explicit DayTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds, uint16_t nanoseconds) noexcept (false /*std::range_error*/);
    
    /// @brief Parses the string provided and create time object
    /// @param time std::string representing time in following formats
    /// \formats[hours, minutes, seconds] - two digits
    /// \formats[milliseconds, microseconds and nanoseconds] - three digits
    /// \formats[hours:minutes] - Minutes precision
    /// \formats[hours:minutes:seconds] - Seconds precision
    /// \formats[hours:minutes:seconds.milliseconds] - Milliseconds precision
    /// \formats[hours:minutes:seconds.milliseconds.microseconds] - Microseconds precision
    /// \formats[hours:minutes:seconds.milliseconds.microseconds.nanoseconds] - Nanoseconds precision
    explicit DayTime(const std::string &time);
    
    DayTime(const DayTime&) = default;
    DayTime(DayTime&&) = default;
    //OPERATORS
    DayTime& operator=(const DayTime&) = default;
    DayTime& operator=(DayTime&&) = default;
    bool operator==(const DayTime &r) const;
    bool operator<(const DayTime &r) const;
    
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
    
    [[nodiscard]] auto hours() const  -> uint8_t {return static_cast<uint8_t>(m_durations.m_hours.count());};
    [[nodiscard]] auto minutes() const -> uint8_t {return static_cast<uint8_t>(m_durations.m_minutes.count());}
    [[nodiscard]] auto seconds() const -> uint8_t {return static_cast<uint8_t>(m_durations.m_seconds.count());}
    [[nodiscard]] auto milliseconds() const -> uint16_t {return static_cast<uint16_t>(m_durations.m_milliseconds.count());}
    [[nodiscard]] auto microseconds() const -> uint16_t {return static_cast<uint16_t>(m_durations.m_microseconds.count());}
    [[nodiscard]] auto nanoseconds() const -> uint16_t {return static_cast<uint16_t>(m_durations.m_nanoseconds.count());}
    [[nodiscard]] auto precision() const -> Precision {return m_precision;}
    
    [[nodiscard]] auto durations() const -> const Durations& {return m_durations;}
 
    [[nodiscard]] static auto localTime(Precision precision = Precision::SECONDS) -> DayTime;
    /// @brief Generates string representation of time. By default return ISO standard representation in formats represented below. If show_precision is set to true each format will be suffixed by offset in form of +(-)hh
    /// @return std::string to represent the time.
    /// \formats[hours:minutes] - Minutes precision
    /// \formats[hours:minutes:seconds] - Seconds precision
    /// \formats[hours:minutes:seconds.milliseconds] - Milliseconds precision
    /// \formats[hours:minutes:seconds.milliseconds.microseconds] - Microseconds precision
    /// \formats[hours:minutes:seconds.milliseconds.microseconds.nanoseconds] - Nanoseconds precision
    [[nodiscard]] virtual auto toString(bool show_offset) const -> std::string;

protected:

private:
    Durations m_durations;
    
    TimeZone m_offset;
    
    Precision m_precision;
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
