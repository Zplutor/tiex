#include <gtest/gtest.h>
#include "tiex_scanner.h"

using namespace tiex;
using namespace tiex::internal;

TEST(Scanner, SkipWhiteSpaces) {
    
    auto test = [](const std::string& string, std::size_t expected_index) {
        Scanner<char> scanner(string.c_str(), string.length());
        scanner.SkipWhiteSpaces();
        return scanner.GetCurrentIndex() == expected_index;
    };
    
    ASSERT_TRUE(test(" \r\n\ta ", 4));
    ASSERT_TRUE(test("23 ", 0));
}


TEST(Scanner, SkipWhiteSpaces_WideChar) {
	std::wstring string(L"  2");
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	scanner.SkipWhiteSpaces();
	ASSERT_EQ(scanner.GetCurrentIndex(), 2);
}


TEST(Scanner, ReadChar) {
    
    {
        std::string string(" 123abc\n");
        Scanner<char> scanner(string.c_str(), string.length());
        
        for (std::size_t index = 0; index != string.length(); ++index) {
            
            char ch = 0;
            bool is_succeeded = scanner.ReadChar(ch);
            ASSERT_TRUE(is_succeeded);
            ASSERT_EQ(ch, string[index]);
            ASSERT_EQ(index + 1, scanner.GetCurrentIndex());
        }
        
        char ch = 0;
        bool is_succeeded = scanner.ReadChar(ch);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(string.length(), scanner.GetCurrentIndex());
    }
    
    {
        std::string string;
        Scanner<char> scanner(string.c_str(), string.length());
        char ch = 10;
        bool is_succeeded = scanner.ReadChar(ch);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(ch, 10);
    }
}


TEST(Scanner, ReadChar_WideChar) {
	std::wstring string(L"C");
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	wchar_t ch = 0;
	bool is_succeeded = scanner.ReadChar(ch);
	ASSERT_TRUE(is_succeeded);
	ASSERT_EQ(ch, L'C');
}


TEST(Scanner, ReadWord) {
    
    auto test_succeess = [](const std::string& string, const std::string& expected) {
        Scanner<char> scanner(string.c_str(), string.length());
        std::string word;
        bool is_succeeded = scanner.ReadWord(word);
        if (! is_succeeded) {
            return false;
        }
        if (word != expected) {
            return false;
        }
        return expected.length() == scanner.GetCurrentIndex();
    };
    
    ASSERT_TRUE(test_succeess("This is a library.", "This"));
    ASSERT_TRUE(test_succeess("hour小时", "hour"));
    
    auto test_failure = [](const std::string& string) {
        Scanner<char> scanner(string.c_str(), string.length());
        std::string word;
        bool is_succeeded = scanner.ReadWord(word);
        if (is_succeeded) {
            return false;
        }
        if (! word.empty()) {
            return false;
        }
        return scanner.GetCurrentIndex() == 0;
    };
    
    ASSERT_TRUE(test_failure("+23.382"));
    ASSERT_TRUE(test_failure(".Hello"));
    ASSERT_TRUE(test_failure(""));
}


TEST(Scanner, ReadWord_WideChar) {
	std::wstring string(L"word");
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	std::wstring word;
	bool is_succeeded = scanner.ReadWord(word);
	ASSERT_TRUE(is_succeeded);
	ASSERT_EQ(word, L"word");
}


TEST(Scanner, ReadNumber) {
    
    auto test_success = [](const std::string& string, const std::string& expected) {
        Scanner<char> scanner(string.c_str(), string.length());
        std::string number;
        bool is_succeeded = scanner.ReadNumber(number);
        if (! is_succeeded) {
            return false;
        }
        if (number != expected) {
            return false;
        }
        return scanner.GetCurrentIndex() == expected.length();
    };
    
    ASSERT_TRUE(test_success("98723245243545354234", "98723245243545354234"));
    ASSERT_TRUE(test_success("987+132432", "987"));
    ASSERT_TRUE(test_success("1.68", "1"));
    ASSERT_TRUE(test_success("+784231u", "+784231"));
    ASSERT_TRUE(test_success("-784231 ", "-784231"));
    
    auto test_failure = [](const std::string& string, std::size_t expected_index) {
        Scanner<char> scanner(string.c_str(), string.length());
        std::string number;
        bool is_succeeded = scanner.ReadNumber(number);
        if (is_succeeded) {
            return false;
        }
        if (! number.empty()) {
            return false;
        }
        return scanner.GetCurrentIndex() == expected_index;
    };
    
    ASSERT_TRUE(test_failure("+ 2", 1));
    ASSERT_TRUE(test_failure("+ 2", 1));
    ASSERT_TRUE(test_failure("yes", 0));
    ASSERT_TRUE(test_failure("+", 1));
    ASSERT_TRUE(test_failure("++", 1));
    ASSERT_TRUE(test_failure("-", 1));
    ASSERT_TRUE(test_failure("--", 1));
    ASSERT_TRUE(test_failure("-+", 1));
    ASSERT_TRUE(test_failure("", 0));
}


TEST(Scanner, ReadNumber_WideChar) {
	std::wstring string(L"87");
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	std::wstring number;
	bool is_succeeded = scanner.ReadNumber(number);
	ASSERT_TRUE(is_succeeded);
	ASSERT_EQ(number, L"87");
}