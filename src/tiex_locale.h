#pragma once

#include <functional>
#include "tiex_config.h"

namespace tiex {
    
/**
 A locale contains information that help to format localized time string.
 */
class Locale {
public:
    class MonthOptions {
    public:
        bool is_number = false;
        bool is_abbreviated = false;
    };
    
    /**
     The prototype of callback to get localized string of month.
     
     @param month
       Numerical value of month, from 1 to 12.
     
     @param options
       Format options. An implementation of the callback should format
       string base on these options.
     
     @return
       A string of month after formatted.
     */
    typedef std::function<String(int month, const MonthOptions& options)> GetMonthCallback;
    
    class WeekdayOptions {
    public:
        bool is_abbreviated = false;
    };
    
    /**
     The prototype of callback to get localized string of weekday.
     
     @param weekday
       Numerical value of weekday, from 0 to 6, which 0 is Sunday.
     
     @param options
       Format options. An implementation of the callback should format
       string base on these options.
     
     @return
       A string of weekday after formatted.
     */
    typedef std::function<String(int weekday, const WeekdayOptions& options)> GetWeekdayCallback;
    
    /**
     The prototype of callback to get localized string of a.m./p.m..
     
     @param is_pm
       A value indicates that whether a string of a.m. or p.m. is needed.
     
     @return
       A string of a.m. or p.m..
     */
    typedef std::function<String(bool is_pm)> GetAmPmCallback;
    
    class HourOptions {
    public:
        bool is_12_hour_clock = false;
    };
    
    /**
     The prototype of callback to get localized string of hour.
     
     @param hour
       Numerical value of hour, from 0 to 23.
     
     @param options
       Format options. An implementation of the callback should format
       string base on these options.
     
     @return
       A string of hour after formatted.
     */
    typedef std::function<String(int hour, const HourOptions& options)> GetHourCallback;
    
public:
    /**
     A callback to get localized string of month.
     */
    GetMonthCallback get_month;
    
    /**
     A callback to get localized string of weekday.
     */
    GetWeekdayCallback get_weekday;
    
    /**
     A callback to get localized string of a.m./p.m..
     */
    GetAmPmCallback get_am_pm;
    
    /**
     A callback to get localized string of hour.
     */
    GetHourCallback get_hour;
    
    /**
     A callback to get localized string of minute.
     */
    std::function<String(int minute)> get_minute;
    
    /**
     A callback to get localized string of second.
     */
    std::function<String(int second)> get_second;
};
    
}
