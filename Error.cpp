#include "Error.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

using std::cerr;
using std::cout;
using std::endl;

int Error::get_error_code() const
{
    return error;
}

std::string Error::get_error_message() const
{
	switch (error)
	{
		case SUCCESS: return "";
		case MISSING_START_NOTE: return "The provided start note argument wasn't specified!";
		case INVALID_START_NOTE: return "The provided start note argument is not a valid musical note!";
		case MISSING_END_NOTE: return "The provided end note argument wasn't specified!";
		case INVALID_END_NOTE: return "The provided end note argument is not a valid musical note!";
		case MISSING_LENGTH: return "The provided length argument wasn't specified!";
		case INVALID_LENGTH: return "The provided length argument is not a valid number!";
		case LENGTH_TOO_LOW: return "The provided length argument cannot be less than 1!";
		case MISSING_NOTE_MODE: return "The provided note mode argument wasn't specified!";
		case INVALID_NOTE_MODE: return "The provided note mode argument is not a valid note mode!";
		case NOTE_MODE_NOT_QUADRATIC: return "The provided note mode is not quadratic!";
		case MISSING_FADE_START: return "The provided fade start argument wasn't specified!";
		case INVALID_FADE_START: return "The provided fade start argument is not a valid number!";
		case FADE_START_OUT_OF_RANGE: return "The provided fade start argument is out of range!";
		case MISSING_FADE_END: return "The provided fade end argument wasn't specified!";
		case INVALID_FADE_END: return "The provided fade end argument is not a valid number!";
		case FADE_END_OUT_OF_RANGE: return "The provided fade end argument is out of range!";
		case MISSING_FADE_MODE: return "The provided fade mode argument wasn't specified!";
		case INVALID_FADE_MODE: return "The provided fade mode argument is not a valid fade mode!";
		case FADE_MODE_NOT_QUADRATIC: return "The provided fade mode is not quadratic!";
		case MISSING_INSTRUMENT: return "The provided instrument argument wasn't specified!";
		case INVALID_INSTRUMENT: return "The provided instrument argument is not a valid Minecraft note block instrument!";
		case MISSING_PANNING_MODE: return "The provided panning mode argument wasn't specified!";
		case INVALID_PANNING_MODE: return "The provided panning mode argument is not a valid panning mode!";
		case MISSING_NBS_EXPORT: return "The provided arguments require NBS export to be enabled!!";
		case MISSING_FADE_RANGE: return "The provided fade range argument wasn't specified!";
		case INVALID_NOTES: return "The provided notes are not valid musical notes!";
		case SAME_NOTES: return "The provided start and end notes are the same!";
		default: return "Unknown error!";
	}
}

Error Error::get_error_by_name(std::string name)
{
    std::ranges::transform(name, name.begin(), toupper);
	if (name == "SUCCESS") return SUCCESS;
	else if (name == "MISSING_START_NOTE") return MISSING_START_NOTE;
	else if (name == "INVALID_START_NOTE") return INVALID_START_NOTE;
	else if (name == "MISSING_END_NOTE") return MISSING_END_NOTE;
	else if (name == "INVALID_END_NOTE") return INVALID_END_NOTE;
	else if (name == "MISSING_LENGTH") return MISSING_LENGTH;
	else if (name == "INVALID_LENGTH") return INVALID_LENGTH;
	else if (name == "LENGTH_TOO_LOW") return LENGTH_TOO_LOW;
	else if (name == "MISSING_NOTE_MODE") return MISSING_NOTE_MODE;
	else if (name == "INVALID_NOTE_MODE") return INVALID_NOTE_MODE;
	else if (name == "NOTE_MODE_NOT_QUADRATIC") return NOTE_MODE_NOT_QUADRATIC;
	else if (name == "MISSING_FADE_START") return MISSING_FADE_START;
	else if (name == "INVALID_FADE_START") return INVALID_FADE_START;
	else if (name == "FADE_START_OUT_OF_RANGE") return FADE_START_OUT_OF_RANGE;
	else if (name == "MISSING_FADE_END") return MISSING_FADE_END;
	else if (name == "INVALID_FADE_END") return INVALID_FADE_END;
	else if (name == "FADE_END_OUT_OF_RANGE") return FADE_END_OUT_OF_RANGE;
	else if (name == "MISSING_FADE_MODE") return MISSING_FADE_MODE;
	else if (name == "INVALID_FADE_MODE") return INVALID_FADE_MODE;
	else if (name == "FADE_MODE_NOT_QUADRATIC") return FADE_MODE_NOT_QUADRATIC;
	else if (name == "MISSING_INSTRUMENT") return MISSING_INSTRUMENT;
	else if (name == "INVALID_INSTRUMENT") return INVALID_INSTRUMENT;
	else if (name == "MISSING_PANNING_MODE") return MISSING_PANNING_MODE;
	else if (name == "INVALID_PANNING_MODE") return INVALID_PANNING_MODE;
	else if (name == "MISSING_NBS_EXPORT") return MISSING_NBS_EXPORT;
	else if (name == "MISSING_FADE_RANGE") return MISSING_FADE_RANGE;
	else if (name == "INVALID_NOTES") return INVALID_NOTES;
	else if (name == "NOTES_SAME") return SAME_NOTES;
	else throw std::invalid_argument("The inputted Error type " + name + " does not correspond to a valid Error");
}

int Error::print_error(const Error error, const std::initializer_list<std::string_view> extra = {})
{
	if (error == SUCCESS) return 0;
	cerr << "Invalid arguments!" << endl;
	cerr << error.get_error_message() << endl;
	for (const auto& extra_error : extra) cerr << extra_error << endl;
	return error.get_error_code();
}

int Error::print_error(const ErrorType error_type)
{
	return print_error(Error(error_type));
}

bool Error::operator==(const Error other_error) const
{
	return error == other_error.error;
}

bool Error::operator==(const ErrorType other_error_type) const
{
	return error == other_error_type;
}
