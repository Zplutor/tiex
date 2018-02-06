#include "tiex_generate.h"
#include <cassert>
#include <iomanip>
#include <sstream>

namespace tiex {
namespace internal {

#if TIEX_USE_WCHAR
    #define ToString std::to_wstring
#else
    #define ToString std::to_string
#endif
    

long GetDifferenceWithTimet(const Specifier& specifier, std::time_t referenced_time, std::time_t formatted_time) {
    
    long difference = static_cast<long>(formatted_time - referenced_time);
    
    switch (specifier.uint) {
            
        case Unit::Week:
            difference /= 7;
            
        case Unit::Day:
            difference /= 24;
            
        case Unit::Hour:
            difference /= 60;
            
        case Unit::Minute:
            difference /= 60;
            
        case Unit::Second:
            break;
            
        default:
            assert(false);
            difference = 0;
            break;
    }
    
    return difference;
}
    
    
long GetDifferenceWithTm(const Specifier& specifier, const std::tm& referenced_tm, const std::tm& formatted_tm) {
    
    auto compare_tm = [](const std::tm& tm1, const std::tm& tm2, bool compare_month) {
        
        int result = 0;
        
        if (compare_month) {
            result = tm1.tm_mon - tm2.tm_mon;
            if (result != 0) {
                return result;
            }
        }
        
        result = tm1.tm_mday - tm2.tm_mday;
        if (result != 0) {
            return result;
        }
        
        result = tm1.tm_hour - tm2.tm_hour;
        if (result != 0) {
            return result;
        }
        
        result = tm1.tm_min - tm2.tm_min;
        if (result != 0) {
            return result;
        }
        
        result = tm1.tm_sec - tm2.tm_sec;
        return result;
    };
    
    long difference = 0;
    long adjustment = 0;
    
    switch (specifier.uint) {
            
        case Unit::Month:
            difference = formatted_tm.tm_year - referenced_tm.tm_year;
            difference = (difference * 12) + (formatted_tm.tm_mon - referenced_tm.tm_mon);
            adjustment = compare_tm(formatted_tm, referenced_tm, false);
            break;
            
        case Unit::Year:
            difference = formatted_tm.tm_year - referenced_tm.tm_year;
            adjustment = compare_tm(formatted_tm, referenced_tm, true);
            break;
            
        default:
            assert(false);
            break;
    }
    
    if (difference < 0) {
        if (adjustment > 0) {
            ++difference;
        }
    }
    else if (difference > 0) {
        if (adjustment < 0) {
            --difference;
        }
    }
    
    return difference;
}
    
 
bool GetTimeDifference(
    const Specifier& specifier,
    const Time& reference_time,
    const Time& formatted_time,
    long& difference) {
    
    switch (specifier.uint) {
            
        case Unit::Month:
        case Unit::Year: {
            
            auto referenced_tm = reference_time.GetTm();
            if (referenced_tm == nullptr) {
                return false;
            }
            
            auto formatted_tm = formatted_time.GetTm();
            if (formatted_tm == nullptr) {
                return false;
            }
            
            difference = GetDifferenceWithTm(specifier, *referenced_tm, *formatted_tm);
            return true;
        }
            
        default:
            difference = GetDifferenceWithTimet(specifier, reference_time.GetTimet(), formatted_time.GetTimet());
            return true;
    }
}
    
    
bool GenerateTextWithSpecifier(
    const Specifier& specifier,
    const Time& reference_time,
    const Time& formatted_time,
    String& text) {

    long difference = 0;
    bool is_succeeded = GetTimeDifference(specifier, reference_time, formatted_time, difference);
    if (! is_succeeded) {
        return false;
    }
    
    text = ToString(std::abs(difference));
    return true;
}

    
bool GenerateResultText(
    const Result& result,
    const Time& reference_time,
    const Time& formatted_time,
    String& text) {
    
    String result_text;
    
    for (std::size_t index = 0; index < result.texts.size(); ++index) {
        
        const auto& each_text = result.texts[index];
        if (each_text.empty()) {
            
            auto iterator = result.specifiers.find(index);
            if (iterator != result.specifiers.end()) {
                
                long difference = 0;
                bool is_succeeded = GetTimeDifference(iterator->second, reference_time, formatted_time, difference);
                if (! is_succeeded) {
                    return false;
                }
                
                result_text.append(ToString(std::abs(difference)));
            }
        }
        else {
            result_text.append(each_text);
        }
    }
    
    if (result.has_standard_specifiers) {
        
        auto formatted_tm = formatted_time.GetTm();
        if (formatted_tm == nullptr) {
            return false;
        }
        
        std::basic_ostringstream<Char> stream;
        stream << std::put_time(formatted_tm, result_text.c_str());
        result_text = stream.str();
    }
    
    text = result_text;
    return true;
}


}
}
