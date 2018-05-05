#pragma once

#include <functional>
#include "tiex_config.h"

namespace tiex {
    
class Locale {
public:
    class MonthOptions {
    public:
        bool is_abbreviated = false;
        bool is_number = false;
    };
    
public:
    std::function<String(int month, const MonthOptions& options)> get_month;
    std::function<String(int weekday, bool abbreviated)> get_weekday;
    std::function<String(bool is_pm)> get_am_pm;
};
    
}
