#pragma once

#include <functional>
#include "tiex_config.h"

namespace tiex {
    
class Locale {
public:
    class MonthOptions {
    public:
        bool is_number = false;
        bool is_abbreviated = false;
    };
    
    class WeekdayOptions {
    public:
        bool is_abbreviated = false;
    };
    
    class HourOptions {
    public:
        bool is_12_hour_clock = false;
    };
    
public:
    std::function<String(int month, const MonthOptions& options)> get_month;
    std::function<String(int weekday, const WeekdayOptions& options)> get_weekday;
    std::function<String(bool is_pm)> get_am_pm;
    std::function<String(int hour, const HourOptions& options)> get_hour;
    std::function<String(int minute)> get_minute;
    std::function<String(int second)> get_second;
};
    
}
