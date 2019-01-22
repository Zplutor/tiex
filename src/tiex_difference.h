#pragma once

#include <ctime>
#include "tiex_unit.h"

namespace tiex {

/**
 Calculate the difference of two time points by specified time unit.

 This function is equivalent to "time1 - time2".
 */
int Difference(std::time_t time1, std::time_t time2, Unit unit);

}