#ifndef TIME_HPP
#define TIME_HPP
#include <string>
#include <iostream>
#include <chrono>


namespace Tristan::Time {

    enum class Precision : uint8_t {
        MINUTES,
        SECONDS,
        MILLISECONDS,
        MICROSECONDS,
        NANOSECONDS
    };
    
    struct TimeOffset {
        uint8_t hours = 0;
        uint8_t minutes = 0;
    };
    
class Time
{
    friend auto operator + (const Time& l, const Time &r) -> Time;
    friend auto operator - (const Time& l, const Time &r) -> Time;
    
    class Durations {
        friend class Time;
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
    
    explicit Time(Precision precision = Precision::SECONDS);
    explicit Time(uint8_t hours, uint8_t minutes) noexcept (false /*std::range_error*/);
    explicit Time(uint8_t hours, uint8_t minutes, uint8_t seconds) noexcept (false /*std::range_error*/);
    explicit Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept (false /*std::range_error*/);
    explicit Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds) noexcept (false /*std::range_error*/);
    explicit Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds, uint16_t nanoseconds) noexcept (false /*std::range_error*/);
    
    /// @brief Parses the string provided and create time object
    /// @param time std::string representing time in following formats
    /// \formats[hours, minutes, seconds] - two digits
    /// \formats[milliseconds, microseconds and nanoseconds] - four digits
    /// \formats[hours:minutes] - Minutes precision
    /// \formats[hours:minutes:seconds] - Seconds precision
    /// \formats[hours:minutes:seconds:milliseconds] - Milliseconds precision
    /// \formats[hours:minutes:seconds:milliseconds:microseconds] - Microseconds precision
    /// \formats[hours:minutes:seconds:milliseconds:microseconds:nanoseconds] - Nanoseconds precision
    explicit Time(const std::string &time);
    
    Time(const Time&) = default;
    Time(Time&&) = default;
    //OPERATORS
    Time& operator=(const Time&) = default;
    Time& operator=(Time&&) = default;
    bool operator==(const Time &r) const;
    bool operator<(const Time &r) const;
    
    void operator += (const Time& r);
    void operator -= (const Time& r);
    
    virtual ~Time() = default;
    
    void setOffset(TimeOffset offset);
    
    void addHours(uint64_t hours);
    void addMinutes(uint64_t minutes);
    void addSeconds(uint64_t seconds);
    void addMilliseconds(uint64_t milliseconds);
    void addMicroseconds(uint64_t microseconds);
    void addNanoseconds(uint64_t nanoseconds);
    
    [[nodiscard]] auto hours() const  -> int {m_durations.m_hours.count();};
    [[nodiscard]] auto minutes() const -> int {m_durations.m_minutes.count();}
    [[nodiscard]] auto seconds() const -> int {m_durations.m_seconds.count();}
    [[nodiscard]] auto milliseconds() const -> int {m_durations.m_milliseconds.count();}
    [[nodiscard]] auto microseconds() const -> int {m_durations.m_microseconds.count();}
    [[nodiscard]] auto nanoseconds() const -> int {m_durations.m_nanoseconds.count();}
    [[nodiscard]] auto precision() const -> Precision {return m_precision;}
    
    [[nodiscard]] auto durations() const -> const Durations& {return m_durations;}
    
    [[nodiscard]] auto localTime() const -> Time;
    /// @brief Generates string representation of time. By default return ISO standard representation in formats represented below. If TimeOffset is set each format will be suffixed by offset in form of +(-)00:00
    /// @return std::string to represent the time.
    /// \formats[hours:minutes] - Minutes precision
    /// \formats[hours:minutes:seconds] - Seconds precision
    /// \formats[hours:minutes:seconds:milliseconds] - Milliseconds precision
    /// \formats[hours:minutes:seconds:milliseconds:microseconds] - Microseconds precision
    /// \formats[hours:minutes:seconds:milliseconds:microseconds:nanoseconds] - Nanoseconds precision
    [[nodiscard]] virtual auto toString() const -> std::string;

protected:

private:
    Durations m_durations;
    
    TimeOffset m_offset;
    
    Precision m_precision;
};

    auto operator != (const Time& l, const Time& r) -> bool;
    auto operator > (const Time& l, const Time& r) -> bool;
    auto operator <= (const Time& l, const Time& r) -> bool;
    auto operator >= (const Time& l, const Time& r) -> bool;
    
    auto operator + (const Time& l, const Time &r) -> Time;
    auto operator - (const Time& l, const Time &r) -> Time;
    
    auto operator<<(std::ostream &out, const Time &time) -> std::ostream&;
}//namespace Tristan

#endif // TIME_HPP
