#include <gtest/gtest.h>
#include "test_utility.h"
#include "tiex.h"

TEST(Case, Example) {

    auto formatter = tiex::Formatter::Create(
        TIEX_LITERAL("[0,*]{Future}")
        TIEX_LITERAL("[-1~min,0]{Just now}")
        TIEX_LITERAL("[-1~h,0]{%~min minute(s) ago}")
        TIEX_LITERAL("[-1.d,0]{%H:%M}")
        TIEX_LITERAL("[-2.d,0]{Yesterday}")
        TIEX_LITERAL("[-3.d,0]{The day before yesterday}")
        TIEX_LITERAL("[-1~w,0]{%A}")
        TIEX_LITERAL("[-1.y,0]{%m-%d}")
        TIEX_LITERAL("[*,0]{%Y-%m-%d}")
    );

    auto referenced_time = MakeTime(2018, 2, 6, 13, 43, 32);
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 7, 0, 0, 0)), TIEX_STRING("Future"));
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 6, 13, 43, 23)), TIEX_STRING("Just now"));
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 6, 12, 53, 0)), TIEX_STRING("50 minute(s) ago"));
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 6, 1, 2, 3)), TIEX_STRING("01:02"));
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 5, 22, 10, 2)), TIEX_STRING("Yesterday"));
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 4, 0, 42, 53)), TIEX_STRING("The day before yesterday"));
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 2, 2, 0, 0, 0)), TIEX_STRING("Friday"));
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2018, 1, 21, 0, 0, 0)), TIEX_STRING("01-21"));
    ASSERT_EQ(formatter.Format(referenced_time, MakeTime(2017, 6, 27, 0, 0, 0)), TIEX_STRING("2017-06-27"));
}