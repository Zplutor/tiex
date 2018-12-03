#pragma once

#include <map>
#include <string>
#include <vector>

namespace tiex {

enum class Unit {
    Second,
    Minute,
    Hour,
    Day,
    Week,
    Month,
    Year,
};

    
class Boundary {
public:
    int value = 0;
    bool round = false;
    Unit unit = Unit::Second;
};


class Condition {
public:
    Boundary backward;
    Boundary forward;
};


class Specifier {
public:
    Unit uint = Unit::Second;
};


template<typename C>
class BasicResult {
public:
    std::vector<std::basic_string<C>> texts;
    std::map<std::size_t, Specifier> specifiers;
    bool has_standard_specifiers = false;
};

using Result = BasicResult<char>;
using WideResult = BasicResult<wchar_t>;


template<typename C>
class BasicRule {
public:
    Condition condition;
    BasicResult<C> result;
};

using Rule = BasicRule<char>;
using WideRule = BasicRule<wchar_t>;


template<typename C>
class BasicExpression {
public:
    std::vector<BasicRule<C>> rules;
};

using Expression = BasicExpression<char>;
using WideExpression = BasicExpression<wchar_t>;

}
