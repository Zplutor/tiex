#pragma once

#include <functional>
#include "tiex_config.h"

namespace tiex {
    
class Locale {
public:
    std::function<String(int weekday, bool abbreviated)> get_weekday;
    std::function<String(bool is_pm)> get_am_pm;
};
    
}
