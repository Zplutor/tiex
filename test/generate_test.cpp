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
        { 0, { TIEX_STRING("zero"), TIEX_STRING("ZERO") } },
        { 1, { TIEX_STRING("one"), TIEX_STRING("ONE") } },
        { 2, { TIEX_STRING("two"), TIEX_STRING("TWO") } },
        { 3, { TIEX_STRING("three"), TIEX_STRING("THREE") } },
        { 4, { TIEX_STRING("four"), TIEX_STRING("FOUR") } },
        { 5, { TIEX_STRING("five"), TIEX_STRING("FIVE") } },
        { 6, { TIEX_STRING("six"), TIEX_STRING("SIX") } },
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
    
    ASSERT_TRUE(test(0, true, TIEX_STRING("zero")));
    ASSERT_TRUE(test(1, true, TIEX_STRING("one")));
    ASSERT_TRUE(test(2, true, TIEX_STRING("two")));
    ASSERT_TRUE(test(3, true, TIEX_STRING("three")));
    ASSERT_TRUE(test(4, true, TIEX_STRING("four")));
    ASSERT_TRUE(test(5, true, TIEX_STRING("five")));
    ASSERT_TRUE(test(6, true, TIEX_STRING("six")));
    ASSERT_TRUE(test(0, false, TIEX_STRING("ZERO")));
    ASSERT_TRUE(test(1, false, TIEX_STRING("ONE")));
    ASSERT_TRUE(test(2, false, TIEX_STRING("TWO")));
    ASSERT_TRUE(test(3, false, TIEX_STRING("THREE")));
    ASSERT_TRUE(test(4, false, TIEX_STRING("FOUR")));
    ASSERT_TRUE(test(5, false, TIEX_STRING("FIVE")));
    ASSERT_TRUE(test(6, false, TIEX_STRING("SIX")));
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


TEST(Generate, GetLocaleText_Month) {
    
    std::map<int, std::pair<String, String>> month_map = {
        { 1, { TIEX_STRING("one"), TIEX_STRING("ONE") } },
        { 2, { TIEX_STRING("two"), TIEX_STRING("TWO") } },
        { 3, { TIEX_STRING("three"), TIEX_STRING("THREE") } },
        { 4, { TIEX_STRING("four"), TIEX_STRING("FOUR") } },
        { 5, { TIEX_STRING("five"), TIEX_STRING("FIVE") } },
        { 6, { TIEX_STRING("six"), TIEX_STRING("SIX") } },
        { 7, { TIEX_STRING("seven"), TIEX_STRING("SEVEN") } },
        { 8, { TIEX_STRING("eight"), TIEX_STRING("EIGHT") } },
        { 9, { TIEX_STRING("nine"), TIEX_STRING("NINE") } },
        { 10, { TIEX_STRING("ten"), TIEX_STRING("TEN") } },
        { 11, { TIEX_STRING("eleven"), TIEX_STRING("ELEVEN") } },
        { 12, { TIEX_STRING("twelve"), TIEX_STRING("TWELVE") } },
    };
    
    auto test = [&month_map](int month, const std::vector<Char> specifier_chars, const String& expected_text) {
        
        Locale locale;
        locale.get_month = [&month_map](int month, bool abbreviated) {
            const auto& pair = month_map[month];
            return abbreviated ? pair.first : pair.second;
        };
        
        auto tm = MakeTm(2018, month, 20, 18, 39, 8);
        
        for (auto each_char : specifier_chars) {
        
            String locale_text;
            bool has_got = GetLocaleText(each_char, tm, locale, locale_text);
            if (! has_got) {
                return false;
            }
            
            if (locale_text != expected_text) {
                return false;
            }
        }
        
        return true;
    };
    
    ASSERT_TRUE(test(1, { 'b', 'h' }, TIEX_STRING("one")));
    ASSERT_TRUE(test(2, { 'b', 'h' }, TIEX_STRING("two")));
    ASSERT_TRUE(test(3, { 'b', 'h' }, TIEX_STRING("three")));
    ASSERT_TRUE(test(4, { 'b', 'h' }, TIEX_STRING("four")));
    ASSERT_TRUE(test(5, { 'b', 'h' }, TIEX_STRING("five")));
    ASSERT_TRUE(test(6, { 'b', 'h' }, TIEX_STRING("six")));
    ASSERT_TRUE(test(7, { 'b', 'h' }, TIEX_STRING("seven")));
    ASSERT_TRUE(test(8, { 'b', 'h' }, TIEX_STRING("eight")));
    ASSERT_TRUE(test(9, { 'b', 'h' }, TIEX_STRING("nine")));
    ASSERT_TRUE(test(10, { 'b', 'h' }, TIEX_STRING("ten")));
    ASSERT_TRUE(test(11, { 'b', 'h' }, TIEX_STRING("eleven")));
    ASSERT_TRUE(test(12, { 'b', 'h' }, TIEX_STRING("twelve")));
    
    ASSERT_TRUE(test(1, { 'B' }, TIEX_STRING("ONE")));
    ASSERT_TRUE(test(2, { 'B' }, TIEX_STRING("TWO")));
    ASSERT_TRUE(test(3, { 'B' }, TIEX_STRING("THREE")));
    ASSERT_TRUE(test(4, { 'B' }, TIEX_STRING("FOUR")));
    ASSERT_TRUE(test(5, { 'B' }, TIEX_STRING("FIVE")));
    ASSERT_TRUE(test(6, { 'B' }, TIEX_STRING("SIX")));
    ASSERT_TRUE(test(7, { 'B' }, TIEX_STRING("SEVEN")));
    ASSERT_TRUE(test(8, { 'B' }, TIEX_STRING("EIGHT")));
    ASSERT_TRUE(test(9, { 'B' }, TIEX_STRING("NINE")));
    ASSERT_TRUE(test(10, { 'B' }, TIEX_STRING("TEN")));
    ASSERT_TRUE(test(11, { 'B' }, TIEX_STRING("ELEVEN")));
    ASSERT_TRUE(test(12, { 'B' }, TIEX_STRING("TWELVE")));
}


TEST(Generate, GetLocaleText_MonthWithoutLocale) {
    
    auto test = [](Char specifier_char) {
        
        auto tm = MakeTm(2018, 3, 20, 18, 46, 21);
        String locale_text;
        bool has_got = GetLocaleText(specifier_char, tm, Locale(), locale_text);
        if (has_got) {
            return false;
        }
        return locale_text.empty();
    };
    
    ASSERT_TRUE(test('b'));
    ASSERT_TRUE(test('h'));
    ASSERT_TRUE(test('B'));
}


TEST(Generate, GetLocaleText_UnsupportedSpecifier) {
    
    auto test = [](Char specifier_char) {
        
        Locale locale;
        locale.get_am_pm = [](bool is_pm) { return TIEX_STRING("ampm"); };
        locale.get_weekday = [](int weekday, bool abbreviated) { return TIEX_STRING("weekday"); };
        locale.get_month = [](int month, bool abbreviated) { return TIEX_STRING("month"); };
        
        auto tm = MakeTm(2018, 3, 16, 13, 40, 22);
        String locale_text;
        bool has_got = GetLocaleText(specifier_char, tm, locale, locale_text);
        if (has_got) {
            return false;
        }
        return locale_text.empty();
    };
    
    std::set<Char> supported_chars = { 'p', 'a', 'A', 'b', 'h', 'B' };
    
    for (int ch = std::numeric_limits<Char>::min(); ch <= std::numeric_limits<Char>::max(); ++ch) {
        if (supported_chars.find(static_cast<Char>(ch)) == supported_chars.end()) {
            ASSERT_TRUE(test(static_cast<Char>(ch)));
        }
    }
}


TEST(Generate, OverrideStandardSpecifiers_Normal) {
        
    auto tm = MakeTm(2018, 3, 18, 22, 23, 49);
    
    Locale locale;
    locale.get_am_pm = [](bool is_pm) { return TIEX_STRING("ampm"); };
    locale.get_weekday = [](int weekday, bool abbreviated) { return TIEX_STRING("weekday"); };
    
    auto result_text = TIEX_STRING("Override locale %A %a%p ");
    bool overrode_all = OverrideStandardSpecifiers(tm, locale, result_text);
    ASSERT_TRUE(overrode_all);
    ASSERT_EQ(result_text, TIEX_STRING("Override locale weekday weekdayampm "));
    
    result_text = TIEX_STRING("Override %H locale %A%a%p ");
    overrode_all = OverrideStandardSpecifiers(tm, locale, result_text);
    ASSERT_FALSE(overrode_all);
    ASSERT_EQ(result_text, TIEX_STRING("Override %H locale weekdayweekdayampm "));
}


TEST(Generate, OverrideStandardSpecifiers_EscapePercent) {
    
    auto tm = MakeTm(2018, 3, 18, 22, 30, 1);
    
    Locale locale;
    locale.get_am_pm = [](bool is_pm) { return TIEX_STRING("ampm"); };
    locale.get_weekday = [](int weekday, bool abbreviated) { return TIEX_STRING("weekday"); };
    
    auto result_text = TIEX_STRING("Escape%% %%p %%M %a");
    bool overrode_all = OverrideStandardSpecifiers(tm, locale, result_text);
    ASSERT_TRUE(overrode_all);
    ASSERT_EQ(result_text, TIEX_STRING("Escape%% %%p %%M weekday"));
}


TEST(Generate, OverrideStandardSpecifiers_NoLocale) {
    
    auto tm = MakeTm(2018, 3, 18, 21, 58, 44);
    auto result_text = TIEX_STRING("%p %a %A");
    bool overrode_all = OverrideStandardSpecifiers(tm, Locale(), result_text);
    ASSERT_FALSE(overrode_all);
    ASSERT_EQ(result_text, TIEX_STRING("%p %a %A"));
}


TEST(Generate, OverrideStandardSpecifiers_PercentAtTail) {
    
    auto tm = MakeTm(2018, 3, 18, 22, 20, 0);
    Locale locale;
    locale.get_am_pm = [](bool is_pm) { return TIEX_STRING("ampm"); };
    auto result_text = TIEX_STRING("Result text %");
    bool overrode_all = OverrideStandardSpecifiers(tm, locale, result_text);
    ASSERT_TRUE(overrode_all);
    ASSERT_EQ(result_text, TIEX_STRING("Result text %"));
}
