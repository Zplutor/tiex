#pragma once

#include <cassert>
#include <ctime>
#include <string>
#include "tiex_error.h"
#include "tiex_expression.h"
#include "tiex_locale.h"

namespace tiex {
namespace internal {

template<typename C>
BasicExpression<C> Parse(const std::basic_string<C>& expression_string, ParseError& parse_error);

template<typename C>
std::basic_string<C> Format(
	const BasicExpression<C>& expression,
	std::time_t referenced_time, 
	std::time_t formatted_time,
	const BasicLocale<C>& locale,
	FormatError& format_error);

}

/**
 A formatter represents a format expression that has been parsed,
 and is used to format relative times to string by the meaning of
 the expression.

 Typically, to create a formmater, you should write a format expression as
 string and pass it to the static function Create. You should reuse one
 formatter whenever possible if the same expression is needed for multiple
 times, to avoid re-parsing and performance.
 */
template<typename C>
class BasicFormatter {
public:
	using Char = C;
	using String = std::basic_string<Char>;
	using Locale = BasicLocale<Char>;
	using Expression = BasicExpression<Char>;

public:
	/**
	 Create a formatter.

	 @param string
	   An expression string that to be parsed.

	 @return
	   An empty formatter if fail to parse the expression.
	 */
	static BasicFormatter Create(const String& expression) {
		ParseError error;
		auto formatter = Create(expression, error);
		assert(error.status == ParseError::Status::None);
		return formatter;
	}

	/**
	 Create a formatter and catch parse error.

	 @param string
	   An expression string that to be parsed.

	 @param parse_error
	   An output parameter that stores information about parse error.

	 @return
	   An empty formatter if fail to parse the expression.
	 */
	static BasicFormatter Create(const String& expression, ParseError& parse_error) {
		return BasicFormatter(internal::Parse(expression, parse_error));
	}

public:
	/**
	 Construct an empty formatter.
	 */
	BasicFormatter() = default;

	/**
	 Construct a formatter with an expression.
	 */
	explicit BasicFormatter(Expression expression) : expression_(std::move(expression)) {

	}

	/**
	 Format times with locale information and catch format error.

	 @param referenced_time
	   The referenced time that is used to compare to the formatted time.

	 @param formatted_time
	   The target time to be formatted to string.

	 @param locale
	   Contains localization information that affect format result.

	 @param format_error
	   An output parameter that stores information about format error.

	 @return
	   A format result string. An empty string is returned if fail to format.
	 */
	String Format(
		std::time_t referenced_time,
		std::time_t formatted_time,
		const Locale& locale,
		FormatError& format_error) {

		return internal::Format(expression_, referenced_time, formatted_time, locale, format_error);
	}

	/**
	 Format times and catch format error.

	 @param referenced_time
	   The referenced time that is used to compare to the formatted time.

	 @param formatted_time
	   The target time to be formatted to string.

	 @param format_error
	   An output parameter that stores information about format error.

	 @return
	   A format result string. An empty string is returned if fail to format.
	 */
	String Format(std::time_t referenced_time, std::time_t formatted_time, FormatError& format_error) {
		return Format(referenced_time, formatted_time, Locale(), format_error);
	}

	/**
	 Format times with locale information.

	 @param referenced_time
	   The referenced time that is used to compare to the formatted time.

	 @param formatted_time
	   The target time to be formatted to string.

	 @param locale
	   Contains localization information that affect format result.

	 @return
	   A format result string. An empty string is returned if fail to format.
	 */
	String Format(std::time_t referenced_time, std::time_t formatted_time, const Locale& locale) {
		FormatError error;
		auto result = Format(referenced_time, formatted_time, locale, error);
		assert(error.status == FormatError::Status::None);
		return result;
	}

	/**
	 Format times.

	 @param referenced_time
	   The referenced time that is used to compare to the formatted time.

	 @param formatted_time
	   The target time to be formatted to string.

	 @return
	   A format result string. An empty string is returned if fail to format.
	 */
	String Format(std::time_t referenced_time, std::time_t formatted_time) {
		return Format(referenced_time, formatted_time, Locale());
	}

private:
	Expression expression_;
};

using Formatter = BasicFormatter<char>;
using WideFormatter = BasicFormatter<wchar_t>;

}

