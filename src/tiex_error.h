#pragma once

#include "tiex_config.h"

namespace tiex {

class ParseError {
public:
    enum class Status {
        None,
        UnexpectedEnd,
        UnexpectedToken,
        ConversionFailed,
    };
    
public:
    void Clear() {
        status = Status::None;
        index = 0;
        token.clear();
    }
    
public:
    Status status = Status::None;
    std::size_t index = 0;
    String token;
};


class FormatError {
public:
    enum class Status {
        None,
        NoMatchedRule,
        TimeError,
    };
    
public:
    void Clear() {
        status = Status::None;
    }
    
public:
    Status status = Status::None;
};
    
}
