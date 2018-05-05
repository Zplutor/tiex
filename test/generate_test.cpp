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


static String ToString(int value) {
#if TIEX_USE_WCHAR
    return std::to_wstring(value);
#else
    return std::to_string(value);
#endif
}


static Locale GetFullLocale() {
    Locale locale;
    locale.get_month = [](int, const Locale::MonthOptions&) { return TIEX_STRING("month"); };
    locale.get_weekday = [](int, const Locale::WeekdayOptions&) { return TIEX_STRING("weekday"); };
    locale.get_am_pm = [](bool) { return TIEX_STRING("ampm"); };
    locale.get_hour = [](int, const Locale::HourOptions&) { return TIEX_STRING("hour"); };
    locale.get_minute = [](int) { return TIEX_STRING("minute"); };
    locale.get_second = [](int) { return TIEX_STRING("second"); };
    return locale;
}


static std::set<Char> GetOverridableSpecifiers() {
    return {
        'p',
        'a', 'A',
        'b', 'h', 'B', 'm',
        'M',
        'S',
        'H', 'I',
    };
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
        locale.get_weekday = [&weekday_map](int weekday, const Locale::WeekdayOptions& options) {
            const auto& pair = weekday_map[weekday];
            return options.is_abbreviated ? pair.first : pair.second;
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


TEST(Generate, GetLocaleText_Month) {
    
    std::map<int, std::tuple<String, String, String>> month_map = {
        { 1,  { TIEX_STRING("one"),    TIEX_STRING("ONE"),    TIEX_STRING("001")  } },
        { 2,  { TIEX_STRING("two"),    TIEX_STRING("TWO"),    TIEX_STRING("002")  } },
        { 3,  { TIEX_STRING("three"),  TIEX_STRING("THREE"),  TIEX_STRING("003")  } },
        { 4,  { TIEX_STRING("four"),   TIEX_STRING("FOUR"),   TIEX_STRING("004")  } },
        { 5,  { TIEX_STRING("five"),   TIEX_STRING("FIVE"),   TIEX_STRING("005")  } },
        { 6,  { TIEX_STRING("six"),    TIEX_STRING("SIX"),    TIEX_STRING("006")  } },
        { 7,  { TIEX_STRING("seven"),  TIEX_STRING("SEVEN"),  TIEX_STRING("007")  } },
        { 8,  { TIEX_STRING("eight"),  TIEX_STRING("EIGHT"),  TIEX_STRING("008")  } },
        { 9,  { TIEX_STRING("nine"),   TIEX_STRING("NINE"),   TIEX_STRING("009")  } },
        { 10, { TIEX_STRING("ten"),    TIEX_STRING("TEN"),    TIEX_STRING("0010") } },
        { 11, { TIEX_STRING("eleven"), TIEX_STRING("ELEVEN"), TIEX_STRING("0011") } },
        { 12, { TIEX_STRING("twelve"), TIEX_STRING("TWELVE"), TIEX_STRING("0012") } },
    };
    
    auto test = [&month_map](int month, const std::vector<Char> specifier_chars, const String& expected_text) {
        
        Locale locale;
        locale.get_month = [&month_map](int month, const Locale::MonthOptions& options) {
            const auto& tuple = month_map[month];
            if (options.is_number) {
                return std::get<2>(tuple);
            }
            if (options.is_abbreviated) {
                return std::get<0>(tuple);
            }
            return std::get<1>(tuple);
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
    
    ASSERT_TRUE(test(1, { 'm' }, TIEX_STRING("001")));
    ASSERT_TRUE(test(2, { 'm' }, TIEX_STRING("002")));
    ASSERT_TRUE(test(3, { 'm' }, TIEX_STRING("003")));
    ASSERT_TRUE(test(4, { 'm' }, TIEX_STRING("004")));
    ASSERT_TRUE(test(5, { 'm' }, TIEX_STRING("005")));
    ASSERT_TRUE(test(6, { 'm' }, TIEX_STRING("006")));
    ASSERT_TRUE(test(7, { 'm' }, TIEX_STRING("007")));
    ASSERT_TRUE(test(8, { 'm' }, TIEX_STRING("008")));
    ASSERT_TRUE(test(9, { 'm' }, TIEX_STRING("009")));
    ASSERT_TRUE(test(10, { 'm' }, TIEX_STRING("0010")));
    ASSERT_TRUE(test(11, { 'm' }, TIEX_STRING("0011")));
    ASSERT_TRUE(test(12, { 'm' }, TIEX_STRING("0012")));
}


TEST(Generate, GetLocaleText_Hour) {
    
    auto test = [](int hour, Char specifier, const String& expected) {
        Locale locale;
        locale.get_hour = [](int hour, const Locale::HourOptions& options) {
            return ToString(hour);
        };
        auto tm = MakeTm(2018, 5, 5, hour, 0, 0);
        String locale_text;
        bool has_got = GetLocaleText(specifier, tm, locale, locale_text);
        if (! has_got) {
            return false;
        }
        return locale_text == expected;
    };
    
    //24 hour clock
    for (auto hour = 0; hour <= 23; ++hour) {
        ASSERT_TRUE(test(hour, 'H', ToString(hour)));
    }
    
    //12 hour clock
    ASSERT_TRUE(test(0,  'I', TIEX_STRING("12")));
    ASSERT_TRUE(test(1,  'I', TIEX_STRING("1")));
    ASSERT_TRUE(test(2,  'I', TIEX_STRING("2")));
    ASSERT_TRUE(test(3,  'I', TIEX_STRING("3")));
    ASSERT_TRUE(test(4,  'I', TIEX_STRING("4")));
    ASSERT_TRUE(test(5,  'I', TIEX_STRING("5")));
    ASSERT_TRUE(test(6,  'I', TIEX_STRING("6")));
    ASSERT_TRUE(test(7,  'I', TIEX_STRING("7")));
    ASSERT_TRUE(test(8,  'I', TIEX_STRING("8")));
    ASSERT_TRUE(test(9,  'I', TIEX_STRING("9")));
    ASSERT_TRUE(test(10, 'I', TIEX_STRING("10")));
    ASSERT_TRUE(test(11, 'I', TIEX_STRING("11")));
    ASSERT_TRUE(test(12, 'I', TIEX_STRING("12")));
    ASSERT_TRUE(test(13, 'I', TIEX_STRING("1")));
    ASSERT_TRUE(test(14, 'I', TIEX_STRING("2")));
    ASSERT_TRUE(test(15, 'I', TIEX_STRING("3")));
    ASSERT_TRUE(test(16, 'I', TIEX_STRING("4")));
    ASSERT_TRUE(test(17, 'I', TIEX_STRING("5")));
    ASSERT_TRUE(test(18, 'I', TIEX_STRING("6")));
    ASSERT_TRUE(test(19, 'I', TIEX_STRING("7")));
    ASSERT_TRUE(test(20, 'I', TIEX_STRING("8")));
    ASSERT_TRUE(test(21, 'I', TIEX_STRING("9")));
    ASSERT_TRUE(test(22, 'I', TIEX_STRING("10")));
    ASSERT_TRUE(test(23, 'I', TIEX_STRING("11")));
}


TEST(Generate, GetLocaleText_Minute) {
    
    auto test = [](int minute, const String& expected) {
        Locale locale;
        locale.get_minute = [](int minute) {
            return ToString(minute);
        };
        auto tm = MakeTm(2018, 5, 5, 23, minute, 0);
        String locale_text;
        bool has_got = GetLocaleText('M', tm, locale, locale_text);
        if (! has_got) {
            return false;
        }
        return locale_text == expected;
    };
    
    for (int minute = 0; minute <= 59; ++minute) {
        ASSERT_TRUE(test(minute, ToString(minute)));
    }
}


TEST(Generate, GetLocaleText_Second) {
    
    auto test = [](int second, const String& expected) {
        Locale locale;
        locale.get_second = [](int second) {
            return ToString(second);
        };
        auto tm = MakeTm(2018, 5, 5, 23, 13, second);
        String locale_text;
        bool has_got = GetLocaleText('S', tm, locale, locale_text);
        if (! has_got) {
            return false;
        }
        return locale_text == expected;
    };
    
    for (int second = 0; second <= 59; ++second) {
        ASSERT_TRUE(test(second, ToString(second)));
    }
}


TEST(Generate, GetLocaleText_NoLocale) {
    
    auto test = [](Char specifier_char) {
        
        auto tm = MakeTm(2018, 5, 5, 23, 06, 29);
        String locale_text;
        bool has_got = GetLocaleText(specifier_char, tm, Locale(), locale_text);
        if (has_got) {
            return false;
        }
        return locale_text.empty();
    };
    
    for (auto each_char : GetOverridableSpecifiers()) {
        ASSERT_TRUE(test(each_char));
    }
}


TEST(Generate, GetLocaleText_UnsupportedSpecifier) {
    
    auto test = [](Char specifier_char) {
        
        auto locale = GetFullLocale();
        
        auto tm = MakeTm(2018, 3, 16, 13, 40, 22);
        String locale_text;
        bool has_got = GetLocaleText(specifier_char, tm, locale, locale_text);
        if (has_got) {
            return false;
        }
        return locale_text.empty();
    };
    
    auto supported_chars = GetOverridableSpecifiers();
    for (int ch = std::numeric_limits<Char>::min(); ch <= std::numeric_limits<Char>::max(); ++ch) {
        if (supported_chars.find(static_cast<Char>(ch)) == supported_chars.end()) {
            ASSERT_TRUE(test(static_cast<Char>(ch)));
        }
    }
}


TEST(Generate, OverrideStandardSpecifiers_Normal) {
        
    auto tm = MakeTm(2018, 3, 18, 22, 23, 49);
    auto locale = GetFullLocale();
    
    auto result_text = TIEX_STRING("Override locale %A%a%p%b%h%B%m ");
    bool overrode_all = OverrideStandardSpecifiers(tm, locale, result_text);
    ASSERT_TRUE(overrode_all);
    ASSERT_EQ(result_text, TIEX_STRING("Override locale weekdayweekdayampmmonthmonthmonthmonth "));
    
    result_text = TIEX_STRING("Override %Z locale %A%a%p%b%h%B%m ");
    overrode_all = OverrideStandardSpecifiers(tm, locale, result_text);
    ASSERT_FALSE(overrode_all);
    ASSERT_EQ(result_text, TIEX_STRING("Override %Z locale weekdayweekdayampmmonthmonthmonthmonth "));
}


TEST(Generate, OverrideStandardSpecifiers_EscapePercent) {
    
    auto tm = MakeTm(2018, 3, 18, 22, 30, 1);
    auto locale = GetFullLocale();

    auto result_text = TIEX_STRING("Escape%% %%p %%M %a");
    bool overrode_all = OverrideStandardSpecifiers(tm, locale, result_text);
    ASSERT_TRUE(overrode_all);
    ASSERT_EQ(result_text, TIEX_STRING("Escape%% %%p %%M weekday"));
}


TEST(Generate, OverrideStandardSpecifiers_NoLocale) {
    
    auto tm = MakeTm(2018, 3, 18, 21, 58, 44);
    auto result_text = TIEX_STRING("%p %a %A %b %h %B %m");
    bool overrode_all = OverrideStandardSpecifiers(tm, Locale(), result_text);
    ASSERT_FALSE(overrode_all);
    ASSERT_EQ(result_text, TIEX_STRING("%p %a %A %b %h %B %m"));
}


TEST(Generate, OverrideStandardSpecifiers_PercentAtTail) {
    
    auto tm = MakeTm(2018, 3, 18, 22, 20, 0);
    auto locale = GetFullLocale();
    
    auto result_text = TIEX_STRING("Result text %");
    bool overrode_all = OverrideStandardSpecifiers(tm, locale, result_text);
    ASSERT_TRUE(overrode_all);
    ASSERT_EQ(result_text, TIEX_STRING("Result text %"));
}
