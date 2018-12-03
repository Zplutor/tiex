#include "tiex_scanner.h"


namespace tiex {
namespace internal {
namespace {


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

 
    
bool Scanner::ReadNumber(String& number) {
    
    
}

    
void Scanner::SkipWhiteSpaces() {
    
    
}

}
}
