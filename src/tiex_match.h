#pragma once

#include <ctime>
#include "tiex_expression.h"
#include "tiex_time.h"

namespace tiex {
namespace internal {

bool MakeBoundaryTime(const Boundary& boundary, const std::tm& tm, std::time_t& time);
    
bool MatchCondition(
    const Condition& condition,
    const Time& referenced_time,
    const Time& formatted_time,
    bool& is_matched);

}
}
