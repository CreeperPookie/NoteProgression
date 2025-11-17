#include "../../NoteProgression.h"

RandomPatternSegment::RandomPatternSegment(std::string segment)
{
	if (segment.starts_with("!"))
	{
		segment.erase(0, 1);
		excluding = true;
	}
	if (const string note_type_string = segment.substr(0, segment.size() >= 2 && segment[1] == '#' ? 2 : 1); note_type_string.starts_with("_") || !NoteType::validate_note(note_type_string)) throw std::invalid_argument("Invalid pattern segment: " + segment);
	else note_type = NoteType(note_type_string);
	if (const std::size_t note_type_size = note_type.get_name().size(); segment.size() > note_type_size)
	{
		is_octave_set = true;
		size_t convertedChars;
		octave = std::stoi(segment.substr(note_type_size), &convertedChars);
		if (convertedChars != segment.size() - note_type_size) throw std::invalid_argument("Octave value is not a number");
	}
}

bool RandomPatternSegment::excludes() const
{
	return excluding;
}

bool RandomPatternSegment::has_octave() const
{
	return is_octave_set;
}

NoteType RandomPatternSegment::get_note_type() const
{
	return note_type;
}

int RandomPatternSegment::get_octave() const
{
	return octave;
}

bool RandomPatternSegment::matches(const Note& note) const
{
	return excluding ? (note_type != note.get_note_type() || (has_octave() && octave != note.get_octave())) : note_type == note.get_note_type() && (!has_octave() || octave == note.get_octave());
}
