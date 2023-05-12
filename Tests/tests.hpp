#include "date_time.hpp"

#include <gtest/gtest.h>
using namespace tristan;
using namespace tristan::time;
using namespace tristan::date;
using namespace tristan::date_time;

TEST(Time, Default_constructor) {
    Time time;
    auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    struct tm t_time = *std::gmtime(&time_t);

    ASSERT_EQ(time.hours(), t_time.tm_hour) << "Hours are not equal";
    ASSERT_EQ(time.minutes(), t_time.tm_min) << "Minutes are not equal";
    ASSERT_EQ(time.seconds(), t_time.tm_sec) << "Seconds are not equal";
}

TEST(Time, EastTimeZone_constructor) {
    auto offset = tristan::TimeZone::EAST_2;
    Time l_time(offset);
    Time l_time_1;
    ASSERT_EQ(l_time.hours(), l_time_1.hours() + static_cast<int8_t>(offset));
}

TEST(Time, Minutes_constructor) {
    Time time(23, 23);

    ASSERT_EQ(time.hours(), 23) << "Hours are not equal";
    ASSERT_EQ(time.minutes(), 23) << "Minutes are not equal";
    ASSERT_EQ(time.precision(), Precision::MINUTES) << "Precision is not equal";
    EXPECT_THROW(Time(24, 23), std::range_error) << "Exception on hours range expected";
    EXPECT_THROW(Time(23, 60), std::range_error) << "Exception on minutes range expected";
}

TEST(Time, Seconds_constructor) {
    Time time(23, 23, 23);

    ASSERT_EQ(time.seconds(), 23) << "Seconds are not equal";
    ASSERT_EQ(time.precision(), Precision::SECONDS) << "Precision is not equal";
    EXPECT_THROW(Time(23, 23, 60), std::range_error) << "Exception on seconds range expected";
}

TEST(Time, Milliseconds_constructor) {
    Time time(23, 23, 23, 23);

    ASSERT_EQ(time.milliseconds(), 23) << "Milliseconds are not equal";
    ASSERT_EQ(time.precision(), Precision::MILLISECONDS) << "Precision is not equal";
    EXPECT_THROW(Time(23, 23, 23, 1000), std::range_error) << "Exception on milliseconds range expected";
}

TEST(Time, Microseconds_constructor) {
    Time time(23, 23, 23, 23, 23);

    ASSERT_EQ(time.microseconds(), 23) << "Microseconds are not equal";
    ASSERT_EQ(time.precision(), Precision::MICROSECONDS) << "Precision is not equal";
    EXPECT_THROW(Time(23, 23, 23, 23, 1000), std::range_error) << "Exception on microseconds range expected";
}

TEST(Time, Nanoseconds_constructor) {
    Time time(23, 23, 23, 23, 23, 23);

    ASSERT_EQ(time.nanoseconds(), 23) << "Nanoseconds are not equal";
    ASSERT_EQ(time.precision(), Precision::NANOSECONDS) << "Precision is not equal";
    EXPECT_THROW(Time(23, 23, 23, 23, 23, 1000), std::range_error) << "Exception on nanoseconds range expected";
}

TEST(Time, String_constructor) {
    std::string s_time = "23:23:23.023.023.023+02";
    std::string invalid_time1 = "2a:23:23:023:023:023";
    std::string invalid_time2 = "23:23:23:0023:023:023";
    std::string invalid_time3 = "23:23:23-023:023:023";
    Time time(s_time);

    ASSERT_EQ(time.hours(), 23) << "Hours are not equal";
    ASSERT_EQ(time.minutes(), 23) << "Minutes are not equal";
    ASSERT_EQ(time.seconds(), 23) << "Seconds are not equal";
    ASSERT_EQ(time.milliseconds(), 23) << "Milliseconds are not equal";
    ASSERT_EQ(time.microseconds(), 23) << "Microseconds are not equal";
    ASSERT_EQ(time.nanoseconds(), 23) << "Nanoseconds are not equal";
    ASSERT_EQ(time.precision(), Precision::NANOSECONDS);
    ASSERT_EQ(time.offset(), TimeZone::EAST_2);

    EXPECT_THROW(Time{invalid_time1}, std::invalid_argument) << "Exception on invalid_time1 expected";
    EXPECT_THROW(Time{invalid_time2}, std::invalid_argument) << "Exception on invalid_time2 range expected";
    EXPECT_THROW(Time{invalid_time3}, std::invalid_argument) << "Exception on invalid_time3 range expected";
}

TEST(Time, AddHours) {
    Time time(21, 23);

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

TEST(Time, AddMinutes) {
    Time time(21, 23);

    time.addMinutes(1);
    ASSERT_EQ(time.minutes(), 24);
    time.addMinutes(60);
    ASSERT_EQ(time.minutes(), 24);
    ASSERT_EQ(time.hours(), 22);
}

TEST(Time, AddSeconds) {
    Time time(21, 23, 23);

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

TEST(Time, AddMilliseconds) {
    Time time(21, 23, 23, 23);

    time.addMilliseconds(1);
    ASSERT_EQ(time.milliseconds(), 24);
    time.addMilliseconds(1000);
    ASSERT_EQ(time.milliseconds(), 24);
    ASSERT_EQ(time.seconds(), 24);
}

TEST(Time, AddMicroseconds) {
    Time time(21, 23, 23, 23, 23);

    time.addMicroseconds(1);
    ASSERT_EQ(time.microseconds(), 24);
    time.addMicroseconds(1000);
    ASSERT_EQ(time.microseconds(), 24);
    ASSERT_EQ(time.milliseconds(), 24);
}

TEST(Time, AddNanoseconds) {
    Time time(21, 23, 23, 23, 23, 23);

    time.addNanoseconds(1);
    ASSERT_EQ(time.nanoseconds(), 24);
    time.addNanoseconds(1000);
    ASSERT_EQ(time.nanoseconds(), 24);
    ASSERT_EQ(time.microseconds(), 24);
}

TEST(Time, SubtractHours) {
    Time time(21, 23);

    time.subtractHours(1);
    ASSERT_EQ(time.hours(), 20);
    time.subtractHours(2);
    ASSERT_EQ(time.hours(), 18);
    time.subtractHours(24);
    ASSERT_EQ(time.hours(), 18);
    time.subtractHours(25);
    ASSERT_EQ(time.hours(), 17);
}

TEST(Time, SubtractMinutes) {
    Time time(21, 23);

    time.subtractMinutes(1);
    ASSERT_EQ(time.minutes(), 22);
    time.subtractMinutes(60);
    ASSERT_EQ(time.minutes(), 22);
    ASSERT_EQ(time.hours(), 20);
}

TEST(Time, SubtractSeconds) {
    Time time(21, 23, 23);

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

TEST(Time, SubtractMilliseconds) {
    Time time(21, 23, 23, 23);

    time.subtractMilliseconds(1);
    ASSERT_EQ(time.milliseconds(), 22);
    time.subtractMilliseconds(1000);
    ASSERT_EQ(time.milliseconds(), 22);
    ASSERT_EQ(time.seconds(), 22);
}

TEST(Time, SubtractMicroseconds) {
    Time time(21, 23, 23, 23, 23);

    time.subtractMicroseconds(1);
    ASSERT_EQ(time.microseconds(), 22);
    time.subtractMicroseconds(1000);
    ASSERT_EQ(time.microseconds(), 22);
    ASSERT_EQ(time.milliseconds(), 22);
}

TEST(Time, SubtractNanoseconds) {
    Time time(21, 23, 23, 23, 23, 23);

    time.subtractNanoseconds(1);
    ASSERT_EQ(time.nanoseconds(), 22);
    time.subtractNanoseconds(1000);
    ASSERT_EQ(time.nanoseconds(), 22);
    ASSERT_EQ(time.microseconds(), 22);
}

TEST(Time, LocalTime) {
    auto time = Time::localTime();
    auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    struct tm t_time = *std::localtime(&time_t);

    ASSERT_EQ(time.hours(), t_time.tm_hour) << "Hours are not equal";
    ASSERT_EQ(time.minutes(), t_time.tm_min) << "Minutes are not equal";
    ASSERT_EQ(time.seconds(), t_time.tm_sec) << "Seconds are not equal";
}

TEST(Time, toString) {
    auto time = Time::localTime();

    std::string s_time = time.toString();

    std::string test_time;
    if (time.hours() < 10) {
        test_time += '0';
    }
    test_time += std::to_string(time.hours());
    test_time += ':';
    auto minutes = time.minutes();
    if (minutes < 10) {
        test_time += '0';
    }
    test_time += std::to_string(minutes);
    test_time += ':';
    auto seconds = time.seconds();
    if (seconds < 10) {
        test_time += '0';
    }
    test_time += std::to_string(time.seconds());
    test_time += "+02";

    ASSERT_EQ(s_time, test_time);
}

TEST(Time, OperatorEqual) {
    Time left_time(23, 23, 23);
    Time right_time(23, 23, 23);

    ASSERT_TRUE(left_time == right_time);
    right_time.addHours(1);
    ASSERT_FALSE(left_time == right_time);
}

TEST(Time, OperatorLess) {
    Time left_time(23, 23, 22);
    Time right_time(23, 23, 23);

    ASSERT_TRUE(left_time < right_time);
}

TEST(Time, OperatorPlus) {
    Time left_time(23, 23, 23);
    Time right_time(23, 23, 23);

    auto day_time = left_time + right_time;
    Time test_time(22, 46, 46);
    ASSERT_TRUE(day_time == test_time);
}

TEST(Time, OperatorMinus) {
    Time left_time(23, 23, 23);
    Time right_time(22, 24, 24);

    auto day_time = left_time - right_time;

    ASSERT_TRUE(day_time == Time(0, 58, 59));
}

TEST(Time, OperatorPlusAssign) {
    Time left_time(23, 23, 23);
    Time right_time(23, 23, 23);

    auto day_time = left_time + right_time;
    left_time += right_time;
    ASSERT_TRUE(day_time == left_time);
}

TEST(Time, OperatorMinusAssign) {
    Time left_time(23, 23, 23);
    Time right_time(22, 24, 24);

    auto day_time = left_time - right_time;
    left_time -= right_time;
    ASSERT_TRUE(day_time == left_time);
}

TEST(Date, DefaultConstructor) {
    Date date;
    auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    struct tm t_time = *std::gmtime(&time_t);
    ASSERT_EQ(date.dayOfTheMonth(), t_time.tm_mday);
    ASSERT_EQ(date.dayOfTheWeek(), t_time.tm_wday);
    ASSERT_EQ(date.month(), t_time.tm_mon + 1);
    ASSERT_EQ(date.year(), t_time.tm_year + 1900);
}

TEST(Date, ExplicitConstructor) {
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

TEST(Date, StringConstructor) {
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

TEST(Date, OperatorEqual) {
    Date date_one("2021-08-25");
    Date date_two(25, 8, 2021);
    EXPECT_TRUE(date_one == date_two);
}

TEST(Date, OperatorLess) {
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

TEST(Date, AddDays) {
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

TEST(Date, AddMonths) {
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

TEST(Date, AddYears) {
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

TEST(Date, SubtractDays) {
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

TEST(Date, SubtractMonths) {
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

TEST(Date, SubtractYears) {
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

TEST(Date, IsWeekend) {
    Date date(25, 8, 2021);

    ASSERT_FALSE(date.isWeekend());

    date = Date(4, 9, 2021);

    ASSERT_TRUE(date.isWeekend());
}

TEST(Date, ToString) {
    Date date(25, 8, 2021);

    std::string s_date = date.toString();
    std::string t_date = "2021-08-25";

    ASSERT_EQ(s_date, t_date);
}

TEST(Date, IsLeapYear) {
    ASSERT_FALSE(Date::isLeapYear(2021));

    ASSERT_TRUE(Date::isLeapYear(2024));
}

TEST(DateTime, StringConstructor) {
    std::string date = "20210101T10:10:10+02";

    DateTime d_date(date);

    ASSERT_EQ(d_date.date().year(), 2021);
    ASSERT_EQ(d_date.date().month(), 1);
    ASSERT_EQ(d_date.date().dayOfTheMonth(), 1);
    ASSERT_EQ(d_date.time().hours(), 10);
    ASSERT_EQ(d_date.time().minutes(), 10);
    ASSERT_EQ(d_date.time().seconds(), 10);
    ASSERT_EQ(d_date.time().offset(), TimeZone::EAST_2);
}

TEST(DateTime, AddSeconds_55){
    std::string date = "20210101T23:59:10+02";

    DateTime l_date_time(date);

    l_date_time.addSeconds(55);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 2);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 0);
    ASSERT_EQ(l_date_time.time().seconds(), 5);
}

TEST(DateTime, AddSeconds_3600){
    std::string date = "20210101T23:10:10+02";

    DateTime l_date_time(date);

    l_date_time.addSeconds(3600);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 2);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, AddMinutes_45){
    std::string date = "20210101T23:10:10+02";

    DateTime l_date_time(date);

    l_date_time.addMinutes(45);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 1);
    ASSERT_EQ(l_date_time.time().hours(), 23);
    ASSERT_EQ(l_date_time.time().minutes(), 55);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, AddMinutes_50){
    std::string date = "20210101T23:10:10+02";

    DateTime l_date_time(date);

    l_date_time.addMinutes(50);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 2);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 0);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, AddMinutes_55){
    std::string date = "20210101T23:10:10+02";

    DateTime l_date_time(date);

    l_date_time.addMinutes(55);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 2);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 5);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, AddMinutes_60){
    std::string date = "20210101T23:10:10+02";

    DateTime l_date_time(date);

    l_date_time.addMinutes(60);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 2);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, AddMinutes_1440){
    std::string date = "20210101T00:10:10+02";

    DateTime l_date_time(date);

    l_date_time.addMinutes(1440);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 2);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, AddMinutes_2880){
    std::string date = "20210101T00:10:10+02";

    DateTime l_date_time(date);

    l_date_time.addMinutes(2880);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 3);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, AddHours_1){
    std::string date = "20210101T23:10:10+02";

    DateTime l_date_time(date);

    l_date_time.addHours(1);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 2);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, AddHours_24){
    std::string date = "20210101T00:10:10+02";

    DateTime l_date_time(date);

    l_date_time.addHours(24);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 2);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, AddHours_48){
    std::string date = "20210101T00:10:10+02";

    DateTime l_date_time(date);

    l_date_time.addHours(48);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 3);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, SubtractSeconds_55){
    std::string date = "20210102T00:00:05+02";

    DateTime l_date_time(date);

    l_date_time.subtractSeconds(55);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 1);
    ASSERT_EQ(l_date_time.time().hours(), 23);
    ASSERT_EQ(l_date_time.time().minutes(), 59);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, SubtractSeconds_3600){
    std::string date = "20210102T00:10:10+02";

    DateTime l_date_time(date);

    l_date_time.subtractSeconds(3600);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 1);
    ASSERT_EQ(l_date_time.time().hours(), 23);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, SubtractMinutes_45){
    std::string date = "20210101T23:55:10+02";

    DateTime l_date_time(date);

    l_date_time.subtractMinutes(45);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 1);
    ASSERT_EQ(l_date_time.time().hours(), 23);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, SubtractMinutes_50){
    std::string date = "20210102T00:00:10+02";

    DateTime l_date_time(date);

    l_date_time.subtractMinutes(50);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 1);
    ASSERT_EQ(l_date_time.time().hours(), 23);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, SubtractMinutes_55){
    std::string date = "20210102T00:05:10+02";

    DateTime l_date_time(date);

    l_date_time.subtractMinutes(55);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 1);
    ASSERT_EQ(l_date_time.time().hours(), 23);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, SubtractMinutes_60){
    std::string date = "20210102T00:10:10+02";

    DateTime l_date_time(date);

    l_date_time.subtractMinutes(60);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 1);
    ASSERT_EQ(l_date_time.time().hours(), 23);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, SubtractMinutes_1440){
    std::string date = "20210102T00:10:10+02";

    DateTime l_date_time(date);

    l_date_time.subtractMinutes(1440);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 1);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, SubtractMinutes_2880){
    std::string date = "20210103T00:10:10+02";

    DateTime l_date_time(date);

    l_date_time.subtractMinutes(2880);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 1);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, SubtractHours_1){
    std::string date = "20210102T00:10:10+02";

    DateTime l_date_time(date);

    l_date_time.subtractHours(1);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 1);
    ASSERT_EQ(l_date_time.time().hours(), 23);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, SubtractHours_24){
    std::string date = "20210102T00:10:10+02";

    DateTime l_date_time(date);

    l_date_time.subtractHours(24);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 1);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}

TEST(DateTime, SubtractHours_48){
    std::string date = "20210103T00:10:10+02";

    DateTime l_date_time(date);

    l_date_time.subtractHours(48);
    ASSERT_EQ(l_date_time.date().dayOfTheMonth(), 1);
    ASSERT_EQ(l_date_time.time().hours(), 0);
    ASSERT_EQ(l_date_time.time().minutes(), 10);
    ASSERT_EQ(l_date_time.time().seconds(), 10);
}