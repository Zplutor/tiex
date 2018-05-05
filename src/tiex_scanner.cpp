#include "tiex_scanner.h"
#include <cwctype>

namespace tiex {
namespace internal {
namespace {

/**
 Determinate whether the specified character is an ASCII alpha.

 std::iswalpha in MSVC always returns unexpected result with Chinese 
 character no matter what locale is set, so a rewritten version here
 is used.
 */
inline bool IsAlpha(Char ch) {
    return 
        ((TIEX_LITERAL('a') <= ch) && (ch <= TIEX_LITERAL('z'))) ||
        ((TIEX_LITERAL('A') <= ch) && (ch <= TIEX_LITERAL('Z')));
}

}

#if TIEX_USE_WCHAR
    #define IsDigit std::iswdigit
    #define IsSpace std::iswspace
#else
    #define IsDigit std::isdigit
    #define IsSpace std::isspace
#endif

    
Scanner::Scanner(const Char* string, std::size_t length) :
    begin_(string),
    end_(string + length),
    cursor_(string) {
    
}

    
bool Scanner::GetChar(Char& ch) const {
    
    if (cursor_ == end_) {
        return false;
    }
    
    ch = *cursor_;
    return true;
}

    
bool Scanner::ReadChar(Char& ch) {
    
    if (cursor_ == end_) {
        return false;
    }
    
    ch = *cursor_;
    ++cursor_;
    return true;
}

    
bool Scanner::ReadWord(String& word) {
    
    String temp_word;
    
    while (cursor_ != end_) {
        
        auto current_char = *cursor_;
        if (IsAlpha(current_char)) {
            temp_word.push_back(current_char);
            ++cursor_;
        }
        else {
            break;
        }
    }
    
    if (temp_word.empty()) {
        return false;
    }
    
    word = temp_word;
    return true;
}

    
bool Scanner::ReadNumber(String& number) {
    
    String temp_number;
    bool has_sign = false;
    
    Char current_char = *cursor_;
    if (current_char == '-' || current_char == '+') {
        temp_number.push_back(current_char);
        has_sign = true;
        ++cursor_;
    }
    
    while (cursor_ != end_) {
        current_char = *cursor_;
        if (IsDigit(current_char)) {
            temp_number.push_back(current_char);
            ++cursor_;
        }
        else {
            break;
        }
    }
    
    bool is_succeeded = false;
    if (has_sign) {
        is_succeeded = temp_number.length() > 1;
    }
    else {
        is_succeeded = ! temp_number.empty();
    }
    
    if (is_succeeded) {
        number = temp_number;
    }
    
    return is_succeeded;
}

    
void Scanner::SkipWhiteSpaces() {
    
    while (cursor_ != end_) {
        if (IsSpace(*cursor_)) {
            ++cursor_;
        }
        else {
            break;
        }
    }
}

}
}
