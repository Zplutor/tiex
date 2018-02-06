#pragma once

#include <string>

namespace tiex {
    
#ifndef TIEX_USE_WCHAR
    #ifdef WIN32
        #define TIEX_USE_WCHAR 1
    #else
        #define TIEX_USE_WCHAR 0
    #endif
#endif
    
#if TIEX_USE_WCHAR
    typedef wchar_t Char;
    #define TIEX_LITERAL(x) L##x
#else
    typedef char Char;
    #define TIEX_LITERAL(x) x
#endif

typedef std::basic_string<Char> String;
#define TIEX_STRING(x) tiex::String(TIEX_LITERAL(x))
    
}
