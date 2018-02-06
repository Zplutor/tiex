#include "tiex_match.h"

namespace tiex {
namespace internal {
namespace {
    
std::tm AdjuatTm(const std::tm& tm, const Boundary& boundary) {
    
    if (boundary.value == 0) {
        return tm;
    }
    
    int adjusted_value = boundary.value;
    if (boundary.round && (boundary.unit != Unit::Second)) {
        if (adjusted_value < 0) {
            ++adjusted_value;
        }
    }
    
    auto adjusted_tm = tm;
    
    switch (boundary.unit) {
        case Unit::Second:
            adjusted_tm.tm_sec += adjusted_value;
            break;
        case Unit::Minute:
            adjusted_tm.tm_min += adjusted_value;
            break;
        case Unit::Hour:
            adjusted_tm.tm_hour += adjusted_value;
            break;
        case Unit::Day:
            adjusted_tm.tm_mday += adjusted_value;
            break;
        case Unit::Week:
            adjusted_tm.tm_mday += adjusted_value * 7;
            break;
        case Unit::Month:
            adjusted_tm.tm_mon += adjusted_value;
            break;
        case Unit::Year:
            adjusted_tm.tm_year += adjusted_value;
            break;
        default:
            break;
    }
    
    if (boundary.round) {
        
        switch (boundary.unit) {
            case Unit::Year:
                adjusted_tm.tm_mon = 0;
            case Unit::Month:
                adjusted_tm.tm_mday = 1;
            case Unit::Week:
            case Unit::Day:
                adjusted_tm.tm_hour = 0;
            case Unit::Hour:
                adjusted_tm.tm_min = 0;
            case Unit::Minute:
                adjusted_tm.tm_sec = 0;
                break;
            default:
                break;
        }
        
        if (boundary.unit == Unit::Week) {
            adjusted_tm.tm_mday -= tm.tm_wday;
        }
    }
    
    return adjusted_tm;
}
    
}
    
bool MakeBoundaryTime(const Boundary& boundary, const std::tm& tm, std::time_t& time) {
    
    if (boundary.value == std::numeric_limits<int>::min()) {
        time = std::numeric_limits<std::time_t>::min();
        return true;
    }
    
    if (boundary.value == std::numeric_limits<int>::max()) {
        time = std::numeric_limits<std::time_t>::max();
        return true;
    }
    
    auto adjusted_tm = AdjuatTm(tm, boundary);
    
    auto converted_tm = adjusted_tm;
    time = std::mktime(&converted_tm);
    
    if (time != -1) {
        return true;
    }
    
    //Check for error
    
    //No adjustment was maked to tm, so no error could happend.
    if (boundary.value == 0) {
        return true;
    }
    
    //if std::mktime succeeds, tm would be modified, so check
    //for modifications to determinate success.
    bool is_changed =
        (adjusted_tm.tm_sec != converted_tm.tm_sec) ||
        (adjusted_tm.tm_min != converted_tm.tm_min) ||
        (adjusted_tm.tm_hour != converted_tm.tm_hour) ||
        (adjusted_tm.tm_mday != converted_tm.tm_mday) ||
        (adjusted_tm.tm_mon != converted_tm.tm_mon) ||
        (adjusted_tm.tm_year != converted_tm.tm_year);
    
    if (is_changed) {
        return true;
    }
    
    return false;
}
    
    
bool MatchCondition(
    const Condition& condition,
    const Time& referenced_time,
    const Time& formatted_time,
    bool& is_matched) {
    
    is_matched = false;
    
    auto referenced_tm = referenced_time.GetTm();
    if (referenced_tm == nullptr) {
        return false;
    }
    
    auto formatted_timet = formatted_time.GetTimet();
    
    std::time_t backward_time = 0;
    bool is_succeeded = internal::MakeBoundaryTime(condition.backward, *referenced_tm, backward_time);
    if (! is_succeeded) {
        return false;
    }
    
    if (backward_time <= formatted_timet) {
        
        std::time_t forward_time = 0;
        is_succeeded = internal::MakeBoundaryTime(condition.forward, *referenced_tm, forward_time);
        if (! is_succeeded) {
            return false;
        }
        
        if (formatted_timet <= forward_time) {
            is_matched = true;
        }
    }
    
    return true;
}

    
}
}
