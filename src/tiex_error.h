#pragma once

namespace tiex {

/*
 Represents an error when parsing format expression.
 */
class ParseError {
public:
    /**
     Describes the reason of parse error.
     */
    enum class Status {
        
        /**
         No error, means that parsing is succeeded.
         */
        None,
        
        /**
         The format expression ends unexpectedly.
         */
        UnexpectedEnd,
        
        /**
         An unexpected token is met.
         
         Typically this means a syntax error.
         */
        UnexpectedToken,
        
        /**
         The number in expression is fail to convert to numerical value.
         */
        ConversionFailed,
    };
    
public:
    /**
     Clear error information, reset to default state.
     */
    void Clear() {
        status = Status::None;
        index = 0;
    }
    
public:
    /**
     Status of the error.
     */
    Status status = Status::None;
    
    /**
     The parsing position in format expression when the error occurs.
     */
    std::size_t index = 0;
};


/**
 Represents an error when formatting times.
 */
class FormatError {
public:
    /**
     Describes the reason of format error.
     */
    enum class Status {
        
        /**
         No error, means that formatting is succeeded.
         */
        None,
        
        /**
         No rule that matches the relationship of times.
         */
        NoMatchedRule,
        
        /**
         A general status that represents errors related to time conversion.
         */
        TimeError,
    };
    
public:
    /**
     Clear error information, reset to default state.
     */
    void Clear() {
        status = Status::None;
    }
    
public:
    /**
     Status of the error.
     */
    Status status = Status::None;
};
    
}
