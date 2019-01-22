#include <gtest/gtest.h>
#include "test_utility.h"
#include "tiex_generate.h"

using namespace tiex;
using namespace tiex::internal;


static std::string ToString(int value) {
#if TIEX_USE_WCHAR
    return std::to_wstring(value);
#else
    return std::to_string(value);
#endif
}


static Locale GetFullLocale() {
    Locale locale;
    locale.get_month = [](int, const Locale::MonthOptions&) { return "month"; };
    locale.get_weekday = [](int, const Locale::WeekdayOptions&) { return "weekday"; };
    locale.get_am_pm = [](bool) { return "ampm"; };
    locale.get_hour = [](int, const Locale::HourOptions&) { return "hour"; };
    locale.get_minute = [](int) { return "minute"; };
    locale.get_second = [](int) { return "second"; };
    return locale;
}


static std::set<char> GetOverridableSpecifiers() {
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
            return is_pm ? "late" : "early";
        };
        
        auto tm = MakeTm(2018, 3, 16, hour, 10, 13);
        std::string locale_text;
        bool has_got = GetLocaleText('p', tm, locale, locale_text);
        if (! has_got) {
            return false;
        }
        
        if (expected_pm) {
            return locale_text == "late";
        }
        else {
            return locale_text == "early";
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
    
    std::map<int, std::pair<std::string, std::string>> weekday_map = {
        { 0, { "zero", "ZERO" } },
        { 1, { "one", "ONE" } },
        { 2, { "two", "TWO" } },
        { 3, { "three", "THREE" } },
        { 4, { "four", "FOUR" } },
        { 5, { "five", "FIVE" } },
        { 6, { "six", "SIX" } },
    };
    
    auto test = [&weekday_map](int weekday, bool abbreviated, const std::string& expected_text) {
        
        Locale locale;
        locale.get_weekday = [&weekday_map](int weekday, const Locale::WeekdayOptions& options) {
            const auto& pair = weekday_map[weekday];
            return options.is_abbreviated ? pair.first : pair.second;
        };
        
        auto tm = MakeTm(2018, 3, 18 + weekday, 13, 27, 28);
        std::string locale_text;
        bool has_got = GetLocaleText(abbreviated ? 'a' : 'A', tm, locale, locale_text);
        if (! has_got) {
            return false;
        }
        
        return locale_text == expected_text;
    };
    
    ASSERT_TRUE(test(0, true, "zero"));
    ASSERT_TRUE(test(1, true, "one"));
    ASSERT_TRUE(test(2, true, "two"));
    ASSERT_TRUE(test(3, true, "three"));
    ASSERT_TRUE(test(4, true, "four"));
    ASSERT_TRUE(test(5, true, "five"));
    ASSERT_TRUE(test(6, true, "six"));
    ASSERT_TRUE(test(0, false, "ZERO"));
    ASSERT_TRUE(test(1, false, "ONE"));
    ASSERT_TRUE(test(2, false, "TWO"));
    ASSERT_TRUE(test(3, false, "THREE"));
    ASSERT_TRUE(test(4, false, "FOUR"));
    ASSERT_TRUE(test(5, false, "FIVE"));
    ASSERT_TRUE(test(6, false, "SIX"));
}


TEST(Generate, GetLocaleText_Month) {
    
    std::map<int, std::tuple<std::string, std::string, std::string>> month_map = {
        { 1,  { "one",    "ONE",    "001"  } },
        { 2,  { "two",    "TWO",    "002"  } },
        { 3,  { "three",  "THREE",  "003"  } },
        { 4,  { "four",   "FOUR",   "004"  } },
        { 5,  { "five",   "FIVE",   "005"  } },
        { 6,  { "six",    "SIX",    "006"  } },
        { 7,  { "seven",  "SEVEN",  "007"  } },
        { 8,  { "eight",  "EIGHT",  "008"  } },
        { 9,  { "nine",   "NINE",   "009"  } },
        { 10, { "ten",    "TEN",    "0010" } },
        { 11, { "eleven", "ELEVEN", "0011" } },
        { 12, { "twelve", "TWELVE", "0012" } },
    };
    
    auto test = [&month_map](int month, const std::vector<char> specifier_chars, const std::string& expected_text) {
        
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
        
            std::string locale_text;
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
    
    ASSERT_TRUE(test(1, { 'b', 'h' }, "one"));
    ASSERT_TRUE(test(2, { 'b', 'h' }, "two"));
    ASSERT_TRUE(test(3, { 'b', 'h' }, "three"));
    ASSERT_TRUE(test(4, { 'b', 'h' }, "four"));
    ASSERT_TRUE(test(5, { 'b', 'h' }, "five"));
    ASSERT_TRUE(test(6, { 'b', 'h' }, "six"));
    ASSERT_TRUE(test(7, { 'b', 'h' }, "seven"));
    ASSERT_TRUE(test(8, { 'b', 'h' }, "eight"));
    ASSERT_TRUE(test(9, { 'b', 'h' }, "nine"));
    ASSERT_TRUE(test(10, { 'b', 'h' }, "ten"));
    ASSERT_TRUE(test(11, { 'b', 'h' }, "eleven"));
    ASSERT_TRUE(test(12, { 'b', 'h' }, "twelve"));
    
    ASSERT_TRUE(test(1, { 'B' }, "ONE"));
    ASSERT_TRUE(test(2, { 'B' }, "TWO"));
    ASSERT_TRUE(test(3, { 'B' }, "THREE"));
    ASSERT_TRUE(test(4, { 'B' }, "FOUR"));
    ASSERT_TRUE(test(5, { 'B' }, "FIVE"));
    ASSERT_TRUE(test(6, { 'B' }, "SIX"));
    ASSERT_TRUE(test(7, { 'B' }, "SEVEN"));
    ASSERT_TRUE(test(8, { 'B' }, "EIGHT"));
    ASSERT_TRUE(test(9, { 'B' }, "NINE"));
    ASSERT_TRUE(test(10, { 'B' }, "TEN"));
    ASSERT_TRUE(test(11, { 'B' }, "ELEVEN"));
    ASSERT_TRUE(test(12, { 'B' }, "TWELVE"));
    
    ASSERT_TRUE(test(1, { 'm' }, "001"));
    ASSERT_TRUE(test(2, { 'm' }, "002"));
    ASSERT_TRUE(test(3, { 'm' }, "003"));
    ASSERT_TRUE(test(4, { 'm' }, "004"));
    ASSERT_TRUE(test(5, { 'm' }, "005"));
    ASSERT_TRUE(test(6, { 'm' }, "006"));
    ASSERT_TRUE(test(7, { 'm' }, "007"));
    ASSERT_TRUE(test(8, { 'm' }, "008"));
    ASSERT_TRUE(test(9, { 'm' }, "009"));
    ASSERT_TRUE(test(10, { 'm' }, "0010"));
    ASSERT_TRUE(test(11, { 'm' }, "0011"));
    ASSERT_TRUE(test(12, { 'm' }, "0012"));
}


TEST(Generate, GetLocaleText_Hour) {
    
    auto test = [](int hour, char specifier, const std::string& expected) {
        Locale locale;
        locale.get_hour = [](int hour, const Locale::HourOptions& options) {
            return ToString(hour);
        };
        auto tm = MakeTm(2018, 5, 5, hour, 0, 0);
        std::string locale_text;
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
    ASSERT_TRUE(test(0,  'I', "12"));
    ASSERT_TRUE(test(1,  'I', "1"));
    ASSERT_TRUE(test(2,  'I', "2"));
    ASSERT_TRUE(test(3,  'I', "3"));
    ASSERT_TRUE(test(4,  'I', "4"));
    ASSERT_TRUE(test(5,  'I', "5"));
    ASSERT_TRUE(test(6,  'I', "6"));
    ASSERT_TRUE(test(7,  'I', "7"));
    ASSERT_TRUE(test(8,  'I', "8"));
    ASSERT_TRUE(test(9,  'I', "9"));
    ASSERT_TRUE(test(10, 'I', "10"));
    ASSERT_TRUE(test(11, 'I', "11"));
    ASSERT_TRUE(test(12, 'I', "12"));
    ASSERT_TRUE(test(13, 'I', "1"));
    ASSERT_TRUE(test(14, 'I', "2"));
    ASSERT_TRUE(test(15, 'I', "3"));
    ASSERT_TRUE(test(16, 'I', "4"));
    ASSERT_TRUE(test(17, 'I', "5"));
    ASSERT_TRUE(test(18, 'I', "6"));
    ASSERT_TRUE(test(19, 'I', "7"));
    ASSERT_TRUE(test(20, 'I', "8"));
    ASSERT_TRUE(test(21, 'I', "9"));
    ASSERT_TRUE(test(22, 'I', "10"));
    ASSERT_TRUE(test(23, 'I', "11"));
}


TEST(Generate, GetLocaleText_Minute) {
    
    auto test = [](int minute, const std::string& expected) {
        Locale locale;
        locale.get_minute = [](int minute) {
            return ToString(minute);
        };
        auto tm = MakeTm(2018, 5, 5, 23, minute, 0);
        std::string locale_text;
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
    
    auto test = [](int second, const std::string& expected) {
        Locale locale;
        locale.get_second = [](int second) {
            return ToString(second);
        };
        auto tm = MakeTm(2018, 5, 5, 23, 13, second);
        std::string locale_text;
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
    
    auto test = [](char specifier_char) {
        
        auto tm = MakeTm(2018, 5, 5, 23, 06, 29);
        std::string locale_text;
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
    
    auto test = [](char specifier_char) {
        
        auto locale = GetFullLocale();
        
        auto tm = MakeTm(2018, 3, 16, 13, 40, 22);
        std::string locale_text;
        bool has_got = GetLocaleText(specifier_char, tm, locale, locale_text);
        if (has_got) {
            return false;
        }
        return locale_text.empty();
    };
    
    auto supported_chars = GetOverridableSpecifiers();
    for (int ch = std::numeric_limits<char>::min(); ch <= std::numeric_limits<char>::max(); ++ch) {
        if (supported_chars.find(static_cast<char>(ch)) == supported_chars.end()) {
            ASSERT_TRUE(test(static_cast<char>(ch)));
        }
    }
}


TEST(Generate, OverrideStandardSpecifiers_Normal) {
        
    auto tm = MakeTm(2018, 3, 18, 22, 23, 49);
    auto locale = GetFullLocale();
    
    std::string result_text = "Override locale %A%a%p%b%h%B%m ";
    bool overrode_all = OverrideStandardSpecifiers(tm, locale, result_text);
    ASSERT_TRUE(overrode_all);
    ASSERT_EQ(result_text, "Override locale weekdayweekdayampmmonthmonthmonthmonth ");
    
    result_text = "Override %Z locale %A%a%p%b%h%B%m ";
    overrode_all = OverrideStandardSpecifiers(tm, locale, result_text);
    ASSERT_FALSE(overrode_all);
    ASSERT_EQ(result_text, "Override %Z locale weekdayweekdayampmmonthmonthmonthmonth ");
}


TEST(Generate, OverrideStandardSpecifiers_EscapePercent) {
    
    auto tm = MakeTm(2018, 3, 18, 22, 30, 1);
    auto locale = GetFullLocale();

    std::string result_text = "Escape%% %%p %%M %a";
    bool overrode_all = OverrideStandardSpecifiers(tm, locale, result_text);
    ASSERT_TRUE(overrode_all);
    ASSERT_EQ(result_text, "Escape%% %%p %%M weekday");
}


TEST(Generate, OverrideStandardSpecifiers_NoLocale) {
    
    auto tm = MakeTm(2018, 3, 18, 21, 58, 44);
	std::string result_text = "%p %a %A %b %h %B %m";
    bool overrode_all = OverrideStandardSpecifiers(tm, Locale(), result_text);
    ASSERT_FALSE(overrode_all);
    ASSERT_EQ(result_text, "%p %a %A %b %h %B %m");
}


TEST(Generate, OverrideStandardSpecifiers_PercentAtTail) {
    
    auto tm = MakeTm(2018, 3, 18, 22, 20, 0);
    auto locale = GetFullLocale();
    
	std::string result_text = "Result text %";
    bool overrode_all = OverrideStandardSpecifiers(tm, locale, result_text);
    ASSERT_TRUE(overrode_all);
    ASSERT_EQ(result_text, "Result text %");
}
