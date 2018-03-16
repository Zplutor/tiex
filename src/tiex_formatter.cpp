#include "tiex_formatter.h"
#include <cassert>
#include "tiex_generate.h"
#include "tiex_match.h"
#include "tiex_parser.h"

namespace tiex {

Formatter Formatter::Create(const String& string) {
    
    ParseError error;
    auto formatter = Create(string, error);
    
    assert(error.status == ParseError::Status::None);
    
    return formatter;
}


Formatter Formatter::Create(const String& string, ParseError& parse_error) {
    
    internal::Scanner scanner(string.c_str(), string.length());
    internal::Parser parser(scanner);
    
    auto expression = parser.Parse();
    parse_error = parser.GetParseError();
    
    return Formatter(std::move(expression));
}


Formatter::Formatter() {
    
}


Formatter::Formatter(const Expression& expression) : expression_(expression) {
    
}


Formatter::Formatter(Expression&& expression) : expression_(std::forward<Expression>(expression)) {
    
}

    
String Formatter::Format(std::time_t referenced_time, std::time_t formatted_time, const Locale& locale) {
    
    FormatError format_error;
    auto string = Format(referenced_time, formatted_time, locale, format_error);
    
    assert(format_error.status == FormatError::Status::None);
    
    return string;
}
    

String Formatter::Format(std::time_t referenced_time, std::time_t formatted_time, const Locale& locale, FormatError& format_error) {
    
    if (expression_.rules.empty()) {
        format_error.status = FormatError::Status::NoMatchedRule;
        return String();
    }
    
    internal::Time referenced(referenced_time);
    internal::Time formatted(formatted_time);
    
    for (const auto& each_rule : expression_.rules) {
        
        bool is_matched = false;
        bool is_succeeded = internal::MatchCondition(each_rule.condition, referenced, formatted, is_matched);
        if (! is_succeeded) {
            format_error.status = FormatError::Status::TimeError;
            return String();
        }
        
        if (is_matched) {
            
            String result_text;
            bool is_succeeded = GenerateResultText(each_rule.result, referenced, formatted, locale, result_text);
            if (! is_succeeded) {
                format_error.status = FormatError::Status::TimeError;
                return String();
            }
            
            return result_text;
        }
    }

    format_error.status = FormatError::Status::NoMatchedRule;
    return String();
}

}
