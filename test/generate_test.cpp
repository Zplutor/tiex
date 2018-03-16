#include <gtest/gtest.h>
#include "test_utility.h"
#include "tiex_generate.h"

using namespace tiex;
using namespace tiex::internal;

static long GetDifferenceWithTimet(Unit unit, std::time_t referenced, std::time_t formatted) {
    Specifier specifier;
    specifier.uint = unit;
    return GetDifferenceWithTimet(specifier, referenced, formatted);
}


static long GetDifferenceWithTm(Unit unit, const std::tm& referenced, const std::tm& formatted) {
    Specifier specifer;
    specifer.uint = unit;
    return GetDifferenceWithTm(specifer, referenced, formatted);
}


TEST(Generate, GetDifferenceWithTimet_Second) {

    auto test = [](std::time_t referenced, std::time_t formatted, long expected) {
        auto actual = GetDifferenceWithTimet(Unit::Second, referenced, formatted);
        return actual == expected;
    };
    
    ASSERT_TRUE(test(-2, 0, 2));
    ASSERT_TRUE(test(-1, 0, 1));
    ASSERT_TRUE(test(0, 0, 0));
    ASSERT_TRUE(test(0, 1, 1));
    ASSERT_TRUE(test(0, 2, 2));
    
    ASSERT_TRUE(test(0, -2, -2));
    ASSERT_TRUE(test(0, -1, -1));
    ASSERT_TRUE(test(1, 0, -1));
    ASSERT_TRUE(test(2, 0, -2));
    
    ASSERT_TRUE(test(1517461654, 1517461644, -10));
    ASSERT_TRUE(test(1517461644, 1517461644, 0));
    ASSERT_TRUE(test(1517461644, 1517461654, 10));
}


TEST(Generate, GetDifferenceWithTimet_Minute) {
    
    auto test = [](std::time_t referenced, std::time_t formatted, long expected) {
        auto actual = GetDifferenceWithTimet(Unit::Minute, referenced, formatted);
        return actual == expected;
    };
    
    auto referenced_time = MakeTime(2018, 2, 1, 13, 11, 59);
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 9,  0),  -2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 9,  58), -2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 9,  59), -2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 10, 0),  -1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 10, 58), -1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 10, 59), -1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 11, 0),  0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 11, 58), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 11, 59), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 12, 0),  0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 12, 58), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 12, 59), 1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 13, 0),  1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 13, 58), 1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 13, 59), 2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 14, 0),  2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 14, 58), 2));
}


TEST(Generate, GetDifferenceWithTimet_Hour) {
    
    auto test = [](std::time_t referenced, std::time_t formatted, long expected) {
        auto actual = GetDifferenceWithTimet(Unit::Hour, referenced, formatted);
        return actual == expected;
    };
    
    auto referenced_time = MakeTime(2018, 2, 1, 13, 20, 44);
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 10, 20, 45), -2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 11, 20, 43), -2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 11, 20, 44), -2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 11, 20, 45), -1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 12, 20, 43), -1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 12, 20, 44), -1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 12, 20, 45), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 20, 43), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 20, 44), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 13, 20, 45), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 14, 20, 43), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 14, 20, 44), 1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 14, 20, 45), 1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 15, 20, 43), 1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 15, 20, 44), 2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 15, 20, 45), 2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1, 16, 20, 43), 2));
}


TEST(Generate, GetDifferenceWithTimet_Day) {
    
    auto test = [](std::time_t referenced, std::time_t formatted, long expected) {
        auto actual = GetDifferenceWithTimet(Unit::Day, referenced, formatted);
        return actual == expected;
    };
    
    auto referenced_time = MakeTime(2018, 2, 1, 13, 29, 8);
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 29, 13, 29, 9), -2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 30, 13, 29, 7), -2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 30, 13, 29, 8), -2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 30, 13, 29, 9), -1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 31, 13, 29, 7), -1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 31, 13, 29, 8), -1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 31, 13, 29, 9), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1,  13, 29, 7), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1,  13, 29, 8), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1,  13, 29, 9), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 2,  13, 29, 7), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 2,  13, 29, 8), 1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 2,  13, 29, 9), 1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 3,  13, 29, 7), 1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 3,  13, 29, 8), 2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 3,  13, 29, 9), 2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 4,  13, 29, 7), 2));
}


TEST(Generate, GetDifferenceWithTimet_Week) {
    
    auto test = [](std::time_t referenced, std::time_t formatted, long expected) {
        auto actual = GetDifferenceWithTimet(Unit::Week, referenced, formatted);
        return actual == expected;
    };
    
    auto referenced_time = MakeTime(2018, 2, 1, 13, 36, 24);
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 11, 13, 36, 25), -2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 18, 13, 36, 23), -2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 18, 13, 36, 24), -2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 18, 13, 36, 25), -1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 25, 13, 36, 23), -1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 25, 13, 36, 24), -1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 1, 25, 13, 36, 25), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1,  13, 36, 23), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1,  13, 36, 24), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 1,  13, 36, 25), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 8,  13, 36, 23), 0));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 8,  13, 36, 24), 1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 8,  13, 36, 25), 1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 15, 13, 36, 23), 1));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 15, 13, 36, 24), 2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 15, 13, 36, 25), 2));
    ASSERT_TRUE(test(referenced_time, MakeTime(2018, 2, 22, 13, 36, 23), 2));
}


TEST(Generate, GetDifferenceWithTm_Month) {
    
    auto test = [](const std::tm& referenced, const std::tm& formatted, long expected) {
        auto actual = GetDifferenceWithTm(Unit::Month, referenced, formatted);
        return actual == expected;
    };
    
    auto referenced_tm = MakeTm(2018, 2, 1, 13, 43, 58);
    ASSERT_TRUE(test(referenced_tm, MakeTm(2017, 11, 1, 13, 43, 59), -2));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2017, 12, 1, 13, 43, 57), -2));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2017, 12, 1, 13, 43, 58), -2));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2017, 12, 1, 13, 43, 59), -1));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 1,  1, 13, 43, 57), -1));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 1,  1, 13, 43, 58), -1));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 1,  1, 13, 43, 59), 0));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 2,  1, 13, 43, 57), 0));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 2,  1, 13, 43, 58), 0));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 2,  1, 13, 43, 59), 0));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 3,  1, 13, 43, 57), 0));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 3,  1, 13, 43, 58), 1));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 3,  1, 13, 43, 59), 1));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 4,  1, 13, 43, 57), 1));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 4,  1, 13, 43, 58), 2));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 4,  1, 13, 43, 59), 2));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 5,  1, 13, 43, 57), 2));
    
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 1,  31, 23, 59, 59), 0));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 2,  1,  0,  0,  0), 0));
}


TEST(Generate, GetDifferenceWithTm_Year) {
    
    auto test = [](const std::tm& referenced, const std::tm& formatted, long expected) {
        auto actual = GetDifferenceWithTm(Unit::Year, referenced, formatted);
        return actual == expected;
    };
    
    auto referenced_tm = MakeTm(2018, 2, 5, 13, 39, 29);
    ASSERT_TRUE(test(referenced_tm, MakeTm(2015, 2, 5, 13, 39, 30), -2));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2016, 2, 5, 13, 39, 28), -2));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2016, 2, 5, 13, 39, 29), -2));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2016, 2, 5, 13, 39, 30), -1));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2017, 2, 5, 13, 39, 28), -1));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2017, 2, 5, 13, 39, 29), -1));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2017, 2, 5, 13, 39, 30), 0));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 2, 5, 13, 39, 28), 0));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 2, 5, 13, 39, 29), 0));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 2, 5, 13, 39, 30), 0));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2019, 2, 5, 13, 39, 28), 0));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2019, 2, 5, 13, 39, 29), 1));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2019, 2, 5, 13, 39, 30), 1));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2020, 2, 5, 13, 39, 28), 1));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2020, 2, 5, 13, 39, 29), 2));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2020, 2, 5, 13, 39, 30), 2));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2021, 2, 5, 13, 39, 28), 2));
    
    ASSERT_TRUE(test(referenced_tm, MakeTm(2017, 12, 31, 23, 59, 59), 0));
    ASSERT_TRUE(test(referenced_tm, MakeTm(2018, 1,  1,  0,  0,  0), 0));
}


TEST(Generate, GetLocaleText_AmPm) {
    
    auto test = [](int hour, bool expected_pm) {
        
        Locale locale;
        locale.get_am_pm = [](bool is_pm) {
            return is_pm ? TIEX_STRING("late") : TIEX_STRING("early");
        };
        
        auto tm = MakeTm(2018, 3, 16, hour, 10, 13);
        String locale_text;
        bool has_got = GetLocaleText('p', tm, locale, locale_text);
        if (! has_got) {
            return false;
        }
        
        if (expected_pm) {
            return locale_text == TIEX_STRING("late");
        }
        else {
            return locale_text == TIEX_STRING("early");
        }
    };
    
    ASSERT_TRUE(test(0, false));
    ASSERT_TRUE(test(1, false));
    ASSERT_TRUE(test(2, false));
    ASSERT_TRUE(test(3, false));
    ASSERT_TRUE(test(4, false));
    ASSERT_TRUE(test(5, false));
    ASSERT_TRUE(test(6, false));
    ASSERT_TRUE(test(7, false));
    ASSERT_TRUE(test(8, false));
    ASSERT_TRUE(test(9, false));
    ASSERT_TRUE(test(10, false));
    ASSERT_TRUE(test(11, false));
    ASSERT_TRUE(test(12, true));
    ASSERT_TRUE(test(13, true));
    ASSERT_TRUE(test(14, true));
    ASSERT_TRUE(test(15, true));
    ASSERT_TRUE(test(16, true));
    ASSERT_TRUE(test(17, true));
    ASSERT_TRUE(test(18, true));
    ASSERT_TRUE(test(19, true));
    ASSERT_TRUE(test(20, true));
    ASSERT_TRUE(test(21, true));
    ASSERT_TRUE(test(22, true));
    ASSERT_TRUE(test(23, true));
}


TEST(Generate, GetLocaleText_AmPmWithoutLocale) {
    
    auto tm = MakeTm(2018, 3, 16, 13, 20, 13);
    String locale_text;
    bool has_got = GetLocaleText('p', tm, Locale(), locale_text);
    ASSERT_FALSE(has_got);
    ASSERT_EQ(locale_text, String());
}


TEST(Generate, GetLocaleText_Weekday) {
    
    std::map<int, std::pair<String, String>> weekday_map = {
        { 0, { TIEX_STRING("日"), TIEX_STRING("周日") } },
        { 1, { TIEX_STRING("一"), TIEX_STRING("周一") } },
        { 2, { TIEX_STRING("二"), TIEX_STRING("周二") } },
        { 3, { TIEX_STRING("三"), TIEX_STRING("周三") } },
        { 4, { TIEX_STRING("四"), TIEX_STRING("周四") } },
        { 5, { TIEX_STRING("五"), TIEX_STRING("周五") } },
        { 6, { TIEX_STRING("六"), TIEX_STRING("周六") } },
    };
    
    auto test = [&weekday_map](int weekday, bool abbreviated, const String& expected_text) {
        
        Locale locale;
        locale.get_weekday = [&weekday_map](int weekday, bool abbreviated) {
            const auto& pair = weekday_map[weekday];
            return abbreviated ? pair.first : pair.second;
        };
        
        auto tm = MakeTm(2018, 3, 18 + weekday, 13, 27, 28);
        String locale_text;
        bool has_got = GetLocaleText(abbreviated ? 'a' : 'A', tm, locale, locale_text);
        if (! has_got) {
            return false;
        }
        
        return locale_text == expected_text;
    };
    
    ASSERT_TRUE(test(0, true, TIEX_STRING("日")));
    ASSERT_TRUE(test(1, true, TIEX_STRING("一")));
    ASSERT_TRUE(test(2, true, TIEX_STRING("二")));
    ASSERT_TRUE(test(3, true, TIEX_STRING("三")));
    ASSERT_TRUE(test(4, true, TIEX_STRING("四")));
    ASSERT_TRUE(test(5, true, TIEX_STRING("五")));
    ASSERT_TRUE(test(6, true, TIEX_STRING("六")));
    ASSERT_TRUE(test(0, false, TIEX_STRING("周日")));
    ASSERT_TRUE(test(1, false, TIEX_STRING("周一")));
    ASSERT_TRUE(test(2, false, TIEX_STRING("周二")));
    ASSERT_TRUE(test(3, false, TIEX_STRING("周三")));
    ASSERT_TRUE(test(4, false, TIEX_STRING("周四")));
    ASSERT_TRUE(test(5, false, TIEX_STRING("周五")));
    ASSERT_TRUE(test(6, false, TIEX_STRING("周六")));
}


TEST(Generate, GetLocaleText_WeekdayWithoutLocale) {
    
    auto test = [](bool abbreviated) {
        
        auto tm = MakeTm(2018, 3, 16, 13, 38, 32);
        String locale_text;
        bool has_got = GetLocaleText(abbreviated ? 'a' : 'A', tm, Locale(), locale_text);
        if (has_got) {
            return false;
        }
        return locale_text.empty();
    };
    
    ASSERT_TRUE(test(true));
    ASSERT_TRUE(test(false));
}


TEST(Generate, GetLocaleText_UnsupportedSpecifier) {
    
    auto test = [](Char specifier_char) {
        
        Locale locale;
        locale.get_am_pm = [](bool is_pm) { return TIEX_STRING("ampm"); };
        locale.get_weekday = [](int weekday, bool abbreviated) { return TIEX_STRING("weekday"); };
        
        auto tm = MakeTm(2018, 3, 16, 13, 40, 22);
        String locale_text;
        bool has_got = GetLocaleText(specifier_char, tm, locale, locale_text);
        if (has_got) {
            return false;
        }
        return locale_text.empty();
    };
    
    std::set<Char> supported_chars = { 'p', 'a', 'A' };
    
    for (int ch = std::numeric_limits<Char>::min(); ch <= std::numeric_limits<Char>::max(); ++ch) {
        if (supported_chars.find(static_cast<Char>(ch)) == supported_chars.end()) {
            ASSERT_TRUE(test(static_cast<Char>(ch)));
        }
    }
}
