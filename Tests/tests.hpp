#ifndef TESTS_HPP
#define TESTS_HPP
#include "date_time.hpp"

#include <gtest/gtest.h>
using namespace mt;
using namespace mt::time;
using namespace mt::date;
using namespace mt::date_time;

TEST(Time, Default_constructor) {
    const Time time;
    const auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    auto [tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year, tm_wday, tm_yday, tm_isdst, tm_gmtoff, tm_zone] = *std::gmtime(&time_t);

    ASSERT_EQ(time.hours().count(), tm_hour) << "Hours are not equal";
    ASSERT_EQ(time.minutes().count(), tm_min) << "Minutes are not equal";
    ASSERT_EQ(time.seconds().count(), tm_sec) << "Seconds are not equal";
}

TEST(Time, EastTimeZone_constructor) {
    auto offset = mt::TimeZone::EAST_2;
    const Time time(offset);
    const Time time_1;
    ASSERT_EQ(time.hours().count(), time_1.hours().count() + static_cast< int8_t >(offset));
}

TEST(Time, TimePoint_constructor) {
    const auto time_point = std::chrono::system_clock::now();
    const Time time(time_point);
    const auto time_t = std::chrono::system_clock::to_time_t(time_point);

    auto [tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year, tm_wday, tm_yday, tm_isdst, tm_gmtoff, tm_zone] = *std::gmtime(&time_t);

    ASSERT_EQ(time.hours().count(), tm_hour) << "Hours are not equal";
    ASSERT_EQ(time.minutes().count(), tm_min) << "Minutes are not equal";
    ASSERT_EQ(time.seconds().count(), tm_sec) << "Seconds are not equal";
}

TEST(Time, Minutes_constructor) {
    const Time time(std::chrono::hours{23}, std::chrono::minutes{23});

    ASSERT_EQ(time.hours().count(), 23) << "Hours are not equal";
    ASSERT_EQ(time.minutes().count(), 23) << "Minutes are not equal";
    EXPECT_THROW(Time(std::chrono::hours{24}, std::chrono::minutes{23}), std::range_error) << "Exception on hours range expected";
    EXPECT_THROW(Time(std::chrono::hours{23}, std::chrono::minutes{60}), std::range_error) << "Exception on minutes range expected";
}

TEST(Time, Seconds_constructor) {
    const Time time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23});

    ASSERT_EQ(time.seconds().count(), 23) << "Seconds are not equal";
    EXPECT_THROW(Time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{60}), std::range_error) << "Exception on seconds range expected";
}

TEST(Time, Milliseconds_constructor) {
    const Time time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23}, std::chrono::milliseconds{23});

    ASSERT_EQ(time.milliseconds().count(), 23) << "Milliseconds are not equal";
    EXPECT_THROW(Time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23}, std::chrono::milliseconds{1000}), std::range_error)
        << "Exception on milliseconds range expected";
}

TEST(Time, Microseconds_constructor) {
    const Time time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23}, std::chrono::milliseconds{23}, std::chrono::microseconds{23});

    ASSERT_EQ(time.microseconds().count(), 23) << "Microseconds are not equal";
    EXPECT_THROW(Time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23}, std::chrono::milliseconds{23}, std::chrono::microseconds{1000}), std::range_error)
        << "Exception on microseconds range expected";
}

TEST(Time, Nanoseconds_constructor) {
    const Time time(
        std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23}, std::chrono::milliseconds{23}, std::chrono::microseconds{23}, std::chrono::nanoseconds{23});

    ASSERT_EQ(time.nanoseconds().count(), 23) << "Nanoseconds are not equal";
    EXPECT_THROW(Time(std::chrono::hours{23},
                      std::chrono::minutes{23},
                      std::chrono::seconds{23},
                      std::chrono::milliseconds{23},
                      std::chrono::microseconds{23},
                      std::chrono::nanoseconds{1000}),
                 std::range_error)
        << "Exception on nanoseconds range expected";
}

TEST(Time, String_constructor) {
    std::string s_time = "23:23:23.023.023.023+02";
    std::string invalid_time1 = "2a:23:23:023:023:023";
    std::string invalid_time2 = "23:23:23:0023:023:023";
    std::string invalid_time3 = "23:23:23-023:023:023";
    Time time(s_time);

    ASSERT_EQ(time.hours().count(), 23) << "Hours are not equal";
    ASSERT_EQ(time.minutes().count(), 23) << "Minutes are not equal";
    ASSERT_EQ(time.seconds().count(), 23) << "Seconds are not equal";
    ASSERT_EQ(time.milliseconds().count(), 23) << "Milliseconds are not equal";
    ASSERT_EQ(time.microseconds().count(), 23) << "Microseconds are not equal";
    ASSERT_EQ(time.nanoseconds().count(), 23) << "Nanoseconds are not equal";
    ASSERT_EQ(time.offset(), TimeZone::EAST_2);

    EXPECT_THROW(Time{invalid_time1}, std::invalid_argument) << "Exception on invalid_time1 expected";
    EXPECT_THROW(Time{invalid_time2}, std::invalid_argument) << "Exception on invalid_time2 range expected";
    EXPECT_THROW(Time{invalid_time3}, std::invalid_argument) << "Exception on invalid_time3 range expected";
}

TEST(Time, AddHours) {
    Time time(std::chrono::hours{21}, std::chrono::minutes{23});
    time += std::chrono::hours{1};
    ASSERT_EQ(time.hours().count(), 22);
    time += std::chrono::hours{2};
    ASSERT_EQ(time.hours().count(), 0);
    time += std::chrono::hours{1};
    time += std::chrono::hours{24};
    ASSERT_EQ(time.hours().count(), 1);
    time += std::chrono::hours{25};
    ASSERT_EQ(time.hours().count(), 2);
}

TEST(Time, AddMinutes) {
    Time time(std::chrono::hours{21}, std::chrono::minutes{23});

    time += std::chrono::minutes{1};
    ASSERT_EQ(time.minutes().count(), 24);
    time += std::chrono::minutes{60};
    ASSERT_EQ(time.minutes().count(), 24);
    ASSERT_EQ(time.hours().count(), 22);
}

TEST(Time, AddSeconds) {
    Time time(std::chrono::hours{21}, std::chrono::minutes{23}, std::chrono::seconds{23});

    time += std::chrono::seconds{1};
    ASSERT_EQ(time.seconds().count(), 24);
    time += std::chrono::seconds{60};
    ASSERT_EQ(time.seconds().count(), 24);
    ASSERT_EQ(time.minutes().count(), 24);
    time += std::chrono::seconds{3600};
    ASSERT_EQ(time.seconds().count(), 24);
    ASSERT_EQ(time.minutes().count(), 24);
    ASSERT_EQ(time.hours().count(), 22);
}

TEST(Time, AddMilliseconds) {
    Time time(std::chrono::hours{21}, std::chrono::minutes{23}, std::chrono::seconds{23}, std::chrono::milliseconds{23});

    time += std::chrono::milliseconds{1};
    ASSERT_EQ(time.milliseconds().count(), 24);
    time += std::chrono::milliseconds{1000};
    ASSERT_EQ(time.milliseconds().count(), 24);
    ASSERT_EQ(time.seconds().count(), 24);
}

TEST(Time, AddMicroseconds) {
    Time time(std::chrono::hours{21}, std::chrono::minutes{23}, std::chrono::seconds{23}, std::chrono::milliseconds{23}, std::chrono::microseconds{23});

    time += std::chrono::microseconds{1};
    ASSERT_EQ(time.microseconds().count(), 24);
    time += std::chrono::microseconds{1000};
    ASSERT_EQ(time.microseconds().count(), 24);
    ASSERT_EQ(time.milliseconds().count(), 24);
}

TEST(Time, AddNanoseconds) {
    Time time(
        std::chrono::hours{21}, std::chrono::minutes{23}, std::chrono::seconds{23}, std::chrono::milliseconds{23}, std::chrono::microseconds{23}, std::chrono::nanoseconds{23});

    time += std::chrono::nanoseconds{1};
    ASSERT_EQ(time.nanoseconds().count(), 24);
    time += std::chrono::nanoseconds{1000};
    ASSERT_EQ(time.nanoseconds().count(), 24);
    ASSERT_EQ(time.microseconds().count(), 24);
}

TEST(Time, SubtractHours) {
    Time time(std::chrono::hours{21}, std::chrono::minutes{23});

    time -= std::chrono::hours{1};
    ASSERT_EQ(time.hours().count(), 20);
    time -= std::chrono::hours{2};
    ASSERT_EQ(time.hours().count(), 18);
    time -= std::chrono::hours{24};
    ASSERT_EQ(time.hours().count(), 18);
    time -= std::chrono::hours{25};
    ASSERT_EQ(time.hours().count(), 17);
}

TEST(Time, SubtractMinutes) {
    Time time(std::chrono::hours{21}, std::chrono::minutes{23});

    time -= std::chrono::minutes{1};
    ASSERT_EQ(time.minutes().count(), 22);
    time -= std::chrono::minutes{60};
    ASSERT_EQ(time.minutes().count(), 22);
    ASSERT_EQ(time.hours().count(), 20);
}

TEST(Time, SubtractSeconds) {
    Time time(std::chrono::hours{21}, std::chrono::minutes{23}, std::chrono::seconds{23});

    time -= std::chrono::seconds{1};
    ASSERT_EQ(time.seconds().count(), 22);
    time -= std::chrono::seconds{60};
    ASSERT_EQ(time.seconds().count(), 22);
    ASSERT_EQ(time.minutes().count(), 22);
    time -= std::chrono::seconds{3600};
    ASSERT_EQ(time.seconds().count(), 22);
    ASSERT_EQ(time.minutes().count(), 22);
    ASSERT_EQ(time.hours().count(), 20);
}

TEST(Time, SubtractMilliseconds) {
    Time time(std::chrono::hours{21}, std::chrono::minutes{23}, std::chrono::seconds{23}, std::chrono::milliseconds{23});

    time -= std::chrono::milliseconds{1};
    ASSERT_EQ(time.milliseconds().count(), 22);
    time -= std::chrono::milliseconds{1000};
    ASSERT_EQ(time.milliseconds().count(), 22);
    ASSERT_EQ(time.seconds().count(), 22);
}

TEST(Time, SubtractMicroseconds) {
    Time time(std::chrono::hours{21}, std::chrono::minutes{23}, std::chrono::seconds{23}, std::chrono::milliseconds{23}, std::chrono::microseconds{23});

    time -= std::chrono::microseconds{1};
    ASSERT_EQ(time.microseconds().count(), 22);
    time -= std::chrono::microseconds{1000};
    ASSERT_EQ(time.microseconds().count(), 22);
    ASSERT_EQ(time.milliseconds().count(), 22);
}

TEST(Time, SubtractNanoseconds) {
    Time time(
        std::chrono::hours{21}, std::chrono::minutes{23}, std::chrono::seconds{23}, std::chrono::milliseconds{23}, std::chrono::microseconds{23}, std::chrono::nanoseconds{23});

    time -= std::chrono::nanoseconds{1};
    ASSERT_EQ(time.nanoseconds().count(), 22);
    time -= std::chrono::nanoseconds{1000};
    ASSERT_EQ(time.nanoseconds().count(), 22);
    ASSERT_EQ(time.microseconds().count(), 22);
}

TEST(Time, LocalTime) {
    const auto time = Time::localTime();
    const auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    auto [tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year, tm_wday, tm_yday, tm_isdst, tm_gmtoff, tm_zone] = *std::localtime(&time_t);

    ASSERT_EQ(time.hours().count(), tm_hour) << "Hours are not equal";
    ASSERT_EQ(time.minutes().count(), tm_min) << "Minutes are not equal";
    ASSERT_EQ(time.seconds().count(), tm_sec) << "Seconds are not equal";
}

TEST(Time, OperatorEqual) {
    const Time left_time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23});
    Time right_time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23});

    ASSERT_TRUE(left_time == right_time);
    right_time += std::chrono::hours{1};
    ASSERT_FALSE(left_time == right_time);
}

TEST(Time, OperatorLess) {
    const Time left_time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{22});
    const Time right_time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23});

    ASSERT_TRUE(left_time < right_time);
}

TEST(Time, OperatorPlus) {
    const Time left_time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23});
    const Time right_time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23});

    const auto day_time = left_time + right_time;
    const Time test_time(std::chrono::hours{22}, std::chrono::minutes{46}, std::chrono::seconds{46});
    ASSERT_TRUE(day_time == test_time);
}

TEST(Time, OperatorMinus) {
    const Time left_time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23});
    const Time right_time(std::chrono::hours{22}, std::chrono::minutes{24}, std::chrono::seconds{24});

    const auto day_time = left_time - right_time;

    ASSERT_TRUE(day_time == Time(std::chrono::hours{0}, std::chrono::minutes{58}, std::chrono::seconds{59}));
}

TEST(Time, OperatorPlusAssign) {
    Time left_time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23});
    const Time right_time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23});

    const auto day_time = left_time + right_time;
    left_time += right_time;
    ASSERT_TRUE(day_time == left_time);
}

TEST(Time, OperatorMinusAssign) {
    Time left_time(std::chrono::hours{23}, std::chrono::minutes{23}, std::chrono::seconds{23});
    const Time right_time(std::chrono::hours{22}, std::chrono::minutes{24}, std::chrono::seconds{24});

    const auto day_time = left_time - right_time;
    left_time -= right_time;
    ASSERT_TRUE(day_time == left_time);
}

TEST(Date, DefaultConstructor) {
    const Date date;
    const auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    auto [tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year, tm_wday, tm_yday, tm_isdst, tm_gmtoff, tm_zone] = *std::gmtime(&time_t);
    ASSERT_EQ(date.monthDay< int32_t >(), tm_mday);
    ASSERT_EQ(date.weekDay< int32_t >(), tm_wday);
    ASSERT_EQ(date.month< int32_t >(), tm_mon + 1);
    ASSERT_EQ(date.year< int32_t >(), tm_year + 1900);
}

TEST(Date, SecondsConstructor) {
    const std::vector< std::pair< std::chrono::seconds, int16_t > > time_stamps{
        std::pair< std::chrono::seconds, uint16_t >{std::chrono::seconds{1577836801}, 2020},
        std::pair< std::chrono::seconds, uint16_t >{std::chrono::seconds{1609459199}, 2020},
        std::pair< std::chrono::seconds, uint16_t >{std::chrono::seconds{1609459201}, 2021},
        std::pair< std::chrono::seconds, uint16_t >{std::chrono::seconds{1640995199}, 2021},
        std::pair< std::chrono::seconds, uint16_t >{std::chrono::seconds{1640995201}, 2022},
        std::pair< std::chrono::seconds, uint16_t >{std::chrono::seconds{1672531199}, 2022},
        std::pair< std::chrono::seconds, uint16_t >{std::chrono::seconds{1672531201}, 2023},
        std::pair< std::chrono::seconds, uint16_t >{std::chrono::seconds{1704067199}, 2023},
        std::pair< std::chrono::seconds, uint16_t >{std::chrono::seconds{1704067201}, 2024},
        std::pair< std::chrono::seconds, uint16_t >{std::chrono::seconds{1735689599}, 2024},
        std::pair< std::chrono::seconds, uint16_t >{std::chrono::seconds{1735689601}, 2025},
    };
    bool january = true;
    for (auto [time_stamp, year]: time_stamps) {
        Date date{time_stamp};
        ASSERT_EQ(date.year< uint16_t >(), year);
        if (january) {
            ASSERT_EQ(date.month< uint8_t >(), uint8_t{1});
            ASSERT_EQ(date.monthDay< uint8_t >(), uint8_t{1});
            if (year == 2020) {
                ASSERT_EQ(date.weekDay(), std::chrono::Wednesday);
            }
            else if (year == 2021) {
                ASSERT_EQ(date.weekDay(), std::chrono::Friday);
            }
            else if (year == 2022) {
                ASSERT_EQ(date.weekDay(), std::chrono::Saturday);
            }
            else if (year == 2023) {
                ASSERT_EQ(date.weekDay(), std::chrono::Sunday);
            }
            else if (year == 2024) {
                ASSERT_EQ(date.weekDay(), std::chrono::Monday);
            } else {
                ASSERT_EQ(date.weekDay(), std::chrono::Wednesday);
            }
        } else {
            ASSERT_EQ(date.month< uint8_t >(), uint8_t{12});
            ASSERT_EQ(date.monthDay< uint8_t >(), uint8_t{31});
            if (year == 2020) {
                ASSERT_EQ(date.weekDay(), std::chrono::Thursday);
            }
            else if (year == 2021) {
                ASSERT_EQ(date.weekDay(), std::chrono::Friday);
            }
            else if (year == 2022) {
                ASSERT_EQ(date.weekDay(), std::chrono::Saturday);
            }
            else if (year == 2023) {
                ASSERT_EQ(date.weekDay(), std::chrono::Sunday);
            }
            else {
                ASSERT_EQ(date.weekDay(), std::chrono::Tuesday);
            }
        }
        january = !january;
    }
}

TEST(Date, OperatorPlusMinus) {
    Date date(std::chrono::years{2024}, std::chrono::months{1}, std::chrono::days{1});
    date += std::chrono::years{1};
    ASSERT_EQ(date.year(), std::chrono::year{2025});
    date -= std::chrono::years{1};
    ASSERT_EQ(date.year(), std::chrono::year{2024});
    date += std::chrono::days{30};
    date += std::chrono::months{1};
    ASSERT_EQ(date.month(), std::chrono::February);
    ASSERT_EQ(date.monthDay(), std::chrono::day{29});
    date += std::chrono::months{1};
    ASSERT_EQ(date.month(), std::chrono::March);
    ASSERT_EQ(date.monthDay(), std::chrono::day{29});
    date += std::chrono::days{1};
    ASSERT_EQ(date.month(), std::chrono::March);
    ASSERT_EQ(date.monthDay(), std::chrono::day{30});
    date -= std::chrono::months{1};
    ASSERT_EQ(date.month(), std::chrono::February);
    ASSERT_EQ(date.monthDay(), std::chrono::day{29});
    date -= std::chrono::years{1};
    ASSERT_EQ(date.year(), std::chrono::year{2023});
    ASSERT_EQ(date.month(), std::chrono::February);
    ASSERT_EQ(date.monthDay(), std::chrono::day{28});
    date += std::chrono::years{1};
    ASSERT_EQ(date.year(), std::chrono::year{2024});
    ASSERT_EQ(date.month(), std::chrono::February);
    ASSERT_EQ(date.monthDay(), std::chrono::day{28});
    date += std::chrono::days{1};
    ASSERT_EQ(date.month(), std::chrono::February);
    ASSERT_EQ(date.monthDay(), std::chrono::day{29});
    date += std::chrono::years{1};
    ASSERT_EQ(date.year(), std::chrono::year{2025});
    ASSERT_EQ(date.month(), std::chrono::February);
    ASSERT_EQ(date.monthDay(), std::chrono::day{28});
    date += std::chrono::months{1};
    ASSERT_EQ(date.year(), std::chrono::year{2025});
    ASSERT_EQ(date.month(), std::chrono::March);
    ASSERT_EQ(date.monthDay(), std::chrono::day{28});
    date += std::chrono::days{3};
    ASSERT_EQ(date.year(), std::chrono::year{2025});
    ASSERT_EQ(date.month(), std::chrono::March);
    ASSERT_EQ(date.monthDay(), std::chrono::day{31});
    date += std::chrono::months{1};
    ASSERT_EQ(date.year(), std::chrono::year{2025});
    ASSERT_EQ(date.month(), std::chrono::April);
    ASSERT_EQ(date.monthDay(), std::chrono::day{30});
    date += std::chrono::months{1};
    date += std::chrono::days{1};
    ASSERT_EQ(date.year(), std::chrono::year{2025});
    ASSERT_EQ(date.month(), std::chrono::May);
    ASSERT_EQ(date.monthDay(), std::chrono::day{31});
    date -= std::chrono::months{1};
    ASSERT_EQ(date.year(), std::chrono::year{2025});
    ASSERT_EQ(date.month(), std::chrono::April);
    ASSERT_EQ(date.monthDay(), std::chrono::day{30});
    date = Date(std::chrono::years{2024}, std::chrono::months{1}, std::chrono::days{1});
    date += std::chrono::days{31};
    ASSERT_EQ(date.month(), std::chrono::February);
    ASSERT_EQ(date.monthDay(), std::chrono::day{1});
    date += std::chrono::days{28};
    ASSERT_EQ(date.month(), std::chrono::February);
    ASSERT_EQ(date.monthDay(), std::chrono::day{29});
    date += std::chrono::days{31};
    ASSERT_EQ(date.month(), std::chrono::March);
    ASSERT_EQ(date.monthDay(), std::chrono::day{31});
    date += std::chrono::days{365};
    ASSERT_EQ(date.year(), std::chrono::year{2025});
    ASSERT_EQ(date.month(), std::chrono::March);
    ASSERT_EQ(date.monthDay(), std::chrono::day{31});
}

#endif  // TESTS_HPP