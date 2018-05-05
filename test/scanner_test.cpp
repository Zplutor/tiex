#include <gtest/gtest.h>
#include "tiex_scanner.h"

using namespace tiex;
using namespace tiex::internal;

TEST(Scanner, SkipWhiteSpaces) {
    
    auto test = [](const String& string, std::size_t expected_index) {
        Scanner scanner(string.c_str(), string.length());
        scanner.SkipWhiteSpaces();
        return scanner.GetCurrentIndex() == expected_index;
    };
    
    ASSERT_TRUE(test(TIEX_STRING(" \r\n\ta "), 4));
    ASSERT_TRUE(test(TIEX_STRING("23 "), 0));
}


TEST(Scanner, ReadChar) {
    
    {
        auto string(TIEX_STRING(" 123abc\n"));
        Scanner scanner(string.c_str(), string.length());
        
        for (std::size_t index = 0; index != string.length(); ++index) {
            
            Char ch = 0;
            bool is_succeeded = scanner.ReadChar(ch);
            ASSERT_TRUE(is_succeeded);
            ASSERT_EQ(ch, string[index]);
            ASSERT_EQ(index + 1, scanner.GetCurrentIndex());
        }
        
        Char ch = 0;
        bool is_succeeded = scanner.ReadChar(ch);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(string.length(), scanner.GetCurrentIndex());
    }
    
    {
        auto string = TIEX_STRING("");
        Scanner scanner(string.c_str(), string.length());
        Char ch = 10;
        bool is_succeeded = scanner.ReadChar(ch);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(ch, 10);
    }
}


TEST(Scanner, ReadWord) {
    
    auto test_succeess = [](const String& string, const String& expected) {
        Scanner scanner(string.c_str(), string.length());
        String word;
        bool is_succeeded = scanner.ReadWord(word);
        if (! is_succeeded) {
            return false;
        }
        if (word != expected) {
            return false;
        }
        return expected.length() == scanner.GetCurrentIndex();
    };
    
    ASSERT_TRUE(test_succeess(TIEX_STRING("This is a library."), TIEX_STRING("This")));
    ASSERT_TRUE(test_succeess(TIEX_STRING("hour小时"), TIEX_STRING("hour")));
    
    auto test_failure = [](const String& string) {
        Scanner scanner(string.c_str(), string.length());
        String word;
        bool is_succeeded = scanner.ReadWord(word);
        if (is_succeeded) {
            return false;
        }
        if (! word.empty()) {
            return false;
        }
        return scanner.GetCurrentIndex() == 0;
    };
    
    ASSERT_TRUE(test_failure(TIEX_STRING("+23.382")));
    ASSERT_TRUE(test_failure(TIEX_STRING(".Hello")));
    ASSERT_TRUE(test_failure(TIEX_STRING("")));
}


TEST(Scanner, ReadNumber) {
    
    auto test_success = [](const String& string, const String& expected) {
        Scanner scanner(string.c_str(), string.length());
        String number;
        bool is_succeeded = scanner.ReadNumber(number);
        if (! is_succeeded) {
            return false;
        }
        if (number != expected) {
            return false;
        }
        return scanner.GetCurrentIndex() == expected.length();
    };
    
    ASSERT_TRUE(test_success(TIEX_STRING("98723245243545354234"), TIEX_STRING("98723245243545354234")));
    ASSERT_TRUE(test_success(TIEX_STRING("987+132432"), TIEX_STRING("987")));
    ASSERT_TRUE(test_success(TIEX_STRING("1.68"), TIEX_STRING("1")));
    ASSERT_TRUE(test_success(TIEX_STRING("+784231u"), TIEX_STRING("+784231")));
    ASSERT_TRUE(test_success(TIEX_STRING("-784231 "), TIEX_STRING("-784231")));
    
    auto test_failure = [](const String& string, std::size_t expected_index) {
        Scanner scanner(string.c_str(), string.length());
        String number;
        bool is_succeeded = scanner.ReadNumber(number);
        if (is_succeeded) {
            return false;
        }
        if (! number.empty()) {
            return false;
        }
        return scanner.GetCurrentIndex() == expected_index;
    };
    
    ASSERT_TRUE(test_failure(TIEX_STRING("+ 2"), 1));
    ASSERT_TRUE(test_failure(TIEX_STRING("+ 2"), 1));
    ASSERT_TRUE(test_failure(TIEX_STRING("yes"), 0));
    ASSERT_TRUE(test_failure(TIEX_STRING("+"), 1));
    ASSERT_TRUE(test_failure(TIEX_STRING("++"), 1));
    ASSERT_TRUE(test_failure(TIEX_STRING("-"), 1));
    ASSERT_TRUE(test_failure(TIEX_STRING("--"), 1));
    ASSERT_TRUE(test_failure(TIEX_STRING("-+"), 1));
    ASSERT_TRUE(test_failure(TIEX_STRING(""), 0));
}
