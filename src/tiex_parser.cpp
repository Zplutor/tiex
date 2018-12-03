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
    
    
}

    
bool Parser::ParseRule(Rule& rule) {
    
    
}

    
bool Parser::ParseCondition(Condition& condition) {
    
    
}

    
bool Parser::ParseBoundary(bool is_forward, Boundary& boundary) {
    
    
}

    
bool Parser::ParseRound(bool& round) {
    

}

    
bool Parser::ParseUnit(Unit& unit) {
    
    
}

   
}
}
