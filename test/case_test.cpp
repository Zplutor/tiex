#include <gtest/gtest.h>
#include "test_utility.h"
#include "tiex.h"

TEST(Case, Example) {

    auto formatter = tiex::Formatter::Create(
        "[0,*]{Future}"
        "[-1~min,0]{Just now}"
        "[-1~h,0]{%~min minute(s) ago}"
        "[-1.d,0]{%H:%M}"
        "[-2.d,0]{Yesterday}"
        "[-3.d,0]{The day before yesterday}"
        "[-1~w,0]{%A}"
        "[-1.y,0]{%m-%d}"
        "[*,0]{%Y-%m-%d}"
    );

    auto referenced_time = MakeTime(2018, 2, 6, 13, 43, 32);
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 7, 0, 0, 0)), "Future");
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 6, 13, 43, 23)), "Just now");
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 6, 12, 53, 0)), "50 minute(s) ago");
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 6, 1, 2, 3)), "01:02");
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 5, 22, 10, 2)), "Yesterday");
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 4, 0, 42, 53)), "The day before yesterday");
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 2, 0, 0, 0)), "Friday");
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 1, 21, 0, 0, 0)), "01-21");
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2017, 6, 27, 0, 0, 0)), "2017-06-27");
}