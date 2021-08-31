#include "date_time.hpp"

#include <gtest/gtest.h>

using namespace tristan::time;
using namespace tristan::date;

TEST(DayTime, Default_constructor){
    DayTime time;
    auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    
    struct tm t_time = *std::gmtime(&time_t);
    
    ASSERT_EQ(time.hours(), t_time.tm_hour) << "Hours are not equal";
    ASSERT_EQ(time.minutes(), t_time.tm_min) << "Minutes are not equal";
    ASSERT_EQ(time.seconds(), t_time.tm_sec) << "Seconds are not equal";
}

TEST(DayTime, Minutes_constructor){
    DayTime time(23, 23);
    
    
    ASSERT_EQ(time.hours(), 23) << "Hours are not equal";
    ASSERT_EQ(time.minutes(), 23) << "Minutes are not equal";
    ASSERT_EQ(time.precision(), Precision::MINUTES) << "Precision is not equal";
    EXPECT_THROW(DayTime(24, 23), std::range_error) << "Exception on hours range expected";
    EXPECT_THROW(DayTime(23, 60), std::range_error) << "Exception on minutes range expected";
}

TEST(DayTime, Seconds_constructor){
    DayTime time(23, 23, 23);
    
    ASSERT_EQ(time.seconds(), 23) << "Seconds are not equal";
    ASSERT_EQ(time.precision(), Precision::SECONDS) << "Precision is not equal";
    EXPECT_THROW(DayTime(23, 23, 60), std::range_error) << "Exception on seconds range expected";
}

TEST(DayTime, Milliseconds_constructor){
    DayTime time(23, 23, 23, 23);
    
    ASSERT_EQ(time.milliseconds(), 23) << "Milliseconds are not equal";
    ASSERT_EQ(time.precision(), Precision::MILLISECONDS) << "Precision is not equal";
    EXPECT_THROW(DayTime(23, 23, 23, 1000), std::range_error) << "Exception on milliseconds range expected";
}

TEST(DayTime, Microseconds_constructor){
    DayTime time(23, 23, 23, 23, 23);
    
    ASSERT_EQ(time.microseconds(), 23) << "Microseconds are not equal";
    ASSERT_EQ(time.precision(), Precision::MICROSECONDS) << "Precision is not equal";
    EXPECT_THROW(DayTime(23, 23, 23, 23, 1000), std::range_error) << "Exception on microseconds range expected";
}

TEST(DayTime, Nanoseconds_constructor){
    DayTime time(23, 23, 23, 23, 23, 23);
    
    ASSERT_EQ(time.nanoseconds(), 23) << "Nanoseconds are not equal";
    ASSERT_EQ(time.precision(), Precision::NANOSECONDS) << "Precision is not equal";
    EXPECT_THROW(DayTime(23, 23, 23, 23, 23, 1000), std::range_error) << "Exception on nanoseconds range expected";
}

TEST(DayTime, String_constructor){
    std::string s_time = "23:23:23.023.023.023";
    std::string invalid_time1 = "2a:23:23:023:023:023";
    std::string invalid_time2 = "23:23:23:0023:023:023";
    std::string invalid_time3 = "23:23:23-023:023:023";
    DayTime time(s_time);
    
    ASSERT_EQ(time.hours(), 23) << "Hours are not equal";
    ASSERT_EQ(time.minutes(), 23) << "Minutes are not equal";
    ASSERT_EQ(time.seconds(), 23) << "Seconds are not equal";
    ASSERT_EQ(time.milliseconds(), 23) << "Milliseconds are not equal";
    ASSERT_EQ(time.microseconds(), 23) << "Microseconds are not equal";
    ASSERT_EQ(time.nanoseconds(), 23) << "Nanoseconds are not equal";
    ASSERT_EQ(time.precision(), Precision::NANOSECONDS);
    
    EXPECT_THROW(DayTime{invalid_time1}, std::invalid_argument) << "Exception on invalid_time1 expected";
    EXPECT_THROW(DayTime{invalid_time2}, std::invalid_argument) << "Exception on invalid_time2 range expected";
    EXPECT_THROW(DayTime{invalid_time3}, std::invalid_argument) << "Exception on invalid_time3 range expected";
}

TEST(DayTime, AddHours){
    DayTime time(21, 23);
    
    time.addHours(1);
    ASSERT_EQ(time.hours(), 22);
    time.addHours(2);
    ASSERT_EQ(time.hours(), 0);
    time.addHours(1);
    time.addHours(24);
    ASSERT_EQ(time.hours(), 1);
    time.addHours(25);
    ASSERT_EQ(time.hours(), 2);
}

TEST(DayTime, AddMinutes){
    DayTime time(21, 23);
    
    time.addMinutes(1);
    ASSERT_EQ(time.minutes(), 24);
    time.addMinutes(60);
    ASSERT_EQ(time.minutes(), 24);
    ASSERT_EQ(time.hours(), 22);
}

TEST(DayTime, AddSeconds){
    DayTime time(21, 23, 23);
    
    time.addSeconds(1);
    ASSERT_EQ(time.seconds(), 24);
    time.addSeconds(60);
    ASSERT_EQ(time.seconds(), 24);
    ASSERT_EQ(time.minutes(), 24);
    time.addSeconds(3600);
    ASSERT_EQ(time.seconds(), 24);
    ASSERT_EQ(time.minutes(), 24);
    ASSERT_EQ(time.hours(), 22);
}

TEST(DayTime, AddMilliseconds){
    DayTime time(21, 23, 23, 23);
    
    time.addMilliseconds(1);
    ASSERT_EQ(time.milliseconds(), 24);
    time.addMilliseconds(1000);
    ASSERT_EQ(time.milliseconds(), 24);
    ASSERT_EQ(time.seconds(), 24);
}

TEST(DayTime, AddMicroseconds){
    DayTime time(21, 23, 23, 23, 23);
    
    time.addMicroseconds(1);
    ASSERT_EQ(time.microseconds(), 24);
    time.addMicroseconds(1000);
    ASSERT_EQ(time.microseconds(), 24);
    ASSERT_EQ(time.milliseconds(), 24);
}

TEST(DayTime, AddNanoseconds){
    DayTime time(21, 23, 23, 23, 23, 23);
    
    time.addNanoseconds(1);
    ASSERT_EQ(time.nanoseconds(), 24);
    time.addNanoseconds(1000);
    ASSERT_EQ(time.nanoseconds(), 24);
    ASSERT_EQ(time.microseconds(), 24);
}

TEST(DayTime, SubtractHours){
    DayTime time(21, 23);
    
    time.subtractHours(1);
    ASSERT_EQ(time.hours(), 20);
    time.subtractHours(2);
    ASSERT_EQ(time.hours(), 18);
    time.subtractHours(24);
    ASSERT_EQ(time.hours(), 18);
    time.subtractHours(25);
    ASSERT_EQ(time.hours(), 17);
}

TEST(DayTime, SubtractMinutes){
    DayTime time(21, 23);
    
    time.subtractMinutes(1);
    ASSERT_EQ(time.minutes(), 22);
    time.subtractMinutes(60);
    ASSERT_EQ(time.minutes(), 22);
    ASSERT_EQ(time.hours(), 20);
}

TEST(DayTime, SubtractSeconds){
    DayTime time(21, 23, 23);
    
    time.subtractSeconds(1);
    ASSERT_EQ(time.seconds(), 22);
    time.subtractSeconds(60);
    ASSERT_EQ(time.seconds(), 22);
    ASSERT_EQ(time.minutes(), 22);
    time.subtractSeconds(3600);
    ASSERT_EQ(time.seconds(), 22);
    ASSERT_EQ(time.minutes(), 22);
    ASSERT_EQ(time.hours(), 20);
}

TEST(DayTime, SubtractMilliseconds){
    DayTime time(21, 23, 23, 23);
    
    time.subtractMilliseconds(1);
    ASSERT_EQ(time.milliseconds(), 22);
    time.subtractMilliseconds(1000);
    ASSERT_EQ(time.milliseconds(), 22);
    ASSERT_EQ(time.seconds(), 22);
}

TEST(DayTime, SubtractMicroseconds){
    DayTime time(21, 23, 23, 23, 23);
    
    time.subtractMicroseconds(1);
    ASSERT_EQ(time.microseconds(), 22);
    time.subtractMicroseconds(1000);
    ASSERT_EQ(time.microseconds(), 22);
    ASSERT_EQ(time.milliseconds(), 22);
}

TEST(DayTime, SubtractNanoseconds){
    DayTime time(21, 23, 23, 23, 23, 23);
    
    time.subtractNanoseconds(1);
    ASSERT_EQ(time.nanoseconds(), 22);
    time.subtractNanoseconds(1000);
    ASSERT_EQ(time.nanoseconds(), 22);
    ASSERT_EQ(time.microseconds(), 22);
}

TEST(DayTime, LocalTime){
    auto time = DayTime::localTime();
    auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    
    struct tm t_time = *std::localtime(&time_t);
    
    ASSERT_EQ(time.hours(), t_time.tm_hour) << "Hours are not equal";
    ASSERT_EQ(time.minutes(), t_time.tm_min) << "Minutes are not equal";
    ASSERT_EQ(time.seconds(), t_time.tm_sec) << "Seconds are not equal";
}

TEST(DayTime, toString){
    auto time = DayTime::localTime();
    
    std::string s_time = time.toString(true);
    
    std::string test_time;
    if (time.hours() < 10){
        test_time += '0';
    }
    test_time += std::to_string(time.hours());
    test_time += ':';
    auto minutes = time.minutes();
    if (minutes < 10){
        test_time += '0';
    }
    test_time += std::to_string(minutes);
    test_time += ':';
    auto seconds = time.seconds();
    if (seconds < 10){
        test_time += '0';
    }
    test_time += std::to_string(time.seconds());
    test_time += "+03";
    
    ASSERT_EQ(s_time, test_time);
}

TEST(DayTime, OperatorEqual){
    DayTime left_time(23, 23, 23);
    DayTime right_time(23, 23, 23);
    
    ASSERT_TRUE(left_time == right_time);
    right_time.addHours(1);
    ASSERT_FALSE(left_time == right_time);
}

TEST(DayTime, OperatorLess){
    DayTime left_time(23, 23, 22);
    DayTime right_time(23, 23, 23);
    
    ASSERT_TRUE(left_time < right_time);
}

TEST(DayTime, OperatorPlus){
    DayTime left_time(23, 23, 23);
    DayTime right_time(23, 23, 23);
    
    auto day_time = left_time + right_time;
    DayTime test_time(22, 46, 46);
    ASSERT_TRUE(day_time == test_time);
}

TEST(DayTime, OperatorMinus){
    DayTime left_time(23, 23, 23);
    DayTime right_time(22, 24, 24);
    
    auto day_time = left_time - right_time;
    
    ASSERT_TRUE(day_time == DayTime(0, 58, 59));
}

TEST(DayTime, OperatorPlusAssign){
    DayTime left_time(23, 23, 23);
    DayTime right_time(23, 23, 23);
    
    auto day_time = left_time + right_time;
    left_time += right_time;
    ASSERT_TRUE(day_time == left_time);
}

TEST(DayTime, OperatorMinusAssign){
    DayTime left_time(23, 23, 23);
    DayTime right_time(22, 24, 24);
    
    auto day_time = left_time - right_time;
    left_time -= right_time;
    ASSERT_TRUE(day_time == left_time);
}

TEST(Date, DefaultConstructor){
    Date date;
    auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    
    struct tm t_time = *std::gmtime(&time_t);
    ASSERT_EQ(date.dayOfTheMonth(), t_time.tm_mday);
    ASSERT_EQ(date.dayOfTheWeek(), t_time.tm_wday);
    ASSERT_EQ(date.month(), t_time.tm_mon + 1);
    ASSERT_EQ(date.year(), t_time.tm_year + 1900);
}

TEST(Date, ExplicitConstructor){
    Date date(25, 8, 2021);

    ASSERT_EQ(date.dayOfTheMonth(), 25);
    ASSERT_EQ(date.dayOfTheWeek(), 3);
    ASSERT_EQ(date.month(), 8);
    ASSERT_EQ(date.year(), 2021);
    
    EXPECT_FALSE(date.isWeekend());
    
    date = Date(1, 1, 2024);
    ASSERT_EQ(date.dayOfTheMonth(), 1);
    ASSERT_EQ(date.dayOfTheWeek(), 1);
    ASSERT_EQ(date.month(), 1);
    ASSERT_EQ(date.year(), 2024);

    date = Date(1, 2, 2024);
    ASSERT_EQ(date.dayOfTheMonth(), 1);
    ASSERT_EQ(date.dayOfTheWeek(), 4);
    ASSERT_EQ(date.month(), 2);
    ASSERT_EQ(date.year(), 2024);

    date = Date(1, 3, 2024);
    ASSERT_EQ(date.dayOfTheMonth(), 1);
    ASSERT_EQ(date.dayOfTheWeek(), 5);
    ASSERT_EQ(date.month(), 3);
    ASSERT_EQ(date.year(), 2024);
    
    EXPECT_THROW(Date(32, 8, 2021), std::range_error);
    EXPECT_THROW(Date(30, 13, 2021), std::range_error);
    EXPECT_THROW(Date(1, 1, 1899), std::range_error);
    EXPECT_THROW(Date(30, 2, 2021), std::range_error);
    EXPECT_THROW(Date(31, 6, 2021), std::range_error);
}

TEST(Date, StringConstructor){
    Date date("2021-08-25");

    ASSERT_EQ(date.dayOfTheMonth(), 25);
    ASSERT_EQ(date.dayOfTheWeek(), 3);
    ASSERT_EQ(date.month(), 8);
    ASSERT_EQ(date.year(), 2021);
    
    EXPECT_FALSE(date.isWeekend());
    
    std::string invalid_date1 = "2021825";
    std::string invalid_date2 = "2021-0825";
    std::string invalid_date3 = "2021-8-25";
    
    EXPECT_THROW(Date{invalid_date1}, std::invalid_argument);
    EXPECT_THROW(Date{invalid_date2}, std::invalid_argument);
    EXPECT_THROW(Date{invalid_date3}, std::invalid_argument);
}

TEST(Date, OperatorEqual){
    Date date_one("2021-08-25");
    Date date_two(25, 8, 2021);
    EXPECT_TRUE(date_one == date_two);
}

TEST(Date, OperatorLess){
    Date date_one("2021-08-25");
    Date date_two(25, 8, 2021);
    EXPECT_FALSE(date_one < date_two);
    Date date_three(26, 8, 2021);
    EXPECT_TRUE(date_one < date_three);
    Date date_four(25, 9, 2021);
    EXPECT_TRUE(date_one < date_four);
    Date date_five(25, 8, 2022);
    EXPECT_TRUE(date_one < date_five);
}

TEST(Date, AddDays){
    Date date(25, 8, 2021);
    
    date.addDays(5);
    ASSERT_EQ(date.dayOfTheMonth(), 30);
    ASSERT_EQ(date.dayOfTheWeek(), 1);
    
    date.addDays(2);
    ASSERT_EQ(date.dayOfTheMonth(), 1);
    ASSERT_EQ(date.dayOfTheWeek(), 3);

    date.addDays(31);
    ASSERT_EQ(date.dayOfTheMonth(), 2);
    ASSERT_EQ(date.dayOfTheWeek(), 6);
    
    date = Date(28, 2, 2024);
    
    date.addDays(1);
    ASSERT_EQ(date.dayOfTheMonth(), 29);
    ASSERT_EQ(date.dayOfTheWeek(), 4);
}

TEST(Date, AddMonths){
    Date date(25, 8, 2021);
    
    date.addMonths(1);
    ASSERT_EQ(date.dayOfTheMonth(), 25);
    ASSERT_EQ(date.month(), 9);
    ASSERT_EQ(date.dayOfTheWeek(), 6);
    
    date = Date(31, 8, 2021);
    
    date.addMonths(1);
    ASSERT_EQ(date.dayOfTheMonth(), 30);
    ASSERT_EQ(date.month(), 9);
    ASSERT_EQ(date.dayOfTheWeek(), 4);
    
    date = Date(31, 1, 2024);
    date.addMonths(1);
    ASSERT_EQ(date.dayOfTheMonth(), 29);
    ASSERT_EQ(date.month(), 2);
    ASSERT_EQ(date.dayOfTheWeek(), 4);
    
    date = Date(31, 1, 2023);
    date.addMonths(1);
    ASSERT_EQ(date.dayOfTheMonth(), 28);
    ASSERT_EQ(date.month(), 2);
    ASSERT_EQ(date.dayOfTheWeek(), 2);
}

TEST(Date, AddYears){
    Date date(25, 8, 2021);
    
    date.addYears(1);
    ASSERT_EQ(date.dayOfTheMonth(), 25);
    ASSERT_EQ(date.year(), 2022);
    ASSERT_EQ(date.dayOfTheWeek(), 4);
    
    date = Date(29, 2, 2024);
    
    date.addYears(1);
    ASSERT_EQ(date.dayOfTheMonth(), 28);
    ASSERT_EQ(date.year(), 2025);
    ASSERT_EQ(date.dayOfTheWeek(), 5);
}

TEST(Date, SubtractDays){
    Date date(25, 8, 2021);
    
    date.subtractDays(5);
    ASSERT_EQ(date.dayOfTheMonth(), 20);
    ASSERT_EQ(date.dayOfTheWeek(), 5);
    
    date.subtractDays(31);
    ASSERT_EQ(date.dayOfTheMonth(), 20);
    ASSERT_EQ(date.dayOfTheWeek(), 2);
    
    date = Date(1, 3, 2024);
    
    date.subtractDays(1);
    ASSERT_EQ(date.dayOfTheMonth(), 29);
    ASSERT_EQ(date.dayOfTheWeek(), 4);
}

TEST(Date, SubtractMonths){
    Date date(25, 8, 2021);
    
    date.subtractMonths(1);
    ASSERT_EQ(date.dayOfTheMonth(), 25);
    ASSERT_EQ(date.month(), 7);
    ASSERT_EQ(date.dayOfTheWeek(), 0);
    
    date = Date(31, 7, 2021);
    
    date.subtractMonths(1);
    ASSERT_EQ(date.dayOfTheMonth(), 30);
    ASSERT_EQ(date.month(), 6);
    ASSERT_EQ(date.dayOfTheWeek(), 3);
    
    date = Date(31, 3, 2024);
    date.subtractMonths(1);
    ASSERT_EQ(date.dayOfTheMonth(), 29);
    ASSERT_EQ(date.month(), 2);
    ASSERT_EQ(date.dayOfTheWeek(), 4);

    date = Date(28, 3, 2024);
    date.subtractMonths(1);
    ASSERT_EQ(date.dayOfTheMonth(), 28);
    ASSERT_EQ(date.month(), 2);

    date = Date(28, 3, 2023);
    date.subtractMonths(1);
    ASSERT_EQ(date.dayOfTheMonth(), 28);
    ASSERT_EQ(date.month(), 2);
}

TEST(Date, SubtractYears){
    Date date(25, 8, 2021);
    
    date.subtractYears(1);
    ASSERT_EQ(date.dayOfTheMonth(), 25);
    ASSERT_EQ(date.year(), 2020);
    ASSERT_EQ(date.dayOfTheWeek(), 2);
    
    date = Date(29, 2, 2024);
    date.subtractYears(1);
    ASSERT_EQ(date.dayOfTheMonth(), 28);
    ASSERT_EQ(date.year(), 2023);
    ASSERT_EQ(date.dayOfTheWeek(), 2);

    date = Date(1, 3, 2024);
    date.subtractYears(1);
    ASSERT_EQ(date.dayOfTheMonth(), 1);
    ASSERT_EQ(date.month(), 3);
    ASSERT_EQ(date.year(), 2023);

    date = Date(28, 2, 2024);
    date.subtractYears(1);
    ASSERT_EQ(date.dayOfTheMonth(), 28);
    ASSERT_EQ(date.month(), 2);
    ASSERT_EQ(date.year(), 2023);

    date = Date(28, 2, 2025);
    date.subtractYears(1);
    ASSERT_EQ(date.dayOfTheMonth(), 28);
    ASSERT_EQ(date.month(), 2);
    ASSERT_EQ(date.year(), 2024);
    
    date = Date(1, 3, 2025);
    date.subtractYears(1);
    ASSERT_EQ(date.dayOfTheMonth(), 1);
    ASSERT_EQ(date.month(), 3);
    ASSERT_EQ(date.year(), 2024);
}

TEST(Date, IsWeekend){
    Date date(25, 8, 2021);
    
    ASSERT_FALSE(date.isWeekend());
    
    date = Date(4, 9, 2021);
    
    ASSERT_TRUE(date.isWeekend());
}

TEST(Date, ToString){
    Date date(25, 8, 2021);
    
    std::string s_date = date.toString();
    std::string t_date = "2021-08-25";
    
    ASSERT_EQ(s_date, t_date);
}

TEST(Date, IsLeapYear){
    ASSERT_FALSE(Date::isLeapYear(2021));
    
    ASSERT_TRUE(Date::isLeapYear(2024));
}