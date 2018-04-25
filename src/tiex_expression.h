#pragma once

#include <map>
#include <vector>
#include "tiex_config.h"

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


class Result {
public:
    std::vector<String> texts;
    std::map<std::size_t, Specifier> specifiers;
    bool has_standard_specifiers = false;
};


class Rule {
public:
    Condition condition;
    Result result;
};


class Expression {
public:
    std::vector<Rule> rules;
};

}
