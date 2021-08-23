#include "time.hpp"

#include <gtest/gtest.h>

using namespace tristan::time;

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

TEST(DayTime, SubstractHours){
    DayTime time(21, 23);
    
    time.substractHours(1);
    ASSERT_EQ(time.hours(), 20);
    time.substractHours(2);
    ASSERT_EQ(time.hours(), 18);
    time.substractHours(24);
    ASSERT_EQ(time.hours(), 18);
    time.substractHours(25);
    ASSERT_EQ(time.hours(), 17);
}

TEST(DayTime, SubstractMinutes){
    DayTime time(21, 23);
    
    time.substractMinutes(1);
    ASSERT_EQ(time.minutes(), 22);
    time.substractMinutes(60);
    ASSERT_EQ(time.minutes(), 22);
    ASSERT_EQ(time.hours(), 20);
}

TEST(DayTime, SubstractSeconds){
    DayTime time(21, 23, 23);
    
    time.substractSeconds(1);
    ASSERT_EQ(time.seconds(), 22);
    time.substractSeconds(60);
    ASSERT_EQ(time.seconds(), 22);
    ASSERT_EQ(time.minutes(), 22);
    time.substractSeconds(3600);
    ASSERT_EQ(time.seconds(), 22);
    ASSERT_EQ(time.minutes(), 22);
    ASSERT_EQ(time.hours(), 20);
}

TEST(DayTime, SubstractMilliseconds){
    DayTime time(21, 23, 23, 23);
    
    time.substractMilliseconds(1);
    ASSERT_EQ(time.milliseconds(), 22);
    time.substractMilliseconds(1000);
    ASSERT_EQ(time.milliseconds(), 22);
    ASSERT_EQ(time.seconds(), 22);
}

TEST(DayTime, SubstractMicroseconds){
    DayTime time(21, 23, 23, 23, 23);
    
    time.substractMicroseconds(1);
    ASSERT_EQ(time.microseconds(), 22);
    time.substractMicroseconds(1000);
    ASSERT_EQ(time.microseconds(), 22);
    ASSERT_EQ(time.milliseconds(), 22);
}

TEST(DayTime, SubstractNanoseconds){
    DayTime time(21, 23, 23, 23, 23, 23);
    
    time.substractNanoseconds(1);
    ASSERT_EQ(time.nanoseconds(), 22);
    time.substractNanoseconds(1000);
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
    
    std::string test_time = std::to_string(time.hours());
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
