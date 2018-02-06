#include <gtest/gtest.h>
#include "test_utility.h"
#include "tiex_match.h"

using namespace tiex;
using namespace tiex::internal;

static bool TestMakeBoundaryTime(const Boundary& boundary, const std::tm& tm, std::time_t expected_time) {
    std::time_t actual_time = 0;
    bool is_succeeded = MakeBoundaryTime(boundary, tm, actual_time);
    if (! is_succeeded) {
        return false;
    }
    return actual_time == expected_time;
}


TEST(Match, MakeBoundaryTime_Second) {
 
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        for (auto round : { true, false }) {
            Boundary boundary;
            boundary.unit = Unit::Second;
            boundary.round = round;
            boundary.value = boundary_value;
            if (! TestMakeBoundaryTime(boundary, tm, expected_time)) {
                return false;
            }
        }
        return true;
    };
    
    auto tm = MakeTm(2017, 12, 3, 15, 33, 45);
    ASSERT_TRUE(test(-60, tm, MakeTime(2017, 12, 3, 15, 32, 45)));
    ASSERT_TRUE(test(-2,  tm, MakeTime(2017, 12, 3, 15, 33, 43)));
    ASSERT_TRUE(test(-1,  tm, MakeTime(2017, 12, 3, 15, 33, 44)));
    ASSERT_TRUE(test(0,   tm, MakeTime(2017, 12, 3, 15, 33, 45)));
    ASSERT_TRUE(test(1,   tm, MakeTime(2017, 12, 3, 15, 33, 46)));
    ASSERT_TRUE(test(2,   tm, MakeTime(2017, 12, 3, 15, 33, 47)));
    ASSERT_TRUE(test(60,  tm, MakeTime(2017, 12, 3, 15, 34, 45)));
}


TEST(Match, MakeBoundaryTime_MinuteNoRound) {
    
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        Boundary boundary;
        boundary.unit = Unit::Minute;
        boundary.round = false;
        boundary.value = boundary_value;
        return TestMakeBoundaryTime(boundary, tm, expected_time);
    };
    
    auto tm = MakeTm(2018, 1, 19, 20, 34, 12);
    ASSERT_TRUE(test(-60,  tm, MakeTime(2018, 1, 19, 19, 34, 12)));
    ASSERT_TRUE(test(-2,   tm, MakeTime(2018, 1, 19, 20, 32, 12)));
    ASSERT_TRUE(test(-1,   tm, MakeTime(2018, 1, 19, 20, 33, 12)));
    ASSERT_TRUE(test(0,    tm, MakeTime(2018, 1, 19, 20, 34, 12)));
    ASSERT_TRUE(test(1,    tm, MakeTime(2018, 1, 19, 20, 35, 12)));
    ASSERT_TRUE(test(2,    tm, MakeTime(2018, 1, 19, 20, 36, 12)));
    ASSERT_TRUE(test(60,   tm, MakeTime(2018, 1, 19, 21, 34, 12)));
}


TEST(Match, MakeBoundaryTime_MinuteRound) {
    
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        Boundary boundary;
        boundary.unit = Unit::Minute;
        boundary.round = true;
        boundary.value = boundary_value;
        return TestMakeBoundaryTime(boundary, tm, expected_time);
    };
    
    auto tm = MakeTm(2018, 1, 19, 20, 34, 12);
    ASSERT_TRUE(test(-60, tm, MakeTime(2018, 1, 19, 19, 35, 0)));
    ASSERT_TRUE(test(-2,  tm, MakeTime(2018, 1, 19, 20, 33, 0)));
    ASSERT_TRUE(test(-1,  tm, MakeTime(2018, 1, 19, 20, 34, 0)));
    ASSERT_TRUE(test(0,   tm, MakeTime(2018, 1, 19, 20, 34, 12)));
    ASSERT_TRUE(test(1,   tm, MakeTime(2018, 1, 19, 20, 35, 0)));
    ASSERT_TRUE(test(2,   tm, MakeTime(2018, 1, 19, 20, 36, 0)));
    ASSERT_TRUE(test(60,  tm, MakeTime(2018, 1, 19, 21, 34, 0)));
}


TEST(Match, MakeBoundaryTime_HourNoRound) {
    
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        Boundary boundary;
        boundary.unit = Unit::Hour;
        boundary.round = false;
        boundary.value = boundary_value;
        return TestMakeBoundaryTime(boundary, tm, expected_time);
    };
    
    auto tm = MakeTm(2018, 1, 25, 13, 16, 28);
    ASSERT_TRUE(test(-24,  tm, MakeTime(2018, 1, 24, 13, 16, 28)));
    ASSERT_TRUE(test(-2,   tm, MakeTime(2018, 1, 25, 11, 16, 28)));
    ASSERT_TRUE(test(-1,   tm, MakeTime(2018, 1, 25, 12, 16, 28)));
    ASSERT_TRUE(test(0,    tm, MakeTime(2018, 1, 25, 13, 16, 28)));
    ASSERT_TRUE(test(1,    tm, MakeTime(2018, 1, 25, 14, 16, 28)));
    ASSERT_TRUE(test(2,    tm, MakeTime(2018, 1, 25, 15, 16, 28)));
    ASSERT_TRUE(test(24,   tm, MakeTime(2018, 1, 26, 13, 16, 28)));
}


TEST(Match, MakeBoundaryTime_HourRound) {
    
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        Boundary boundary;
        boundary.unit = Unit::Hour;
        boundary.round = true;
        boundary.value = boundary_value;
        return TestMakeBoundaryTime(boundary, tm, expected_time);
    };
    
    auto tm = MakeTm(2018, 1, 25, 13, 16, 28);
    ASSERT_TRUE(test(-24,  tm, MakeTime(2018, 1, 24, 14, 0, 0)));
    ASSERT_TRUE(test(-2,   tm, MakeTime(2018, 1, 25, 12, 0, 0)));
    ASSERT_TRUE(test(-1,   tm, MakeTime(2018, 1, 25, 13, 0, 0)));
    ASSERT_TRUE(test(0,    tm, MakeTime(2018, 1, 25, 13, 16, 28)));
    ASSERT_TRUE(test(1,    tm, MakeTime(2018, 1, 25, 14, 0, 0)));
    ASSERT_TRUE(test(2,    tm, MakeTime(2018, 1, 25, 15, 0, 0)));
    ASSERT_TRUE(test(24,   tm, MakeTime(2018, 1, 26, 13, 0, 0)));
}


TEST(Match, MakeBoundaryTime_DayNoRound) {
    
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        Boundary boundary;
        boundary.unit = Unit::Day;
        boundary.round = false;
        boundary.value = boundary_value;
        return TestMakeBoundaryTime(boundary, tm, expected_time);
    };
    
    auto tm = MakeTm(2018, 1, 25, 13, 24, 3);
    ASSERT_TRUE(test(-30, tm, MakeTime(2017, 12, 26, 13, 24, 3)));
    ASSERT_TRUE(test(-2,  tm, MakeTime(2018, 1, 23, 13, 24, 3)));
    ASSERT_TRUE(test(-1,  tm, MakeTime(2018, 1, 24, 13, 24, 3)));
    ASSERT_TRUE(test(0,   tm, MakeTime(2018, 1, 25, 13, 24, 3)));
    ASSERT_TRUE(test(1,   tm, MakeTime(2018, 1, 26, 13, 24, 3)));
    ASSERT_TRUE(test(2,   tm, MakeTime(2018, 1, 27, 13, 24, 3)));
    ASSERT_TRUE(test(30,  tm, MakeTime(2018, 2, 24, 13, 24, 3)));
    
    ASSERT_TRUE(test(-1, MakeTm(2016, 3, 1, 1, 2, 3), MakeTime(2016, 2, 29, 1, 2, 3)));
    ASSERT_TRUE(test(-1, MakeTm(2018, 3, 1, 1, 2, 3), MakeTime(2018, 2, 28, 1, 2, 3)));
    ASSERT_TRUE(test(-1, MakeTm(2018, 4, 1, 1, 2, 3), MakeTime(2018, 3, 31, 1, 2, 3)));
    ASSERT_TRUE(test(-1, MakeTm(2018, 5, 1, 1, 2, 3), MakeTime(2018, 4, 30, 1, 2, 3)));
    
    ASSERT_TRUE(test(1, MakeTm(2018, 2, 28, 1, 2, 3), MakeTime(2018, 3, 1, 1, 2, 3)));
    ASSERT_TRUE(test(1, MakeTm(2016, 2, 29, 1, 2, 3), MakeTime(2016, 3, 1, 1, 2, 3)));
    ASSERT_TRUE(test(1, MakeTm(2018, 3, 31, 1, 2, 3), MakeTime(2018, 4, 1, 1, 2, 3)));
    ASSERT_TRUE(test(1, MakeTm(2018, 4, 30, 1, 2, 3), MakeTime(2018, 5, 1, 1, 2, 3)));
}


TEST(Match, MakeBoundaryTime_DayRound) {
    
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        Boundary boundary;
        boundary.unit = Unit::Day;
        boundary.round = true;
        boundary.value = boundary_value;
        return TestMakeBoundaryTime(boundary, tm, expected_time);
    };
    
    auto tm = MakeTm(2018, 1, 25, 13, 24, 3);
    ASSERT_TRUE(test(-30, tm, MakeTime(2017, 12, 27, 0, 0, 0)));
    ASSERT_TRUE(test(-2,  tm, MakeTime(2018, 1, 24, 0, 0, 0)));
    ASSERT_TRUE(test(-1,  tm, MakeTime(2018, 1, 25, 0, 0, 0)));
    ASSERT_TRUE(test(0,   tm, MakeTime(2018, 1, 25, 13, 24, 3)));
    ASSERT_TRUE(test(1,   tm, MakeTime(2018, 1, 26, 0, 0, 0)));
    ASSERT_TRUE(test(2,   tm, MakeTime(2018, 1, 27, 0, 0, 0)));
    ASSERT_TRUE(test(30,  tm, MakeTime(2018, 2, 24, 0, 0, 0)));
    
    ASSERT_TRUE(test(-1, MakeTm(2016, 3, 1, 1, 2, 3), MakeTime(2016, 3, 1, 0, 0, 0)));
    ASSERT_TRUE(test(-1, MakeTm(2018, 3, 1, 1, 2, 3), MakeTime(2018, 3, 1, 0, 0, 0)));
    ASSERT_TRUE(test(-1, MakeTm(2018, 4, 1, 1, 2, 3), MakeTime(2018, 4, 1, 0, 0, 0)));
    ASSERT_TRUE(test(-1, MakeTm(2018, 5, 1, 1, 2, 3), MakeTime(2018, 5, 1, 0, 0, 0)));
    
    ASSERT_TRUE(test(1, MakeTm(2018, 2, 28, 1, 2, 3), MakeTime(2018, 3, 1, 0, 0, 0)));
    ASSERT_TRUE(test(1, MakeTm(2016, 2, 29, 1, 2, 3), MakeTime(2016, 3, 1, 0, 0, 0)));
    ASSERT_TRUE(test(1, MakeTm(2018, 3, 31, 1, 2, 3), MakeTime(2018, 4, 1, 0, 0, 0)));
    ASSERT_TRUE(test(1, MakeTm(2018, 4, 30, 1, 2, 3), MakeTime(2018, 5, 1, 0, 0, 0)));
}


TEST(Match, MakeBoundaryTime_MonthNoRound) {
    
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        Boundary boundary;
        boundary.unit = Unit::Month;
        boundary.round = false;
        boundary.value = boundary_value;
        return TestMakeBoundaryTime(boundary, tm, expected_time);
    };
    
    auto tm = MakeTm(2018, 1, 26, 12, 21, 49);
    ASSERT_TRUE(test(-12, tm, MakeTime(2017, 1, 26, 12, 21, 49)));
    ASSERT_TRUE(test(-2,  tm, MakeTime(2017, 11, 26, 12, 21, 49)));
    ASSERT_TRUE(test(-1,  tm, MakeTime(2017, 12, 26, 12, 21, 49)));
    ASSERT_TRUE(test(0,   tm, MakeTime(2018, 1, 26, 12, 21, 49)));
    ASSERT_TRUE(test(1,   tm, MakeTime(2018, 2, 26, 12, 21, 49)));
    ASSERT_TRUE(test(2,   tm, MakeTime(2018, 3, 26, 12, 21, 49)));
    ASSERT_TRUE(test(12,  tm, MakeTime(2019, 1, 26, 12, 21, 49)));
}


TEST(Match, MakeBoundaryTime_MonthRound) {
    
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        Boundary boundary;
        boundary.unit = Unit::Month;
        boundary.round = true;
        boundary.value = boundary_value;
        return TestMakeBoundaryTime(boundary, tm, expected_time);
    };
    
    auto tm = MakeTm(2018, 1, 26, 12, 21, 49);
    ASSERT_TRUE(test(-12, tm, MakeTime(2017, 2, 1, 0, 0, 0)));
    ASSERT_TRUE(test(-2,  tm, MakeTime(2017, 12, 1, 0, 0, 0)));
    ASSERT_TRUE(test(-1,  tm, MakeTime(2018, 1, 1, 0, 0, 0)));
    ASSERT_TRUE(test(0,   tm, MakeTime(2018, 1, 26, 12, 21, 49)));
    ASSERT_TRUE(test(1,   tm, MakeTime(2018, 2, 1, 0, 0, 0)));
    ASSERT_TRUE(test(2,   tm, MakeTime(2018, 3, 1, 0, 0, 0)));
    ASSERT_TRUE(test(12,  tm, MakeTime(2019, 1, 1, 0, 0, 0)));
}


TEST(Match, MakeBoundaryTime_YearNoRound) {
    
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        Boundary boundary;
        boundary.unit = Unit::Year;
        boundary.round = false;
        boundary.value = boundary_value;
        return TestMakeBoundaryTime(boundary, tm, expected_time);
    };
    
    auto tm = MakeTm(2018, 1, 26, 12, 30, 33);
    ASSERT_TRUE(test(-2, tm, MakeTime(2016, 1, 26, 12, 30, 33)));
    ASSERT_TRUE(test(-1, tm, MakeTime(2017, 1, 26, 12, 30, 33)));
    ASSERT_TRUE(test(0,  tm, MakeTime(2018, 1, 26, 12, 30, 33)));
    ASSERT_TRUE(test(1,  tm, MakeTime(2019, 1, 26, 12, 30, 33)));
    ASSERT_TRUE(test(2,  tm, MakeTime(2020, 1, 26, 12, 30, 33)));
}


TEST(Match, MakeBoundaryTime_YearRound) {
    
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        Boundary boundary;
        boundary.unit = Unit::Year;
        boundary.round = true;
        boundary.value = boundary_value;
        return TestMakeBoundaryTime(boundary, tm, expected_time);
    };
    
    auto tm = MakeTm(2018, 1, 26, 12, 30, 33);
    ASSERT_TRUE(test(-2, tm, MakeTime(2017, 1, 1, 0, 0, 0)));
    ASSERT_TRUE(test(-1, tm, MakeTime(2018, 1, 1, 0, 0, 0)));
    ASSERT_TRUE(test(0,  tm, MakeTime(2018, 1, 26, 12, 30, 33)));
    ASSERT_TRUE(test(1,  tm, MakeTime(2019, 1, 1, 0, 0, 0)));
    ASSERT_TRUE(test(2,  tm, MakeTime(2020, 1, 1, 0, 0, 0)));
}


TEST(Match, MakeBoundaryTime_WeekNoRound) {
    
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        Boundary boundary;
        boundary.unit = Unit::Week;
        boundary.round = false;
        boundary.value = boundary_value;
        return TestMakeBoundaryTime(boundary, tm, expected_time);
    };
    
    auto tm = MakeTm(2018, 1, 26, 12, 35, 24);
    ASSERT_TRUE(test(-2, tm, MakeTime(2018, 1, 12, 12, 35, 24)));
    ASSERT_TRUE(test(-1, tm, MakeTime(2018, 1, 19, 12, 35, 24)));
    ASSERT_TRUE(test(0,  tm, MakeTime(2018, 1, 26, 12, 35, 24)));
    ASSERT_TRUE(test(1,  tm, MakeTime(2018, 2, 2, 12, 35, 24)));
    ASSERT_TRUE(test(2,  tm, MakeTime(2018, 2, 9, 12, 35, 24)));
}


TEST(Match, MakeBoundaryTime_WeekRound) {
    
    auto test = [](int boundary_value, const std::tm& tm, std::time_t expected_time) {
        Boundary boundary;
        boundary.unit = Unit::Week;
        boundary.round = true;
        boundary.value = boundary_value;
        return TestMakeBoundaryTime(boundary, tm, expected_time);
    };
    
    auto tm = MakeTm(2018, 1, 26, 12, 35, 24);
    ASSERT_TRUE(test(-2, tm, MakeTime(2018, 1, 14, 0, 0, 0)));
    ASSERT_TRUE(test(-1, tm, MakeTime(2018, 1, 21, 0, 0, 0)));
    ASSERT_TRUE(test(0,  tm, MakeTime(2018, 1, 26, 12, 35, 24)));
    ASSERT_TRUE(test(1,  tm, MakeTime(2018, 1, 28, 0, 0, 0)));
    ASSERT_TRUE(test(2,  tm, MakeTime(2018, 2, 4, 0, 0, 0)));
}


TEST(Match, MakeBoundaryTime_Limit) {
    
    auto test = [](int boundary_value, std::time_t expected_time) {
        
        auto units = {
            Unit::Second, Unit::Minute, Unit::Hour, Unit::Day, Unit::Week, Unit::Month, Unit::Year,
        };
        
        for (auto unit : units) {
            for (auto round : { true, false }) {
                
                Boundary boundary;
                boundary.value = boundary_value;
                boundary.unit = unit;
                boundary.round = round;
                
                std::tm tm = { 0 };
                if (! TestMakeBoundaryTime(boundary, tm, expected_time)) {
                    return false;
                }
            }
        }
        
        return true;
    };
    
    ASSERT_TRUE(test(std::numeric_limits<int>::min(), std::numeric_limits<std::time_t>::min()));
    ASSERT_TRUE(test(std::numeric_limits<int>::max(), std::numeric_limits<std::time_t>::max()));
}
