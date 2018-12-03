#pragma once

#include <cctype>
#include <cwctype>
#include <string>

namespace tiex {
namespace internal {

/**
 Determinate whether the specified character is an ASCII alpha.

 std::iswalpha in MSVC always returns unexpected result with Chinese
 character no matter what locale is set, so a rewritten version here
 is used.
 */
inline bool IsAlpha(int ch) {
	return
		(('a' <= ch) && (ch <= 'z')) ||
		(('A' <= ch) && (ch <= 'Z'));
}


inline bool IsDigit(char ch) {
	return std::isdigit(ch);
}

inline bool IsDigit(wchar_t ch) {
	return std::iswdigit(ch);
}


inline bool IsSpace(char ch) {
	return std::isspace(ch);
}

inline bool IsSpace(wchar_t ch) {
	return std::iswspace(ch);
}


template<typename C>
class Scanner {
public:
	using Char = C;
	using String = std::basic_string<Char>;

public:
    Scanner(const Char* string, std::size_t length) :
		begin_(string),
		end_(string + length),
		cursor_(string) {

	}

	Scanner(const Scanner&) = delete;
	Scanner& operator=(const Scanner&) = delete;
    

	bool GetChar(Char& ch) const {

		if (cursor_ == end_) {
			return false;
		}

		ch = *cursor_;
		return true;
	}
    

	bool ReadChar(Char& ch) {

		if (cursor_ == end_) {
			return false;
		}

		ch = *cursor_;
		++cursor_;
		return true;
	}


	bool ReadWord(String& word) {

		String temp_word;

		while (cursor_ != end_) {

			auto current_char = *cursor_;
			if (IsAlpha(current_char)) {
				temp_word.push_back(current_char);
				++cursor_;
			}
			else {
				break;
			}
		}

		if (temp_word.empty()) {
			return false;
		}

		word = temp_word;
		return true;
	}

	bool ReadNumber(String& number) {

		String temp_number;
		bool has_sign = false;

		Char current_char = *cursor_;
		if (current_char == '-' || current_char == '+') {
			temp_number.push_back(current_char);
			has_sign = true;
			++cursor_;
		}

		while (cursor_ != end_) {
			current_char = *cursor_;
			if (IsDigit(current_char)) {
				temp_number.push_back(current_char);
				++cursor_;
			}
			else {
				break;
			}
		}

		bool is_succeeded = false;
		if (has_sign) {
			is_succeeded = temp_number.length() > 1;
		}
		else {
			is_succeeded = !temp_number.empty();
		}

		if (is_succeeded) {
			number = temp_number;
		}

		return is_succeeded;
	}


	void SkipWhiteSpaces() {

		while (cursor_ != end_) {
			if (IsSpace(*cursor_)) {
				++cursor_;
			}
			else {
				break;
			}
		}
	}
    

    std::size_t GetCurrentIndex() const {
        return cursor_ - begin_;
    }
    

    bool IsEnd() const {
        return cursor_ == end_;
    }
    
private:
    const Char* begin_;
    const Char* end_;
    const Char* cursor_;
};

}
}
