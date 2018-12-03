#include "tiex_formatter.h"
#include "tiex_generate.h"
#include "tiex_match.h"
#include "tiex_parser.h"
#include "tiex_scanner.h"

namespace tiex {
namespace internal {

template<typename C>
BasicExpression<C> Parse(const std::basic_string<C>& expression_string, ParseError& parse_error) {

	Scanner<C> scanner(expression_string.c_str(), expression_string.length());
	Parser<C> parser(scanner);

	auto expression = parser.Parse();
	parse_error = parser.GetParseError();

	return expression;
}

template
BasicExpression<char> Parse<char>(const std::basic_string<char>& expression, ParseError& parse_error);

template
BasicExpression<wchar_t> Parse<wchar_t>(const std::basic_string<wchar_t>& expression, ParseError& parse_error);


template<typename C>
std::basic_string<C> Format(
	const BasicExpression<C>& expression,
	std::time_t referenced_time,
	std::time_t formatted_time,
	const BasicLocale<C>& locale,
	FormatError& format_error) {

	if (expression.rules.empty()) {
		format_error.status = FormatError::Status::NoMatchedRule;
		return {};
	}

	internal::Time referenced(referenced_time);
	internal::Time formatted(formatted_time);

	for (const auto& each_rule : expression.rules) {

		bool is_matched = false;
		bool is_succeeded = internal::MatchCondition(each_rule.condition, referenced, formatted, is_matched);
		if (! is_succeeded) {
			format_error.status = FormatError::Status::TimeError;
			return {};
		}

		if (is_matched) {

			std::basic_string<C> result_text;
			bool is_succeeded = GenerateResultText(each_rule.result, referenced, formatted, locale, result_text);
			if (! is_succeeded) {
				format_error.status = FormatError::Status::TimeError;
				return {};
			}

			return result_text;
		}
	}

	format_error.status = FormatError::Status::NoMatchedRule;
	return {};
}

template
std::basic_string<char> Format(
	const BasicExpression<char>& expression,
	std::time_t referenced_time,
	std::time_t formatted_time,
	const BasicLocale<char>& locale,
	FormatError& format_error);

template
std::basic_string<wchar_t> Format(
	const BasicExpression<wchar_t>& expression,
	std::time_t referenced_time,
	std::time_t formatted_time,
	const BasicLocale<wchar_t>& locale,
	FormatError& format_error);

}
}
