#pragma once

#include "tiex_config.h"
#include "tiex_error.h"
#include "tiex_expression.h"
#include "tiex_scanner.h"

namespace tiex {
namespace internal {

class Parser {
public:
    Parser(Scanner& scanner);
    
    Expression Parse();
    
    const ParseError& GetParseError() const {
        return parse_error_;
    }
    
//These private mehtods are declared as public just for testing purpose.
public:
    bool ParseExpression(Expression& expression);
    bool ParseRule(Rule& rule);
    bool ParseCondition(Condition& condition);
    bool ParseBoundary(bool is_forward, Boundary& boundary);
    bool ParseRound(bool& round);
    bool ParseUnit(Unit& unit);
    bool ParseResult(Result& result);
    bool ParseChar(Char expected_char);
    bool ParseNumber(int& value);
    bool ParseWord(String& word);
    
private:
    void SetError(ParseError::Status status, const String& token, int index_adjustment);
    
    void SetError(ParseError::Status status) {
        SetError(status, String(), 0);
    }
    
    void SetError(ParseError::Status status, const String& token) {
        SetError(status, token, 0);
    }
    
    void SetError(ParseError::Status status, Char ch) {
        SetError(status, String(1, ch), 0);
    }
    
    void SetError(ParseError::Status status, Char ch, int index_adjustment) {
        SetError(status, String(1, ch), index_adjustment);
    }
    
private:
    Scanner& scanner_;
    ParseError parse_error_;
};

}
}
