#include "tiex_difference.h"
#include "tiex_generate.h"

namespace tiex {

int Difference(std::time_t time1, std::time_t time2, Unit unit) {
    long difference = 0;
    //Note: the order of operands are reversed.
    internal::GetTimeDifference(unit, internal::Time(time2), internal::Time(time1), difference);
    return static_cast<int>(difference);
}

}