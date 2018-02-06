#pragma once

#include <ctime>
#include "tiex_config.h"
#include "tiex_error.h"
#include "tiex_expression.h"

namespace tiex {

class Formatter {
public:
    static Formatter Create(const String& string);
    static Formatter Create(const String& string, ParseError& parse_error);
    
public:
    Formatter();
    Formatter(const Expression& expression);
    Formatter(Expression&& expression);
    
    String Format(std::time_t referenced_time, std::time_t formatted_time);
    String Format(std::time_t referenced_time, std::time_t formatted_time, FormatError& format_error);
    
private:
    Expression expression_;
};

}

