#include <gtest/gtest.h>
#include "tiex_parser.h"

using namespace tiex;
using namespace tiex::internal;

TEST(Parser, ParseChar) {
    
    auto string = TIEX_STRING("71");
    Scanner scanner(string.c_str(), string.length());
    Parser parser(scanner);
    
    bool is_succeeded = parser.ParseChar('7');
    ASSERT_TRUE(is_succeeded);
    
    is_succeeded = parser.ParseChar('8');
    ASSERT_FALSE(is_succeeded);
    ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedToken);
    ASSERT_EQ(parser.GetParseError().index, 1);
    ASSERT_EQ(parser.GetParseError().token, TIEX_STRING("1"));
    
    is_succeeded = parser.ParseChar('1');
    ASSERT_TRUE(is_succeeded);
    
    is_succeeded = parser.ParseChar('1');
    ASSERT_FALSE(is_succeeded);
    ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedEnd);
    ASSERT_EQ(parser.GetParseError().index, 2);
    ASSERT_EQ(parser.GetParseError().token, String());
}


TEST(Parser, ParseWord) {
    
    auto string = TIEX_STRING("Macbook8");
    Scanner scanner(string.c_str(), string.length());
    Parser parser(scanner);
    
    String word;
    bool is_succeeded = parser.ParseWord(word);
    ASSERT_TRUE(is_succeeded);
    ASSERT_EQ(word, TIEX_STRING("Macbook"));
    
    word.clear();
    is_succeeded = parser.ParseWord(word);
    ASSERT_FALSE(is_succeeded);
    ASSERT_TRUE(word.empty());
    ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedToken);
    ASSERT_EQ(parser.GetParseError().index, 7);
    ASSERT_EQ(parser.GetParseError().token, TIEX_STRING("8"));
    
    parser.ParseChar('8');
    
    word.clear();
    is_succeeded = parser.ParseWord(word);
    ASSERT_FALSE(is_succeeded);
    ASSERT_TRUE(word.empty());
    ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedEnd);
    ASSERT_EQ(parser.GetParseError().index, 8);
    ASSERT_EQ(parser.GetParseError().token, String());
}


TEST(Parser, ParseNumber) {
    
    {
        auto string = TIEX_STRING("4500=");
        Scanner scanner(string.c_str(), string.length());
        Parser parser(scanner);
        
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
        ASSERT_EQ(parser.GetParseError().token, TIEX_STRING("="));
        
        parser.ParseChar('=');
        
        is_succeeded = parser.ParseNumber(value);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(value, 0);
        ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedEnd);
        ASSERT_EQ(parser.GetParseError().index, 5);
        ASSERT_EQ(parser.GetParseError().token, TIEX_STRING(""));
    }
    
    {
        auto string = TIEX_STRING(" 12345678901234567890");
        Scanner scanner(string.c_str(), string.length());
        Parser parser(scanner);
        
        parser.ParseChar(' ');
        
        int value = 0;
        bool is_succeeded = parser.ParseNumber(value);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(value, 0);
        ASSERT_EQ(parser.GetParseError().status, ParseError::Status::ConversionFailed);
        ASSERT_EQ(parser.GetParseError().index, 1);
        ASSERT_EQ(parser.GetParseError().token, TIEX_STRING("12345678901234567890"));
    }
}


TEST(Parser, ParseUnit) {
    
    struct {
        String token;
        Unit unit;
    } const unit_items[] = {
        { TIEX_STRING("s"), Unit::Second },
        { TIEX_STRING("min"), Unit::Minute },
        { TIEX_STRING("h"), Unit::Hour },
        { TIEX_STRING("d"), Unit::Day },
        { TIEX_STRING("w"), Unit::Week },
        { TIEX_STRING("mth"), Unit::Month },
        { TIEX_STRING("y"), Unit::Year },
    };
    
    for (const auto& each_item : unit_items) {
        
        Scanner scanner(each_item.token.c_str(), each_item.token.length());
        Parser parser(scanner);
        
        Unit unit;
        bool is_succeeded = parser.ParseUnit(unit);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(unit, each_item.unit);
    }
    
    auto string = TIEX_STRING("abc");
    Scanner scanner(string.c_str(), string.length());
    Parser parser(scanner);
    
    Unit unit = Unit::Second;
    bool is_succeeded = parser.ParseUnit(unit);
    ASSERT_FALSE(is_succeeded);
    ASSERT_EQ(unit, Unit::Second);
    ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedToken);
    ASSERT_EQ(parser.GetParseError().index, 0);
    ASSERT_EQ(parser.GetParseError().token, TIEX_STRING("abc"));
}


TEST(Parser, ParseRound) {
    
    struct {
        String token;
        bool is_round;
    } const round_items[] = {
        { TIEX_STRING("."), true },
        { TIEX_STRING("~"), false },
    };
    
    for (const auto& each_item : round_items) {
        
        Scanner scanner(each_item.token.c_str(), each_item.token.length());
        Parser parser(scanner);
        
        bool is_round;
        bool is_succeeded = parser.ParseRound(is_round);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(is_round, each_item.is_round);
    }
    
    auto string = TIEX_STRING("^");
    Scanner scanner(string.c_str(), string.length());
    Parser parser(scanner);
    
    bool is_round = false;
    bool is_succeeded = parser.ParseRound(is_round);
    ASSERT_FALSE(is_succeeded);
    ASSERT_EQ(is_round, false);
    ASSERT_EQ(parser.GetParseError().status, ParseError::Status::UnexpectedToken);
    ASSERT_EQ(parser.GetParseError().index, 0);
    ASSERT_EQ(parser.GetParseError().token, TIEX_STRING("^"));
}


TEST(Parser, ParseBoundary_Zero) {

    auto string = TIEX_STRING("0");
    Scanner scanner(string.c_str(), string.length());
    Parser parser(scanner);

    Boundary boundary;
    bool is_succeeded = parser.ParseBoundary(false, boundary);
    ASSERT_TRUE(is_succeeded);
    ASSERT_EQ(boundary.value, 0);
    ASSERT_EQ(boundary.round, false);
    ASSERT_EQ(boundary.unit, Unit::Second);
}


TEST(Parser, ParseBoundary_Wildcard) {
    
    auto string = TIEX_STRING("*");
    auto test = [&string](bool is_forward, int expected_value) {
        
        Scanner scanner(string.c_str(), string.length());
        Parser parser(scanner);
        
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
    
    String strings[] = {
        TIEX_STRING("-1.d"),
        TIEX_STRING("-1 . d"),
    };
    
    for (const auto& each_string : strings) {
        
        Scanner scanner(each_string.c_str(), each_string.length());
        Parser parser(scanner);
        
        Boundary boundary;
        bool is_succeeded = parser.ParseBoundary(false, boundary);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(boundary.value, -1);
        ASSERT_EQ(boundary.round, true);
        ASSERT_EQ(boundary.unit, Unit::Day);
    }
}


TEST(Parser, ParseBoundary_Failure) {
    
    String strings[] = {
        TIEX_STRING("-1s.d"),
        TIEX_STRING("-1.sd"),
    };
    
    for (const auto& each_string : strings) {
        
        Scanner scanner(each_string.c_str(), each_string.length());
        Parser parser(scanner);
        
        Boundary boundary;
        bool is_succeeded = parser.ParseBoundary(false, boundary);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(boundary.value, 0);
        ASSERT_EQ(boundary.round, false);
        ASSERT_EQ(boundary.unit, Unit::Second);
    }
}


TEST(Parser, ParseCondition_Normal) {
    
    String strings[] = {
        TIEX_STRING("[-8~h,+6.min]"),
        TIEX_STRING("[ -8~h , +6.min ]"),
    };
    
    for (const auto& each_string : strings) {
        
        Scanner scanner(each_string.c_str(), each_string.length());
        Parser parser(scanner);
        
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
    
    String strings[] = {
        TIEX_STRING("[]"),
        TIEX_STRING("[,]"),
        TIEX_STRING("[1.s,]"),
        TIEX_STRING("[,1.s]"),
        TIEX_STRING("[-1.s,1.s"),
        TIEX_STRING("-1.s,1.s]"),
        TIEX_STRING("-1.s,1.s"),
        TIEX_STRING("(-1.s,1.s)"),
    };

    for (const auto& each_string : strings) {
        
        Scanner scanner(each_string.c_str(), each_string.length());
        Parser parser(scanner);
        
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


TEST(Parser, ParseResult_NoSpecifier) {
    
    struct {
        String string;
        bool has_standard_specifier;
    } result_items[] = {
        
        { TIEX_STRING("{ }"), false },
        { TIEX_STRING("{today}"), false },
        { TIEX_STRING("{ [today] }"), false },
        { TIEX_STRING("{ [  today  ] }"), false },
        { TIEX_STRING("{%h o'clock}"), true },
        { TIEX_STRING("{Now %s seconds}"), true },
        { TIEX_STRING("{Week %OW }"), true },
    };
    
    for (const auto& each_item : result_items) {
        
        Scanner scanner(each_item.string.c_str(), each_item.string.length());
        Parser parser(scanner);
        
        Result result;
        bool is_succeeded = parser.ParseResult(result);
        ASSERT_TRUE(is_succeeded);
        ASSERT_TRUE(result.specifiers.empty());
        ASSERT_EQ(result.has_standard_specifiers, each_item.has_standard_specifier);
        ASSERT_EQ(result.texts.size(), 1);
        ASSERT_EQ(result.texts[0], each_item.string.substr(1, each_item.string.length() - 2));
    }
    
    //Empty result
    auto string = TIEX_STRING("{}");
    Scanner scanner(string.c_str(), string.length());
    Parser parser(scanner);
    
    Result result;
    bool is_succeeded = parser.ParseResult(result);
    ASSERT_TRUE(is_succeeded);
    ASSERT_TRUE(result.specifiers.empty());
    ASSERT_TRUE(result.texts.empty());
    ASSERT_FALSE(result.has_standard_specifiers);
}


TEST(Parser, ParseResult_HasSpecifier) {
    
    auto test = [](
        const String& string,
        const std::vector<String>& expected_texts,
        const std::map<std::size_t, Unit>& expected_specifer_units) {
        
        Scanner scanner(string.c_str(), string.length());
        Parser parser(scanner);
        
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
    
    ASSERT_TRUE(test(TIEX_STRING("{%~min}"),
        { TIEX_STRING("") },
        { { 0, Unit::Minute } }));
    
    ASSERT_TRUE(test(TIEX_STRING("{Occurs %~d ago}"),
        { TIEX_STRING("Occurs "), TIEX_STRING(""), TIEX_STRING(" ago") },
        { { 1, Unit::Day } }));
    
    ASSERT_TRUE(test(TIEX_STRING("{%~d days, %~mth months, %~y years}"),
        {
            TIEX_STRING(""),
            TIEX_STRING(" days, "),
            TIEX_STRING(""),
            TIEX_STRING(" months, "),
            TIEX_STRING(""),
            TIEX_STRING(" years"),
        },
        {
            { 0, Unit::Day },
            { 2, Unit::Month },
            { 4, Unit::Year },
        }));
}


TEST(Parser, ParseResult_Failure) {
    
    auto test = [](const String& string, ParseError::Status status, std::size_t index, const String& token) {
        Scanner scanner(string.c_str(), string.length());
        Parser parser(scanner);
        Result result;
        bool is_succeeded = parser.ParseResult(result);
        if (is_succeeded) {
            return false;
        }
        const auto& error = parser.GetParseError();
        return error.status == status && error.index == index && error.token == token;
    };
    
    ASSERT_TRUE(test(TIEX_STRING(""), ParseError::Status::UnexpectedEnd, 0, String()));
    ASSERT_TRUE(test(TIEX_STRING("{1234"), ParseError::Status::UnexpectedEnd, 5, String()));
    ASSERT_TRUE(test(TIEX_STRING("1234}"), ParseError::Status::UnexpectedToken, 0, TIEX_STRING("1")));
}


TEST(Parser, ParseRule_Normal) {
    
    String strings[] = {
        TIEX_STRING("[-2.d,-1.d]{yesterday}"),
        TIEX_STRING("[-2.d,-1.d]   {yesterday}"),
    };
    
    for (const auto& each_string : strings) {
        Scanner scanner(each_string.c_str(), each_string.length());
        Parser parser(scanner);
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
        ASSERT_EQ(rule.result.texts[0], TIEX_STRING("yesterday"));
        ASSERT_EQ(rule.result.specifiers.size(), 0);
        ASSERT_EQ(rule.result.has_standard_specifiers, false);
    }
}


TEST(Parser, ParseRule_Failure) {

    String strings[] = {
        TIEX_STRING("[-2.d,-1.d]"),
        TIEX_STRING("{yesterday}"),
        TIEX_STRING("[-2.d,-1.d]&{yesterday}"),
    };
    
    for (const auto& each_string : strings) {
        Scanner scanner(each_string.c_str(), each_string.length());
        Parser parser(scanner);
        Rule rule;
        bool is_succeeded = parser.ParseRule(rule);
        ASSERT_FALSE(is_succeeded);
    }
}


TEST(Parser, ParseExpression_Single) {
    
    String strings[] = {
        TIEX_STRING("[*,*]{all}"),
        TIEX_STRING("  [*,*]{all}  "),
    };
    
    for (const auto& each_string : strings) {
        
        Scanner scanner(each_string.c_str(), each_string.length());
        Parser parser(scanner);
        
        Expression expression;
        bool is_succeeded = parser.ParseExpression(expression);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(expression.rules.size(), 1);
    }
}


TEST(Parser, ParseExpression_Multiple) {
    
    String strings[] = {
        TIEX_STRING("[*,*]{all}[-1.min,1.min]{just now}"),
        TIEX_STRING(" [*,*]{all}  [-1.min,1.min]{just now}  "),
        TIEX_STRING("  [*,*]{all}     [-1.min,1.min]{just now}   "),
    };
    
    for (const auto& each_string : strings) {
        
        Scanner scanner(each_string.c_str(), each_string.length());
        Parser parser(scanner);
        
        Expression expression;
        bool is_succeeded = parser.ParseExpression(expression);
        ASSERT_TRUE(is_succeeded);
        ASSERT_EQ(expression.rules.size(), 2);
    }
}


TEST(Parser, ParseExpression_Failure) {
    
    String strings[] = {
        TIEX_STRING(""),
        TIEX_STRING("[*,*][*,*]{all}"),
        TIEX_STRING("[*,*]{all}{all}"),
        TIEX_STRING("[*,*]{all}0[*,*]{all}"),
    };
    
    for (const auto& each_string : strings) {
        
        Scanner scanner(each_string.c_str(), each_string.length());
        Parser parser(scanner);
        
        Expression expression;
        bool is_succeeded = parser.ParseExpression(expression);
        ASSERT_FALSE(is_succeeded);
        ASSERT_EQ(expression.rules.size(), 0);
    }
}
