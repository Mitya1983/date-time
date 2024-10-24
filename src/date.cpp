#include "date.hpp"
#include "time.hpp"

#include <algorithm>
#include <source_location>
#if defined __cpp_lib_format
  #include <format>
#endif

mt::date::Date::Date() { m_date = std::chrono::year_month_day{std::chrono::floor< std::chrono::days >(std::chrono::system_clock::now())}; }

mt::date::Date::Date(const std::chrono::seconds since_epoch) {
    const std::chrono::time_point< std::chrono::system_clock > time_point{std::chrono::duration_cast< std::chrono::system_clock::duration >(since_epoch)};
    m_date = std::chrono::year_month_day{std::chrono::floor< std::chrono::days >(time_point)};
}

mt::date::Date::Date(mt::TimeZone p_time_zone) {
    auto time_point_now = std::chrono::system_clock::now();
    time_point_now += std::chrono::duration_cast< std::chrono::system_clock::duration >(std::chrono::hours{static_cast< int8_t >(p_time_zone)});
    m_date = std::chrono::year_month_day{std::chrono::floor< std::chrono::days >(time_point_now)};
}

mt::date::Date::Date(const std::chrono::year p_year, const std::chrono::month p_month, const std::chrono::day p_day) {
    if (!p_day.ok()) {
#if defined __cpp_lib_format
        throw std::range_error(std::format("Bad [day] value was provided - {} the value between 1 and 31 is expected", p_day));
#else
        std::string message = "Bad [day] value was provided - " + std::to_string(static_cast< uint32_t >(day)) + " the value between 1 and 31 is expected";
        throw std::range_error(message);
#endif
    }
    if (!p_month.ok()) {
#if defined __cpp_lib_format
        throw std::range_error(std::format("Bad [month] value was provided - {} the value between 1 and 12 is expected", p_month));
#else
        std::string message = "Bad [month] value was provided - " + std::to_string(static_cast< uint32_t >(month)) + " the value between 1 and 12 is expected";
        throw std::range_error(message);
#endif
    }
    if ((p_month == std::chrono::April || p_month == std::chrono::June || p_month == std::chrono::September || p_month == std::chrono::November) && p_day == std::chrono::day{31}) {
#if defined __cpp_lib_format
        throw std::range_error(std::format("Date 31 is not possible for provided month {}", p_month));
#else
        std::string message = "Day 31 is not possible for provided month " + std::to_string(static_cast< uint32_t >(month));
        throw std::range_error(message);
#endif
    }
    if (p_month == std::chrono::February) {
        if (const auto leap_year = p_year.is_leap(); (leap_year && p_day > std::chrono::day{29}) || (not leap_year && p_day > std::chrono::day{28})) {
#if defined __cpp_lib_format
            throw std::range_error(std::format("Day {} is not possible for provided month {}", p_day, p_month));
#else
            const std::string message
                = "Day " + std::to_string(static_cast< uint32_t >(day)) + " is not possible for provided month " + std::to_string(static_cast< uint32_t >(month));
            throw std::range_error(message);
#endif
        }
    }
    m_date = std::chrono::year_month_day{p_year, p_month, p_day};
}

mt::date::Date::Date(const std::chrono::years p_years, const std::chrono::months p_months, const std::chrono::days p_days) :
    Date{std::chrono::year{static_cast< int32_t >(p_years.count())},
         std::chrono::month{static_cast< uint32_t >(p_months.count())},
         std::chrono::day{static_cast< uint32_t >(p_days.count())}} { }

mt::date::Date::Date(const std::string& p_iso_date) {
    const auto l_length = p_iso_date.length();
    if (l_length != 8 && l_length != 10) {
#if defined __cpp_lib_format
        throw std::invalid_argument(std::format("Provided date has invalid format. Expected either YYYYMMDD either YYYY-MM-DD, but {} received", p_iso_date));
#else
        throw std::invalid_argument("Provided date has invalid format. "
                                    "Value should be provided as either 8 (YYYYMMDD) or 10 (YYYY-MM-DD) characters string. "
                                    "But "
                                    + std::to_string(l_length) + " characters string was provided");
#endif
    }
    if (not std::ranges::any_of(p_iso_date, [](const char c) {
            if (c < '0' || c > '9') {
                if (c != '-') {
                    return true;
                }
            }
            return false;
        })) {
        const auto year = std::chrono::year{std::stoi(p_iso_date.substr(0, 4))};
        std::chrono::month month{};
        std::chrono::day day{};
        switch (l_length) {  //NOLINT
            case 8: {
                month = std::chrono::month{static_cast< uint32_t >(std::stoi(p_iso_date.substr(4, 2)))};
                day = std::chrono::day{static_cast< uint32_t >(std::stoi(p_iso_date.substr(6, 2)))};
                break;
            }
            case 10: {
                month = std::chrono::month{static_cast< uint32_t >(std::stoi(p_iso_date.substr(5, 2)))};
                day = std::chrono::day{static_cast< uint32_t >(std::stoi(p_iso_date.substr(8, 2)))};
                break;
            }
        }
        if (!day.ok()) {
#if defined __cpp_lib_format
            throw std::range_error(std::format("Bad [day] value was provided - {} the value between 1 and 31 is expected", day));
#else
            std::string message = "Bad [day] value was provided - " + std::to_string(static_cast< uint32_t >(day)) + " the value between 1 and 31 is expected";
            throw std::range_error(message);
#endif
        }
        if (!month.ok()) {
#if defined __cpp_lib_format
            throw std::range_error(std::format("Bad [month] value was provided - {} the value between 1 and 12 is expected", month));
#else
            std::string message = "Bad [month] value was provided - " + std::to_string(static_cast< uint32_t >(month)) + " the value between 1 and 12 is expected";
            throw std::range_error(message);
#endif
        }
        if ((month == std::chrono::April || month == std::chrono::June || month == std::chrono::September || month == std::chrono::November) && day == std::chrono::day{31}) {
#if defined __cpp_lib_format
            throw std::range_error(std::format("Date 31 is not possible for provided month {}", month));
#else
            std::string message = "Day 31 is not possible for provided month " + std::to_string(static_cast< uint32_t >(month));
            throw std::range_error(message);
#endif
        }
        if (month == std::chrono::February) {
            if (const auto leap_year = year.is_leap(); (leap_year && day > std::chrono::day{29}) || (not leap_year && day > std::chrono::day{28})) {
#if defined __cpp_lib_format
                throw std::range_error(std::format("Day {} is not possible for provided month {}", day, month));
#else
                const std::string message
                    = "Day " + std::to_string(static_cast< uint32_t >(day)) + " is not possible for provided month " + std::to_string(static_cast< uint32_t >(month));
                throw std::range_error(message);
#endif
            }
        }
        m_date = std::chrono::year_month_day{year, month, day};
    } else {
        throw std::invalid_argument("Bad [iso_date] string format. String should contain only numbers and hyphen");
    }
}

auto mt::date::Date::operator==(const mt::date::Date& other) const -> bool { return m_date == other.m_date; }

auto mt::date::Date::operator<(const mt::date::Date& other) const -> bool { return m_date < other.m_date; }

void mt::date::Date::operator+=(const DateDuration p_value) { *this = *this + p_value; }

void mt::date::Date::operator-=(const DateDuration p_value) { *this = *this - p_value; }

auto mt::date::Date::date() const -> std::chrono::year_month_day { return m_date; }

auto mt::date::Date::monthDay() const -> std::chrono::day { return m_date.day(); }

auto mt::date::Date::weekDay() const -> std::chrono::weekday { return std::chrono::weekday{m_date}; }

auto mt::date::Date::month() const -> std::chrono::month { return m_date.month(); }

auto mt::date::Date::year() const -> std::chrono::year { return m_date.year(); }

auto mt::date::Date::isWeekend() const -> bool { return weekDay() == std::chrono::Sunday || weekDay() == std::chrono::Saturday; }

std::string mt::date::Date::toString(const std::function< std::string(const Date&) >& formatter) const {
    if (formatter) {
        return formatter(*this);
    }
#if defined __cpp_lib_format
    return std::format("{0:%Y-%m-%d}", *this);
#else
    std::string result;
    result += std::to_string(p_date.year< uint16_t >());
    result += '-';
    const auto month = p_date.month< uint8_t >();
    if (month < 10)
        result += '0';
    result += std::to_string(month);
    result += '-';
    const auto day = p_date.monthDay< uint8_t >();
    if (day < 10)
        result += '0';
    result += std::to_string(day);

    return result;
#endif
}

auto mt::date::Date::localDate() -> mt::date::Date {
    const auto tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto offset = std::localtime(&tm)->tm_gmtoff;
    return mt::date::Date(static_cast< mt::TimeZone >(offset / 3600));
}

bool mt::date::operator!=(const mt::date::Date& l, const mt::date::Date& r) { return !(l == r); }

bool mt::date::operator>(const mt::date::Date& l, const mt::date::Date& r) { return !(l < r); }

bool mt::date::operator<=(const mt::date::Date& l, const mt::date::Date& r) { return l < r || l == r; }

bool mt::date::operator>=(const mt::date::Date& l, const mt::date::Date& r) { return l > r || l == r; }

mt::date::Date mt::date::operator+(Date p_date, const DateDuration p_value) {
    std::visit(
        [&p_date]< typename DateValueType >(DateValueType&& value) -> void {
            if constexpr (std::is_same_v< std::decay_t< DateValueType >, std::chrono::years >) {
                p_date.m_date += value;
                if (!p_date.year().is_leap() && p_date.month() == std::chrono::February && p_date.monthDay() == std::chrono::day{29}) {
                    p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{28}};
                }
            } else if constexpr (std::is_same_v< std::decay_t< DateValueType >, std::chrono::months >) {
                p_date.m_date += value;
                if (p_date.month() == std::chrono::February && p_date.monthDay() > std::chrono::day{28}) {
                    if (!p_date.year().is_leap()) {
                        p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{28}};
                    } else {
                        p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{29}};
                    }
                } else if (const auto month = p_date.month(); (month == std::chrono::April || month == std::chrono::June || month == std::chrono::September
                                                              || month == std::chrono::November) && p_date.monthDay() == std::chrono::day{31}) {
                    p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{30}};
                }
            } else {
                auto time_point = std::chrono::sys_days(p_date.m_date);
                time_point += value;
                p_date.m_date = std::chrono::year_month_day{time_point};
                if (p_date.month() == std::chrono::February && p_date.monthDay() > std::chrono::day{28}) {
                    if (!p_date.year().is_leap()) {
                        p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{28}};
                    } else {
                        p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{29}};
                    }
                } else if (const auto month = p_date.month(); (month == std::chrono::April || month == std::chrono::June || month == std::chrono::September
                                                              || month == std::chrono::November) && p_date.monthDay() == std::chrono::day{31}) {
                    p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{30}};
                }
            }
        },
        p_value);
    return p_date;
}

mt::date::Date mt::date::operator-(Date p_date, const DateDuration p_value) {
    std::visit(
        [&p_date]< typename DateValueType >(DateValueType&& value) -> void {
            if constexpr (std::is_same_v< std::decay_t< DateValueType >, std::chrono::years >) {
                p_date.m_date -= value;
                if (!p_date.year().is_leap() && p_date.month() == std::chrono::February && p_date.monthDay() == std::chrono::day{29}) {
                    p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{28}};
                }
            } else if constexpr (std::is_same_v< std::decay_t< DateValueType >, std::chrono::months >) {
                p_date.m_date -= value;
                if (p_date.month() == std::chrono::February && p_date.monthDay() > std::chrono::day{28}) {
                    if (!p_date.year().is_leap()) {
                        p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{28}};
                    } else {
                        p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{29}};
                    }
                } else if (const auto month = p_date.month(); (month == std::chrono::April || month == std::chrono::June || month == std::chrono::September
                                                              || month == std::chrono::November) && p_date.monthDay() == std::chrono::day{31}) {
                    p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{30}};
                }
            } else {
                auto time_point = std::chrono::sys_days(p_date.m_date);
                time_point -= value;
                p_date.m_date = std::chrono::year_month_day{time_point};
                if (p_date.month() == std::chrono::February && p_date.monthDay() > std::chrono::day{28}) {
                    if (!p_date.year().is_leap()) {
                        p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{28}};
                    } else {
                        p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{29}};
                    }
                } else if (const auto month = p_date.month(); (month == std::chrono::April || month == std::chrono::June || month == std::chrono::September
                                                              || month == std::chrono::November) && p_date.monthDay() == std::chrono::day{31}) {
                    p_date.m_date = std::chrono::year_month_day{p_date.year(), p_date.month(), std::chrono::day{30}};
                }
            }
        },
        p_value);
    return p_date;
}

auto mt::date::operator<<(std::ostream& out, const Date& date) -> std::ostream& {
    const auto& _string = date.toString();
    out.write(_string.data(), std::ssize(_string));
    return out;
}