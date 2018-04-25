#include "tiex_parser.h"

namespace tiex {
namespace internal {

Parser::Parser(Scanner& scanner) :
    scanner_(scanner) {
    
}

    
Expression Parser::Parse() {
    
    parse_error_.Clear();
    
    Expression expression;
    ParseExpression(expression);
    return expression;
}

    
bool Parser::ParseExpression(Expression& expression) {
    
    std::vector<Rule> rules;
    
    scanner_.SkipWhiteSpaces();
    
    bool is_succeeded = false;
    do {
        
        Rule rule;
        is_succeeded = ParseRule(rule);
        if (! is_succeeded) {
            break;
        }
        
        rules.push_back(rule);
        
        scanner_.SkipWhiteSpaces();
    }
    while (! scanner_.IsEnd());
    
    if (is_succeeded) {
        expression.rules = std::move(rules);
    }
    
    return is_succeeded;
}

    
bool Parser::ParseRule(Rule& rule) {
    
    Condition condition;
    if (! ParseCondition(condition)) {
        return false;
    }
    
    scanner_.SkipWhiteSpaces();
    
    Result result;
    if (! ParseResult(result)) {
        return false;
    }
    
    rule.condition = std::move(condition);
    rule.result = std::move(result);
    return true;
}

    
bool Parser::ParseCondition(Condition& condition) {
    
    if (! ParseChar('[')) {
        return false;
    }
    
    scanner_.SkipWhiteSpaces();
    
    Boundary backward;
    if (! ParseBoundary(false, backward)) {
        return false;
    }
    
    scanner_.SkipWhiteSpaces();
    
    if (! ParseChar(',')) {
        return false;
    }
    
    scanner_.SkipWhiteSpaces();
    
    Boundary forward;
    if (! ParseBoundary(true, forward)) {
        return false;
    }
    
    scanner_.SkipWhiteSpaces();
    
    if (! ParseChar(']')) {
        return false;
    }
    
    condition.backward = std::move(backward);
    condition.forward = std::move(forward);
    return true;
}

    
bool Parser::ParseBoundary(bool is_forward, Boundary& boundary) {
    
    Char ch = 0;
    if (! scanner_.GetChar(ch)) {
        SetError(ParseError::Status::UnexpectedEnd);
        return false;
    }
    
    if (ch == '0') {

        boundary.value = 0;
        scanner_.ReadChar(ch);
        return true;
    }
    else if (ch == '*') {
        
        boundary.value =
            is_forward ?
            std::numeric_limits<int>::max() :
            std::numeric_limits<int>::min();
        
        scanner_.ReadChar(ch);
        return true;
    }
    
    int value = 0;
    if (! ParseNumber(value)) {
        return false;
    }
    
    scanner_.SkipWhiteSpaces();
    
    bool round = false;
    if (! ParseRound(round)) {
        return false;
    }
    
    scanner_.SkipWhiteSpaces();
    
    Unit unit = Unit::Second;
    if (! ParseUnit(unit)) {
        return false;
    }
    
    boundary.value = value;
    boundary.round = round;
    boundary.unit = unit;
    return true;
}

    
bool Parser::ParseRound(bool& round) {
    
    Char ch = 0;
    if (! scanner_.ReadChar(ch)) {
        SetError(ParseError::Status::UnexpectedEnd, ch);
        return false;
    }
    
    if (ch == '.') {
        round = true;
        return true;
    }
    
    if (ch == '~') {
        round = false;
        return true;
    }
    
    SetError(ParseError::Status::UnexpectedToken, ch, -1);
    return false;
}

    
bool Parser::ParseUnit(Unit& unit) {
    
    String word;
    if (! ParseWord(word)) {
        return false;
    }
    
    const struct {
        const Char* word;
        Unit unit;
    } unit_map[] = {
        { TIEX_LITERAL("s"),   Unit::Second },
        { TIEX_LITERAL("min"), Unit::Minute },
        { TIEX_LITERAL("h"),   Unit::Hour },
        { TIEX_LITERAL("d"),   Unit::Day },
        { TIEX_LITERAL("w"),   Unit::Week },
        { TIEX_LITERAL("mth"), Unit::Month },
        { TIEX_LITERAL("y"),   Unit::Year },
    };
    
    bool is_found = false;
    
    for (const auto& each_item : unit_map) {
        if (word == each_item.word) {
            unit = each_item.unit;
            is_found = true;
            break;
        }
    }
    
    if (! is_found) {
        SetError(ParseError::Status::UnexpectedToken, word, - static_cast<int>(word.length()));
    }
    
    return is_found;
}

    
bool Parser::ParseResult(Result& result) {
    
    if (! ParseChar('{')) {
        return false;
    }
    
    bool is_succeeded = true;
    std::vector<String> texts;
    std::map<std::size_t, Specifier> specifiers;
    bool has_standard_specifiers = false;
    String current_text;
    
    while (true) {
        
        Char ch = 0;
        if (! scanner_.ReadChar(ch)) {
            SetError(ParseError::Status::UnexpectedEnd);
            is_succeeded = false;
            break;
        }
        
        if (ch == '}') {
            if (! current_text.empty()) {
                texts.push_back(current_text);
            }
            break;
        }
        
        if (ch == '%') {
            
            if (! scanner_.ReadChar(ch)) {
                SetError(ParseError::Status::UnexpectedEnd);
                is_succeeded = false;
                break;
            }
            
            if (ch == '~') {
                
                Unit unit = Unit::Second;
                if (! ParseUnit(unit)) {
                    is_succeeded = false;
                    break;
                }
                
                if (! current_text.empty()) {
                    texts.push_back(current_text);
                    current_text.clear();
                }
                
                auto specifier_index = texts.size();
                texts.push_back(String());
                
                Specifier specifier;
                specifier.uint = unit;
                specifiers[specifier_index] = specifier;
            }
            else {
                has_standard_specifiers = true;
                current_text.push_back('%');
                current_text.push_back(ch);
            }
        }
        else {
            current_text.push_back(ch);
        }
    }
    
    if (is_succeeded) {
        result.texts = std::move(texts);
        result.specifiers = std::move(specifiers);
        result.has_standard_specifiers = has_standard_specifiers;
    }
    
    return is_succeeded;
}

    
bool Parser::ParseChar(Char expected_char) {
    
    Char read_char = 0;
    if (! scanner_.GetChar(read_char)) {
        SetError(ParseError::Status::UnexpectedEnd);
        return false;
    }
    
    if (read_char != expected_char) {
        SetError(ParseError::Status::UnexpectedToken, read_char);
        return false;
    }
    
    scanner_.ReadChar(read_char);
    return true;
}

    
bool Parser::ParseNumber(int& value) {
    
    String number;
    if (! scanner_.ReadNumber(number)) {
        
        Char last_char = 0;
        if (scanner_.GetChar(last_char)) {
            SetError(ParseError::Status::UnexpectedToken, last_char);
        }
        else {
            SetError(ParseError::Status::UnexpectedEnd);
        }
        return false;
    }
    
    try {
        value = std::stoi(number);
        return true;
    }
    catch (...) {
        SetError(ParseError::Status::ConversionFailed, number, - static_cast<int>(number.length()));
        return false;
    }
}

    
bool Parser::ParseWord(String& word) {
    
    if (! scanner_.ReadWord(word)) {
        
        Char last_char = 0;
        if (scanner_.GetChar(last_char)) {
            SetError(ParseError::Status::UnexpectedToken, last_char);
        }
        else {
            SetError(ParseError::Status::UnexpectedEnd);
        }
        return false;
    }
    
    return true;
}


void Parser::SetError(ParseError::Status status, const String& token, int index_adjustment) {
    parse_error_.status = status;
    parse_error_.token = token;
    parse_error_.index = scanner_.GetCurrentIndex() + index_adjustment;
}

}
}
