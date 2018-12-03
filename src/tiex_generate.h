#pragma once

#include <ctime>
#include <iomanip>
#include <sstream>
#include "tiex_expression.h"
#include "tiex_locale.h"
#include "tiex_time.h"

namespace tiex {
namespace internal {

long GetDifferenceWithTimet(const Specifier& specifier, std::time_t referenced_time, std::time_t formatted_time);
long GetDifferenceWithTm(const Specifier& specifier, const std::tm& referenced_tm, const std::tm& formatted_tm);
bool GetTimeDifference(const Specifier& specifier, const Time& reference_time, const Time& formatted_time, long& difference);
    
template<typename C>
std::basic_string<C> ToString(int value);

template<>
inline std::string ToString<char>(int value) {
	return std::to_string(value);
}

template<>
inline std::wstring ToString<wchar_t>(int value) {
	return std::to_wstring(value);
};


template<typename C>
bool GetLocaleText(
	C specifier_char,
	const std::tm& formatted_tm,
	const BasicLocale<C>& locale,
	std::basic_string<C>& locale_text) {

	//Mintue
	if (specifier_char == 'M') {
		if (locale.get_minute != nullptr) {

			locale_text = locale.get_minute(formatted_tm.tm_min);
			return true;
		}
	}
	//Second
	else if (specifier_char == 'S') {
		if (locale.get_second != nullptr) {

			locale_text = locale.get_second(formatted_tm.tm_sec);
			return true;
		}
	}
	//Hour
	else if ((specifier_char == 'H') ||
		(specifier_char == 'I')) {
		if (locale.get_hour != nullptr) {

			BasicLocale<C>::HourOptions options;
			int hour = formatted_tm.tm_hour;

			if (specifier_char == 'I') {
				options.is_12_hour_clock = true;

				if (hour == 0) {
					hour = 12;
				}
				else if (hour > 12) {
					hour -= 12;
				}
			}

			locale_text = locale.get_hour(hour, options);
			return true;
		}
	}
	//AM/PM
	else if (specifier_char == 'p') {
		if (locale.get_am_pm != nullptr) {

			bool is_pm = formatted_tm.tm_hour >= 12;
			locale_text = locale.get_am_pm(is_pm);
			return true;
		}
	}
	//Weekday
	else if ((specifier_char == 'a') ||
		(specifier_char == 'A')) {
		if (locale.get_weekday != nullptr) {

			BasicLocale<C>::WeekdayOptions options;
			options.is_abbreviated = (specifier_char == 'a');
			locale_text = locale.get_weekday(formatted_tm.tm_wday, options);
			return true;
		}
	}
	//Month
	else if ((specifier_char == 'b') ||
		(specifier_char == 'h') ||
		(specifier_char == 'B') ||
		(specifier_char == 'm')) {
		if (locale.get_month != nullptr) {

			BasicLocale<C>::MonthOptions options;
			if (specifier_char == 'm') {
				options.is_number = true;
			}
			else if (specifier_char != 'B') {
				options.is_abbreviated = true;
			}

			locale_text = locale.get_month(formatted_tm.tm_mon + 1, options);
			return true;
		}
	}

	return false;
}


template<typename C>
bool OverrideStandardSpecifiers(const std::tm& formatted_tm, const BasicLocale<C>& locale, std::basic_string<C>& result_text) {

	if ((locale.get_month == nullptr) &&
		(locale.get_weekday == nullptr) &&
		(locale.get_am_pm == nullptr) &&
		(locale.get_hour == nullptr) &&
		(locale.get_minute == nullptr) &&
		(locale.get_second == nullptr)) {
		return false;
	}

	bool has_overridden_all = true;

	std::size_t index = 0;
	while (index < result_text.length() - 1) {

		if (result_text[index] != '%') {
			++index;
			continue;
		}

		C next_char = result_text[index + 1];
		std::basic_string<C> override_text;
		bool has_got = GetLocaleText(next_char, formatted_tm, locale, override_text);
		if (! has_got) {

			if (next_char != '%') {
				has_overridden_all = false;
			}
			index += 2;
			continue;
		}

		result_text.replace(index, 2, override_text);
		index += override_text.length();
	}

	return has_overridden_all;
}
    

template<typename C>
bool GenerateResultText(
	const BasicResult<C>& result,
	const Time& reference_time,
	const Time& formatted_time,
	const BasicLocale<C>& locale,
	std::basic_string<C>& text) {

	std::basic_string<C> result_text;

	for (std::size_t index = 0; index < result.texts.size(); ++index) {

		const auto& each_text = result.texts[index];
		if (each_text.empty()) {

			auto iterator = result.specifiers.find(index);
			if (iterator != result.specifiers.end()) {

				long difference = 0;
				bool is_succeeded = GetTimeDifference(iterator->second, reference_time, formatted_time, difference);
				if (!is_succeeded) {
					return false;
				}

				result_text.append(ToString<C>(std::abs(difference)));
			}
		}
		else {
			result_text.append(each_text);
		}
	}

	if (result.has_standard_specifiers) {

		auto formatted_tm = formatted_time.GetTm();
		if (formatted_tm == nullptr) {
			return false;
		}

		bool has_overridden_all = OverrideStandardSpecifiers(*formatted_tm, locale, result_text);
		if (! has_overridden_all) {

			std::basic_ostringstream<C> stream;
			stream << std::put_time(formatted_tm, result_text.c_str());
			result_text = stream.str();
		}
	}

	text = result_text;
	return true;
}
    
}
}
