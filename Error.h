#pragma once

#include <string>

class Error
{
public:
	enum ErrorType
	{
		SUCCESS, // Should not be used, not an "error"
		MISSING_START_NOTE,
		INVALID_START_NOTE,
		MISSING_END_NOTE,
		INVALID_END_NOTE,
		MISSING_LENGTH,
		INVALID_LENGTH,
		LENGTH_TOO_LOW,
		MISSING_NOTE_MODE,
		INVALID_NOTE_MODE,
		NOTE_MODE_NOT_QUADRATIC,
		MISSING_FADE_START,
		INVALID_FADE_START,
		FADE_START_OUT_OF_RANGE,
		MISSING_FADE_END,
		INVALID_FADE_END,
		FADE_END_OUT_OF_RANGE,
		MISSING_FADE_MODE,
		INVALID_FADE_MODE,
		FADE_MODE_NOT_QUADRATIC,
		MISSING_INSTRUMENT,
		INVALID_INSTRUMENT,
		MISSING_PANNING_MODE,
		INVALID_PANNING_MODE,
		MISSING_NBS_EXPORT,
		MISSING_FADE_RANGE,
		INVALID_NOTES,
		SAME_NOTES,
		MISSING_RANDOM,
		MISSING_RANDOM_PATTERN,
		INVALID_RANDOM_PATTERN
	};
	Error() = default;
	constexpr Error(const ErrorType error_type) : error(error_type) {}
	constexpr operator ErrorType() const { return error; }
	operator bool() const = delete;
	int get_error_code() const;
	[[nodiscard]] std::string get_error_message() const;
	static Error get_error_by_name(std::string name);
	static int print_error(Error error, std::initializer_list<std::string_view> extra);
	static int print_error(ErrorType error_type);
	bool operator==(Error other_error) const;
	bool operator==(ErrorType other_error_type) const;
private:
	ErrorType error;
};

