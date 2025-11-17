#pragma once
#include "Note.h"
#include "NoteType.h"

class RandomPatternSegment
{
	bool excluding = false;
	bool is_octave_set = false;
	NoteType note_type = NoteType::_;
	int octave = 0;
public:
	RandomPatternSegment(std::string segment);
	bool excludes() const;
	bool has_octave() const;
	NoteType get_note_type() const;
	int get_octave() const;
	bool matches(const Note& note) const;
};
