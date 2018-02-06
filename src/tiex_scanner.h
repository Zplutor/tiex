#pragma once

#include "tiex_config.h"

namespace tiex {
namespace internal {

class Scanner {
public:
    Scanner(const Char* string, std::size_t length);
    
    bool GetChar(Char& ch) const;
    
    bool ReadChar(Char& ch);
    bool ReadWord(String& word);
    bool ReadNumber(String& number);
    
    void SkipWhiteSpaces();
    
    std::size_t GetCurrentIndex() const {
        return cursor_ - begin_;
    }
    
    bool IsEnd() const {
        return cursor_ == end_;
    }
    
private:
    const Char* begin_;
    const Char* end_;
    const Char* cursor_;
};

}
}
