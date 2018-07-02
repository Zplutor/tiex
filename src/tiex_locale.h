#pragma once

#include <functional>
#include "tiex_config.h"

namespace tiex {
    
/**
 A locale contains information that help to format localized time string.
 */
class Locale {
public:
    /**
     Contains options to format localized string of month.
     */
    class MonthOptions {
    public:
        /**
         Indicates that whether a text of number is expected.
         
         If it is true, corresponds to %m specifier, otherwise corresponds to %b, %B or %h specifiers.
         */
        bool is_number = false;
        
        /**
         Indicates that whether an abbreviated result is expected.
         
         If it is true, corresponds to %b or %h specifiers, otherwise corresponds to %B specifier.
         */
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
    
    /**
     Contains options to format localized string of weekday.
     */
    class WeekdayOptions {
    public:
        /**
         Indicates that whether an abbreviated result is expected.
         
         If it is true, corresponds to %a specifier, otherwise corresponds to %A specifier.
         */
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
    
    /**
     Contains options to format localized string of hour.
     */
    class HourOptions {
    public:
        /**
         Indicates that whether the hour is based on 12 hour clock.
         
         If it is true, corresponds to %I specifier, otherwise corresponds to %H specifier.
         */
        bool is_12_hour_clock = false;
    };
    
    /**
     The prototype of callback to get localized string of hour.
     
     @param hour
       Numerical value of hour, from 0 to 23 on 24 hour clock, from 1 to 12 on 12 hour clock.
     
     @param options
       Format options. An implementation of the callback should format
       string base on these options.
     
     @return
       A string of hour after formatted.
     */
    typedef std::function<String(int hour, const HourOptions& options)> GetHourCallback;
    
    /**
     The prototype of callback to get localized string of minute.
     
     @param minute
       Numerical value of minute, from 0 to 59.
     
     @return
       A string of minute after formatted.
     */
    typedef std::function<String(int minute)> GetMinuteCallback;
    
    /**
     The prototype of callback to get localized string of second.
     
     @param second
       Numerical value of second, from 0 to 59.
     
     @return
       A string of second after formatted.
     */
    typedef std::function<String(int second)> GetSecondCallback;
    
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
    GetMinuteCallback get_minute;
    
    /**
     A callback to get localized string of second.
     */
    GetSecondCallback get_second;
};
    
}
