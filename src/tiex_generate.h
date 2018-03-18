#pragma once

#include <ctime>
#include "tiex_expression.h"
#include "tiex_locale.h"
#include "tiex_time.h"

namespace tiex {
namespace internal {

long GetDifferenceWithTimet(const Specifier& specifier, std::time_t referenced_time, std::time_t formatted_time);
long GetDifferenceWithTm(const Specifier& specifier, const std::tm& referenced_tm, const std::tm& formatted_tm);
    
bool GetLocaleText(Char specifier_char, const std::tm& formatted_tm, const Locale& locale, String& locale_text);
bool OverrideStandardSpecifiers(const std::tm& formatted_tm, const Locale& locale, String& result_text);
    
bool GenerateResultText(
    const Result& result,
    const Time& reference_time,
    const Time& formatted_time,
    const Locale& locale,
    String& text);
    
}
}
