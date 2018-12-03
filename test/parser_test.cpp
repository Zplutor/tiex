#include <gtest/gtest.h>
#include "tiex_parser.h"

using namespace tiex;
using namespace tiex::internal;

TEST(Parser, ParseChar) {
    
    std::string string = "71";
    Scanner<char> scanner(string.c_str(), string.length());
    Parser<char> parser(scanner);
    
    bool is_succeeded = parser.ParseChar('7');
    ASSERT_TRUE(is_succeeded);
    
    is_succeeded = parser.ParseChar('8');
    ASSERT_FALSE(is_succeeded);
    ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedToken);
    ASSERT_EQ(parser.GetParseError().index, 1);
    
    is_succeeded = parser.ParseChar('1');
    ASSERT_TRUE(is_succeeded);
    
    is_succeeded = parser.ParseChar('1');
    ASSERT_FALSE(is_succeeded);
    ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedEnd);
    ASSERT_EQ(parser.GetParseError().index, 2);
}


TEST(Parser, ParseChar_WideChar) {
	std::wstring string = L"7";
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	Parser<wchar_t> parser(scanner);
	bool is_succeeded = parser.ParseChar(L'7');
	ASSERT_TRUE(is_succeeded);
}


TEST(Parser, ParseWord) {
    
    std::string string = "Macbook8";
    Scanner<char> scanner(string.c_str(), string.length());
    Parser<char> parser(scanner);
    
    std::string word;
    bool is_succeeded = parser.ParseWord(word);
    ASSERT_TRUE(is_succeeded);
    ASSERT_EQ(word, "Macbook");
    
    word.clear();
    is_succeeded = parser.ParseWord(word);
    ASSERT_FALSE(is_succeeded);
    ASSERT_TRUE(word.empty());
    ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedToken);
    ASSERT_EQ(parser.GetParseError().index, 7);
    
    parser.ParseChar('8');
    
    word.clear();
    is_succeeded = parser.ParseWord(word);
    ASSERT_FALSE(is_succeeded);
    ASSERT_TRUE(word.empty());
    ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedEnd);
    ASSERT_EQ(parser.GetParseError().index, 8);
}


TEST(Parser, ParseWord_WideChar) {
	std::wstring string = L"word";
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	Parser<wchar_t> parser(scanner);
	std::wstring word;
	bool is_succeeded = parser.ParseWord(word);
	ASSERT_TRUE(is_succeeded);
	ASSERT_EQ(word, L"word");
}


TEST(Parser, ParseNumber) {
    
    {
        std::string string = "4500=";
        Scanner<char> scanner(string.c_str(), string.length());
        Parser<char> parser(scanner);
        
        int value = 0;
        bool is_succeeded = parser.ParseNumber(value);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(value, 4500);
        
        value = 0;
        is_succeeded = parser.ParseNumber(value);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(value, 0);
        ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedToken);
        ASSERT_EQ(parser.GetParseError().index, 4);
        
        parser.ParseChar('=');
        
        is_succeeded = parser.ParseNumber(value);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(value, 0);
        ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedEnd);
        ASSERT_EQ(parser.GetParseError().index, 5);
    }
    
    {
        std::string string = " 12345678901234567890";
        Scanner<char> scanner(string.c_str(), string.length());
        Parser<char> parser(scanner);
        
        parser.ParseChar(' ');
        
        int value = 0;
        bool is_succeeded = parser.ParseNumber(value);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(value, 0);
        ASSERT_EQ(parser.GetParseError().status, ParseError::Status::ConversionFailed);
        ASSERT_EQ(parser.GetParseError().index, 1);
    }
}


TEST(Parser, ParseNumber_WideChar) {
	std::wstring string = L"878";
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	Parser<wchar_t> parser(scanner);
	int number = 0;
	bool is_succeeded = parser.ParseNumber(number);
	ASSERT_TRUE(is_succeeded);
	ASSERT_EQ(number, 878);
}


TEST(Parser, ParseUnit) {
    
    struct {
        std::string token;
        Unit unit;
    } const unit_items[] = {
        { "s", Unit::Second },
        { "min", Unit::Minute },
        { "h", Unit::Hour },
        { "d", Unit::Day },
        { "w", Unit::Week },
        { "mth", Unit::Month },
        { "y", Unit::Year },
    };
    
    for (const auto& each_item : unit_items) {
        
        Scanner<char> scanner(each_item.token.c_str(), each_item.token.length());
        Parser<char> parser(scanner);
        
        Unit unit;
        bool is_succeeded = parser.ParseUnit(unit);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(unit, each_item.unit);
    }
    
    std::string string = "abc";
    Scanner<char> scanner(string.c_str(), string.length());
    Parser<char> parser(scanner);
    
    Unit unit = Unit::Second;
    bool is_succeeded = parser.ParseUnit(unit);
    ASSERT_FALSE(is_succeeded);
    ASSERT_EQ(unit, Unit::Second);
    ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedToken);
    ASSERT_EQ(parser.GetParseError().index, 0);
}


TEST(Parser, ParseUnit_WideChar) {
	std::wstring string = L"min";
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	Parser<wchar_t> parser(scanner);
	Unit unit = Unit::Second;
	bool is_succeeded = parser.ParseUnit(unit);
	ASSERT_TRUE(is_succeeded);
	ASSERT_EQ(unit, Unit::Minute);
}


TEST(Parser, ParseRound) {
    
    struct {
        std::string token;
        bool is_round;
    } const round_items[] = {
        { ".", true },
        { "~", false },
    };
    
    for (const auto& each_item : round_items) {
        
        Scanner<char> scanner(each_item.token.c_str(), each_item.token.length());
        Parser<char> parser(scanner);
        
        bool is_round;
        bool is_succeeded = parser.ParseRound(is_round);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(is_round, each_item.is_round);
    }
    
    std::string string = "^";
    Scanner<char> scanner(string.c_str(), string.length());
    Parser<char> parser(scanner);
    
    bool is_round = false;
    bool is_succeeded = parser.ParseRound(is_round);
    ASSERT_FALSE(is_succeeded);
    ASSERT_EQ(is_round, false);
    ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedToken);
    ASSERT_EQ(parser.GetParseError().index, 0);
}


TEST(Parser, ParseRound_WideChar) {
	std::wstring string = L".";
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	Parser<wchar_t> parser(scanner);
	bool is_round = false;
	bool is_succeeded = parser.ParseRound(is_round);
	ASSERT_TRUE(is_succeeded);
	ASSERT_TRUE(is_round);
}


TEST(Parser, ParseBoundary_Zero) {

    std::string string = "0";
    Scanner<char> scanner(string.c_str(), string.length());
    Parser<char> parser(scanner);

    Boundary boundary;
    bool is_succeeded = parser.ParseBoundary(false, boundary);
    ASSERT_TRUE(is_succeeded);
    ASSERT_EQ(boundary.value, 0);
    ASSERT_EQ(boundary.round, false);
    ASSERT_EQ(boundary.unit, Unit::Second);
}


TEST(Parser, ParseBoundary_Wildcard) {
    
    std::string string = "*";
    auto test = [&string](bool is_forward, int expected_value) {
        
        Scanner<char> scanner(string.c_str(), string.length());
        Parser<char> parser(scanner);
        
        Boundary boundary;
        bool is_succeeded = parser.ParseBoundary(is_forward, boundary);
        if (! is_succeeded) {
            return false;
        }
        return boundary.value == expected_value && boundary.round == false && boundary.unit == Unit::Second;
    };
    
    ASSERT_TRUE(test(false, std::numeric_limits<int>::min()));
    ASSERT_TRUE(test(true, std::numeric_limits<int>::max()));
}


TEST(Parser, ParseBoundary_Normal) {
    
    std::string strings[] = {
        "-1.d",
        "-1 . d",
    };
    
    for (const auto& each_string : strings) {
        
        Scanner<char> scanner(each_string.c_str(), each_string.length());
        Parser<char> parser(scanner);
        
        Boundary boundary;
        bool is_succeeded = parser.ParseBoundary(false, boundary);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(boundary.value, -1);
        ASSERT_EQ(boundary.round, true);
        ASSERT_EQ(boundary.unit, Unit::Day);
    }
}


TEST(Parser, ParseBoundary_Failure) {
    
    std::string strings[] = {
        "-1s.d",
        "-1.sd",
    };
    
    for (const auto& each_string : strings) {
        
        Scanner<char> scanner(each_string.c_str(), each_string.length());
        Parser<char> parser(scanner);
        
        Boundary boundary;
        bool is_succeeded = parser.ParseBoundary(false, boundary);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(boundary.value, 0);
        ASSERT_EQ(boundary.round, false);
        ASSERT_EQ(boundary.unit, Unit::Second);
    }
}


TEST(Parser, ParseBoundary_WideChar) {
	std::wstring string(L"5.mth");
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	Parser<wchar_t> parser(scanner);
	Boundary boundary;
	bool is_succeeded = parser.ParseBoundary(true, boundary);
	ASSERT_TRUE(is_succeeded);
	ASSERT_EQ(boundary.value, 5);
	ASSERT_EQ(boundary.round, true);
	ASSERT_EQ(boundary.unit, Unit::Month);
}


TEST(Parser, ParseCondition_Normal) {
    
    std::string strings[] = {
        "[-8~h,+6.min]",
        "[ -8~h , +6.min ]",
    };
    
    for (const auto& each_string : strings) {
        
        Scanner<char> scanner(each_string.c_str(), each_string.length());
        Parser<char> parser(scanner);
        
        Condition condition;
        bool is_succeeded = parser.ParseCondition(condition);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(condition.backward.value, -8);
        ASSERT_EQ(condition.backward.round, false);
        ASSERT_EQ(condition.backward.unit, Unit::Hour);
        ASSERT_EQ(condition.forward.value, 6);
        ASSERT_EQ(condition.forward.round, true);
        ASSERT_EQ(condition.forward.unit, Unit::Minute);
    }
}


TEST(Parser, ParseCondition_Failure) {
    
    std::string strings[] = {
        "[]",
        "[,]",
        "[1.s,]",
        "[,1.s]",
        "[-1.s,1.s",
        "-1.s,1.s]",
        "-1.s,1.s",
        "(-1.s,1.s)",
    };

    for (const auto& each_string : strings) {
        
        Scanner<char> scanner(each_string.c_str(), each_string.length());
        Parser<char> parser(scanner);
        
        Condition condition;
        bool is_succeeded = parser.ParseCondition(condition);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(condition.backward.value, 0);
        ASSERT_EQ(condition.backward.round, false);
        ASSERT_EQ(condition.backward.unit, Unit::Second);
        ASSERT_EQ(condition.forward.value, 0);
        ASSERT_EQ(condition.forward.round, false);
        ASSERT_EQ(condition.forward.unit, Unit::Second);
    }
}


TEST(Parser, ParseCondition_WideChar) {
	std::wstring string(L"[-1~h,+2.d]");
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	Parser<wchar_t> parser(scanner);
	Condition condition;
	bool is_succeeded = parser.ParseCondition(condition);
	ASSERT_TRUE(is_succeeded);
	ASSERT_EQ(condition.backward.value, -1);
	ASSERT_EQ(condition.backward.round, false);
	ASSERT_EQ(condition.backward.unit, Unit::Hour);
	ASSERT_EQ(condition.forward.value, 2);
	ASSERT_EQ(condition.forward.round, true);
	ASSERT_EQ(condition.forward.unit, Unit::Day);
}


TEST(Parser, ParseResult_NoSpecifier) {
    
    struct {
        std::string string;
        bool has_standard_specifier;
    } result_items[] = {
        
        { "{ }", false },
        { "{today}", false },
        { "{ [today] }", false },
        { "{ [  today  ] }", false },
        { "{%h o'clock}", true },
        { "{Now %s seconds}", true },
        { "{Week %OW }", true },
    };
    
    for (const auto& each_item : result_items) {
        
        Scanner<char> scanner(each_item.string.c_str(), each_item.string.length());
        Parser<char> parser(scanner);
        
        Result result;
        bool is_succeeded = parser.ParseResult(result);
        ASSERT_TRUE(is_succeeded);
        ASSERT_TRUE(result.specifiers.empty());
        ASSERT_EQ(result.has_standard_specifiers, each_item.has_standard_specifier);
        ASSERT_EQ(result.texts.size(), 1);
        ASSERT_EQ(result.texts[0], each_item.string.substr(1, each_item.string.length() - 2));
    }
    
    //Empty result
    std::string string = "{}";
    Scanner<char> scanner(string.c_str(), string.length());
    Parser<char> parser(scanner);
    
    Result result;
    bool is_succeeded = parser.ParseResult(result);
    ASSERT_TRUE(is_succeeded);
    ASSERT_TRUE(result.specifiers.empty());
    ASSERT_TRUE(result.texts.empty());
    ASSERT_FALSE(result.has_standard_specifiers);
}


TEST(Parser, ParseResult_HasSpecifier) {
    
    auto test = [](
        const std::string& string,
        const std::vector<std::string>& expected_texts,
        const std::map<std::size_t, Unit>& expected_specifer_units) {
        
        Scanner<char> scanner(string.c_str(), string.length());
        Parser<char> parser(scanner);
        
        Result result;
        bool is_succeeded = parser.ParseResult(result);
        if (! is_succeeded) {
            return false;
        }
        
        if (result.has_standard_specifiers) {
            return false;
        }
        
        if (result.texts.size() != expected_texts.size()) {
            return false;
        }
        for (std::size_t index = 0; index < result.texts.size(); ++index) {
            if (result.texts[index] != expected_texts[index]) {
                return false;
            }
        }
        
        if (result.specifiers.size() != expected_specifer_units.size()) {
            return false;
        }
        for (const auto& each_specifier : result.specifiers) {
            auto iterator = expected_specifer_units.find(each_specifier.first);
            if (iterator == expected_specifer_units.end()) {
                return false;
            }
            if (iterator->second != each_specifier.second.uint) {
                return false;
            }
        }
        
        return true;
    };
    
    ASSERT_TRUE(test("{%~min}",
        { "" },
        { { 0, Unit::Minute } }));
    
    ASSERT_TRUE(test("{Occurs %~d ago}",
        { "Occurs ", "", " ago" },
        { { 1, Unit::Day } }));
    
    ASSERT_TRUE(test("{%~d days, %~mth months, %~y years}",
        {
            "",
            " days, ",
            "",
            " months, ",
            "",
            " years",
        },
        {
            { 0, Unit::Day },
            { 2, Unit::Month },
            { 4, Unit::Year },
        }));
}


TEST(Parser, ParseResult_Failure) {
    
    auto test = [](const std::string& string, ParseError::Status status, std::size_t index, const std::string& token) {
        Scanner<char> scanner(string.c_str(), string.length());
        Parser<char> parser(scanner);
        Result result;
        bool is_succeeded = parser.ParseResult(result);
        if (is_succeeded) {
            return false;
        }
        const auto& error = parser.GetParseError();
        return error.status == status && error.index == index;
    };
    
    ASSERT_TRUE(test("", ParseError::Status::UnexpectedEnd, 0, std::string()));
    ASSERT_TRUE(test("{1234", ParseError::Status::UnexpectedEnd, 5, std::string()));
    ASSERT_TRUE(test("1234}", ParseError::Status::UnexpectedToken, 0, "1"));
}


TEST(Parser, ParseResult_WideChar) {
	std::wstring string(L"{hello}");
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	Parser<wchar_t> parser(scanner);
	WideResult result;
	bool is_succeeded = parser.ParseResult(result);
	ASSERT_TRUE(is_succeeded);
	ASSERT_EQ(result.has_standard_specifiers, false);
	ASSERT_TRUE(result.specifiers.empty());
	ASSERT_FALSE(result.texts.empty());
	ASSERT_EQ(result.texts[0], L"hello");
}


TEST(Parser, ParseRule_Normal) {
    
    std::string strings[] = {
        "[-2.d,-1.d]{yesterday}",
        "[-2.d,-1.d]   {yesterday}",
    };
    
    for (const auto& each_string : strings) {
        Scanner<char> scanner(each_string.c_str(), each_string.length());
        Parser<char> parser(scanner);
        Rule rule;
        bool is_succeeded = parser.ParseRule(rule);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(rule.condition.backward.value, -2);
        ASSERT_EQ(rule.condition.backward.round, true);
        ASSERT_EQ(rule.condition.backward.unit, Unit::Day);
        ASSERT_EQ(rule.condition.forward.value, -1);
        ASSERT_EQ(rule.condition.forward.round, true);
        ASSERT_EQ(rule.condition.forward.unit, Unit::Day);
        ASSERT_EQ(rule.result.texts.size(), 1);
        ASSERT_EQ(rule.result.texts[0], "yesterday");
        ASSERT_EQ(rule.result.specifiers.size(), 0);
        ASSERT_EQ(rule.result.has_standard_specifiers, false);
    }
}


TEST(Parser, ParseRule_Failure) {

    std::string strings[] = {
        "[-2.d,-1.d]",
        "{yesterday}",
        "[-2.d,-1.d]&{yesterday}",
    };
    
    for (const auto& each_string : strings) {
        Scanner<char> scanner(each_string.c_str(), each_string.length());
        Parser<char> parser(scanner);
        Rule rule;
        bool is_succeeded = parser.ParseRule(rule);
        ASSERT_FALSE(is_succeeded);
    }
}


TEST(Parser, ParseRule_WideChar) {
	std::wstring string(L"[-2.d,-1.d]{yesterday}");
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	Parser<wchar_t> parser(scanner);
	WideRule rule;
	bool is_succeeded = parser.ParseRule(rule);
	ASSERT_TRUE(is_succeeded);
	ASSERT_EQ(rule.condition.backward.value, -2);
	ASSERT_EQ(rule.condition.backward.round, true);
	ASSERT_EQ(rule.condition.backward.unit, Unit::Day);
	ASSERT_EQ(rule.condition.forward.value, -1);
	ASSERT_EQ(rule.condition.forward.round, true);
	ASSERT_EQ(rule.condition.forward.unit, Unit::Day);
	ASSERT_EQ(rule.result.texts.size(), 1);
	ASSERT_EQ(rule.result.texts[0], L"yesterday");
	ASSERT_EQ(rule.result.specifiers.size(), 0);
	ASSERT_EQ(rule.result.has_standard_specifiers, false);
}


TEST(Parser, ParseExpression_Single) {
    
    std::string strings[] = {
        "[*,*]{all}",
        "  [*,*]{all}  ",
    };
    
    for (const auto& each_string : strings) {
        
        Scanner<char> scanner(each_string.c_str(), each_string.length());
        Parser<char> parser(scanner);
        
        Expression expression;
        bool is_succeeded = parser.ParseExpression(expression);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(expression.rules.size(), 1);
    }
}


TEST(Parser, ParseExpression_Multiple) {
    
    std::string strings[] = {
        "[*,*]{all}[-1.min,1.min]{just now}",
        " [*,*]{all}  [-1.min,1.min]{just now}  ",
        "  [*,*]{all}     [-1.min,1.min]{just now}   ",
    };
    
    for (const auto& each_string : strings) {
        
        Scanner<char> scanner(each_string.c_str(), each_string.length());
        Parser<char> parser(scanner);
        
        Expression expression;
        bool is_succeeded = parser.ParseExpression(expression);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(expression.rules.size(), 2);
    }
}


TEST(Parser, ParseExpression_Failure) {
    
    std::string strings[] = {
        "",
        "[*,*][*,*]{all}",
        "[*,*]{all}{all}",
        "[*,*]{all}0[*,*]{all}",
    };
    
    for (const auto& each_string : strings) {
        
        Scanner<char> scanner(each_string.c_str(), each_string.length());
        Parser<char> parser(scanner);
        
        Expression expression;
        bool is_succeeded = parser.ParseExpression(expression);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(expression.rules.size(), 0);
    }
}


TEST(Parser, ParseExpression_WideChar) {
	std::wstring string(L"[*,*]{all}[-1.min,1.min]{just now}");
	Scanner<wchar_t> scanner(string.c_str(), string.length());
	Parser<wchar_t> parser(scanner);
	WideExpression expression;
	bool is_succeeded = parser.ParseExpression(expression);
	ASSERT_TRUE(is_succeeded);
	ASSERT_EQ(expression.rules.size(), 2);
}