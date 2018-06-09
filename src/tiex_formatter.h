#pragma once

#include <ctime>
#include "tiex_config.h"
#include "tiex_error.h"
#include "tiex_expression.h"
#include "tiex_locale.h"

namespace tiex {

/**
 A formatter represents a format expression that has been parsed,
 and is used to format relative times to string by the meaning of
 the expression.
 
 Typically, to create a formmater, you should write a format expression as
 string and pass it to the static function Create. You should reuse one
 formatter whenever possible if the same expression is needed for multiple
 times, to avoid re-parsing and performance.
 */
class Formatter {
public:
    /**
     Create a formatter.
     
     @param string
       An expression string that to be parsed.
     
     @return
       An empty formatter if fail to parse the expression.
     */
    static Formatter Create(const String& string);
    
    /**
     Create a formatter and catch parse error.
     
     @param string
       An expression string that to be parsed.
     
     @param parse_error
       An output parameter that stores information about parse error.
     
     @return
       An empty formatter if fail to parse the expression.
    */
    static Formatter Create(const String& string, ParseError& parse_error);
    
public:
    /**
     Construct an empty formatter.
     */
    Formatter();
    
    /**
     Construct a formatter with an expression, by copying its content.
     */
    Formatter(const Expression& expression);
    
    /**
     Construct a formatter with an expression, by moving its content.
     */
    Formatter(Expression&& expression);
    
    /**
     Format times with locale information and catch format error.
     
     @param referenced_time
       The referenced time that is used to compare to the formatted time.
     
     @param formatted_time
       The target time to be formatted to string.
     
     @param locale
       Contains localization information that affect format result.
     
     @param format_error
       An output parameter that stores information about format error.
     
     @return
       A format result string. An empty string is returned if fail to format.
     */
    String Format(
        std::time_t referenced_time,
        std::time_t formatted_time,
        const Locale& locale,
        FormatError& format_error);
    
    /**
     Format times and catch format error.
     
     @param referenced_time
       The referenced time that is used to compare to the formatted time.
     
     @param formatted_time
       The target time to be formatted to string.
     
     @param format_error
       An output parameter that stores information about format error.
     
     @return
       A format result string. An empty string is returned if fail to format.
     */
    String Format(std::time_t referenced_time, std::time_t formatted_time, FormatError& format_error) {
        return Format(referenced_time, formatted_time, Locale(), format_error);
    }
    
    /**
     Format times with locale information.
     
     @param referenced_time
       The referenced time that is used to compare to the formatted time.
     
     @param formatted_time
       The target time to be formatted to string.
     
     @param locale
       Contains localization information that affect format result.
     
     @return
       A format result string. An empty string is returned if fail to format.
     */
    String Format(std::time_t referenced_time, std::time_t formatted_time, const Locale& locale);
    
    /**
     Format times.
     
     @param referenced_time
       The referenced time that is used to compare to the formatted time.
     
     @param formatted_time
       The target time to be formatted to string.
     
     @return
       A format result string. An empty string is returned if fail to format.
     */
    String Format(std::time_t referenced_time, std::time_t formatted_time) {
        return Format(referenced_time, formatted_time, Locale());
    }
    
private:
    Expression expression_;
};

}

