#pragma once

#include "tiex_error.h"
#include "tiex_expression.h"
#include "tiex_scanner.h"

namespace tiex {
namespace internal {

template<typename C>
class Parser {
public:
	using Char = C;
	using String = std::basic_string<Char>;

public:
	Parser(Scanner<Char>& scanner) : scanner_(scanner) {

	}

	Parser(const Parser&) = delete;
	Parser& operator=(const Parser&) = delete;
    

    BasicExpression<Char> Parse() {

		parse_error_.Clear();

		BasicExpression<Char> expression;
		ParseExpression(expression);
		return expression;
	}
    

    const ParseError& GetParseError() const {
        return parse_error_;
    }
    

//These private mehtods are declared as public just for testing purpose.
public:
	bool ParseExpression(BasicExpression<Char>& expression) {

		std::vector<BasicRule<Char>> rules;

		scanner_.SkipWhiteSpaces();

		bool is_succeeded = false;
		do {

			BasicRule<Char> rule;
			is_succeeded = ParseRule(rule);
			if (! is_succeeded) {
				break;
			}

			rules.push_back(rule);

			scanner_.SkipWhiteSpaces();
		} 
		while (! scanner_.IsEnd());

		if (is_succeeded) {
			expression.rules = std::move(rules);
		}

		return is_succeeded;
	}


	bool ParseRule(BasicRule<Char>& rule) {

		Condition condition;
		if (! ParseCondition(condition)) {
			return false;
		}

		scanner_.SkipWhiteSpaces();

		BasicResult<Char> result;
		if (! ParseResult(result)) {
			return false;
		}

		rule.condition = std::move(condition);
		rule.result = std::move(result);
		return true;
	}


	bool ParseCondition(Condition& condition) {

		if (! ParseChar('[')) {
			return false;
		}

		scanner_.SkipWhiteSpaces();

		Boundary backward;
		if (! ParseBoundary(false, backward)) {
			return false;
		}

		scanner_.SkipWhiteSpaces();

		if (! ParseChar(',')) {
			return false;
		}

		scanner_.SkipWhiteSpaces();

		Boundary forward;
		if (! ParseBoundary(true, forward)) {
			return false;
		}

		scanner_.SkipWhiteSpaces();

		if (! ParseChar(']')) {
			return false;
		}

		condition.backward = std::move(backward);
		condition.forward = std::move(forward);
		return true;
	}


	bool ParseBoundary(bool is_forward, Boundary& boundary) {

		Char ch = 0;
		if (! scanner_.GetChar(ch)) {
			SetError(ParseError::Status::UnexpectedEnd);
			return false;
		}

		if (ch == '0') {

			boundary.value = 0;
			scanner_.ReadChar(ch);
			return true;
		}
		else if (ch == '*') {

			boundary.value =
				is_forward ?
				std::numeric_limits<int>::max() :
				std::numeric_limits<int>::min();

			scanner_.ReadChar(ch);
			return true;
		}

		int value = 0;
		if (! ParseNumber(value)) {
			return false;
		}

		scanner_.SkipWhiteSpaces();

		bool round = false;
		if (! ParseRound(round)) {
			return false;
		}

		scanner_.SkipWhiteSpaces();

		Unit unit = Unit::Second;
		if (! ParseUnit(unit)) {
			return false;
		}

		boundary.value = value;
		boundary.round = round;
		boundary.unit = unit;
		return true;
	}


	bool ParseRound(bool& round) {

		Char ch = 0;
		if (! scanner_.ReadChar(ch)) {
			SetError(ParseError::Status::UnexpectedEnd);
			return false;
		}

		if (ch == '.') {
			round = true;
			return true;
		}

		if (ch == '~') {
			round = false;
			return true;
		}

		SetError(ParseError::Status::UnexpectedToken, -1);
		return false;
	}


	bool ParseUnit(Unit& unit) {

		String word;
		if (! ParseWord(word)) {
			return false;
		}

		bool is_found = GetUnit(word, unit);
		if (! is_found) {
			SetError(ParseError::Status::UnexpectedToken, -static_cast<int>(word.length()));
		}

		return is_found;
	}


	bool ParseResult(BasicResult<Char>& result) {

		if (! ParseChar('{')) {
			return false;
		}

		bool is_succeeded = true;
		std::vector<String> texts;
		std::map<std::size_t, Specifier> specifiers;
		bool has_standard_specifiers = false;
		String current_text;

		while (true) {

			Char ch = 0;
			if (! scanner_.ReadChar(ch)) {
				SetError(ParseError::Status::UnexpectedEnd);
				is_succeeded = false;
				break;
			}

			if (ch == '}') {
				if (! current_text.empty()) {
					texts.push_back(current_text);
				}
				break;
			}

			if (ch == '%') {

				if (! scanner_.ReadChar(ch)) {
					SetError(ParseError::Status::UnexpectedEnd);
					is_succeeded = false;
					break;
				}

				if (ch == '~') {

					Unit unit = Unit::Second;
					if (! ParseUnit(unit)) {
						is_succeeded = false;
						break;
					}

					if (! current_text.empty()) {
						texts.push_back(current_text);
						current_text.clear();
					}

					auto specifier_index = texts.size();
					texts.push_back({});

					Specifier specifier;
					specifier.uint = unit;
					specifiers[specifier_index] = specifier;
				}
				else {
					has_standard_specifiers = true;
					current_text.push_back('%');
					current_text.push_back(ch);
				}
			}
			else {
				current_text.push_back(ch);
			}
		}

		if (is_succeeded) {
			result.texts = std::move(texts);
			result.specifiers = std::move(specifiers);
			result.has_standard_specifiers = has_standard_specifiers;
		}

		return is_succeeded;
	}


	bool ParseChar(Char expected_char) {

		Char read_char = 0;
		if (! scanner_.GetChar(read_char)) {
			SetError(ParseError::Status::UnexpectedEnd);
			return false;
		}

		if (read_char != expected_char) {
			SetError(ParseError::Status::UnexpectedToken);
			return false;
		}

		scanner_.ReadChar(read_char);
		return true;
	}


	bool ParseNumber(int& value) {

		String number;
		if (! scanner_.ReadNumber(number)) {

			if (scanner_.IsEnd()) {
				SetError(ParseError::Status::UnexpectedEnd);
			}
			else {
				SetError(ParseError::Status::UnexpectedToken);
			}
			return false;
		}

		try {
			value = std::stoi(number);
			return true;
		}
		catch (...) {
			SetError(ParseError::Status::ConversionFailed, -static_cast<int>(number.length()));
			return false;
		}
	}


	bool ParseWord(String& word) {

		if (! scanner_.ReadWord(word)) {

			if (scanner_.IsEnd()) {
				SetError(ParseError::Status::UnexpectedEnd);
			}
			else {
				SetError(ParseError::Status::UnexpectedToken);
			}
			return false;
		}

		return true;
	}
    
private:
	static bool GetUnit(const String& string, Unit& unit) {

		if (string.length() == 1) {

			switch (string[0]) {
			case 's':
				unit = Unit::Second;
				return true;
			case 'h':
				unit = Unit::Hour;
				return true;
			case 'd':
				unit = Unit::Day;
				return true;
			case 'w':
				unit = Unit::Week;
				return true;
			case 'y':
				unit = Unit::Year;
				return true;
			}
		}
		else if (string.length() == 3) {

			if (string[0] == 'm') {
				if ((string[1] == 'i') && (string[2] == 'n')) {
					unit = Unit::Minute;
					return true;
				}
				if ((string[1] == 't') && (string[2] == 'h')) {
					unit = Unit::Month;
					return true;
				}
			}
		}

		return false;
	}

private:
	void SetError(ParseError::Status status, int index_adjustment) {
		parse_error_.status = status;
		parse_error_.index = scanner_.GetCurrentIndex() + index_adjustment;
	}
    
    void SetError(ParseError::Status status) {
        SetError(status, 0);
    }
    
private:
	Scanner<Char>& scanner_;
    ParseError parse_error_;
};

}
}
