#include "Note.h"

#include <algorithm>
#include <iterator>
#include <cmath>
#include <utility>

Note Note::invalid;

Note::Note() : note_type(NoteType::_), octave(std::numeric_limits<int>::max()) {}

Note::Note(string note) : Note(std::move(note), Instrument::PLING) {}

Note::Note(string note, const Instrument instrument) : instrument(instrument)
{
	std::ranges::transform(note, note.begin(), toupper);
	string base;
    if (note.find('#') != string::npos) base = note.substr(0, note.find_first_of('#') + 1);
    else base = std::string(1, note.at(0));
	note_type = NoteType(base);
	size_t convertedChars;
	string octave_string;
	for (size_t index = base.size(); index < note.size(); index++)
	{
		if (char octave_char = note[index]; octave_char == '+' || octave_char == '-' || (octave_char >= '0' && octave_char <= '9')) octave_string += octave_char;
		else break;
	}
	octave = std::stoi(octave_string, &convertedChars);
	if (convertedChars != octave_string.size()) throw std::invalid_argument("Octave value is not a number");
	string cents_string = note.substr(base.size() + octave_string.size());
	if (cents_string.ends_with("c")) cents_string = cents_string.substr(0, cents_string.size() - 1);
	if (!cents_string.empty())
	{
		cents = std::stoi(cents_string, &convertedChars);
		if (convertedChars != cents_string.size()) throw std::invalid_argument("Cents value is not a number");
	}
	else offset_cents(0); // Shift the inputted note type to compensate for cents (i.e., for C4+125c -> C#4+25c)
}

Note::Note(const int key)
{
	//if (key < 0) throw new std::invalid_argument("Key value is less than 0");
	//if (key > 87) throw new std::invalid_argument("Key value is greater than 87");
	octave = static_cast<int>(floor(((key / 12.0) - (3.0 / 12)) + 1)); // because C is the start of the octave, we need to subtract 3 to get the octave number, then add 1 to get the octave number
	note_type = NoteType(key);
}

NoteType Note::get_note_type() const
{
	return note_type;
}

void Note::decrease_note()
{
	if (note_type.getOrdinal() == 0) octave--;
	note_type.decreaseNote();
}

void Note::increase_note()
{
	if (note_type.getOrdinal() == NoteType::getMaxOrdinal()) octave++;
	note_type.increaseNote();
}

Instrument Note::get_instrument() const
{
	return instrument;
}

void Note::set_instrument(const Instrument instrument)
{
	this->instrument = instrument;
}

int Note::get_octave() const
{
	return octave;
}

void Note::set_octave(const int octave)
{
	this->octave = octave;
}

int Note::get_cents() const
{
	return cents;
}

void Note::set_cents(const int cents)
{
	this->cents = cents;
}

void Note::offset_cents(const int cents)
{
	this->cents += cents;
	if (this->cents < 0)
	{
		while (this->cents <= -100)
		{
			decrease_note();
			this->cents += 100;
		}
		if (this->cents < -50)
		{
			decrease_note();
			this->cents = 100 + this->cents;
		}
	}
	else if (this->cents > 0)
	{
		while (this->cents >= 100)
		{
			increase_note();
			this->cents -= 100;
		}
		if (this->cents > 50)
		{
			increase_note();
			this->cents = -(100 - this->cents);
		}
	}
}

int Note::get_id() const
{
	return Note("A0").compare(*this);
	// if (id < 0) return 0;
	// else if (id > 87) return 87;
	// return id;
}

Note Note::clone() const
{
	auto clone = Note(note_type.getName() + std::to_string(octave));
	clone.set_cents(this->cents);
	clone.set_instrument(this->instrument);
	return clone;
}

string Note::to_string() const
{
	return note_type.getName() + std::to_string(octave) + (this->cents <= 0 ? "" : "+") + (this->cents == 0 ? "" : std::to_string(this->cents) + "c");
}

/*
bool Note::operator<(Note note)
{
	std::cout << "Comparing if " << this->toString() << " is less than " << note.toString() << std::endl;
	std::cout << (this->octave < note.octave || this->getBaseNote() < note.getBaseNote() || this->cents < note.cents) << std::endl;
	return this->octave < note.octave || this->getBaseNote() < note.getBaseNote() || this->cents < note.cents;
}

bool Note::operator>(Note note)
{
	std::cout << "Comparing if " << this->toString() << " is more than " << note.toString() << std::endl;
	if (this->octave != note.octave) return this->octave > note.octave;
	else if 
	std::cout << (this->octave > note.octave || this->getBaseNote() > note.getBaseNote() || this->cents > note.cents) << std::endl;
	return this->octave > note.octave || this->getBaseNote() > note.getBaseNote() || this->cents > note.cents;
}*/

std::strong_ordering Note::operator<=>(const Note note) const
{
	if (this->octave != note.octave) return this->octave <=> note.octave;
	else if (this->get_note_type() != note.get_note_type()) return this->get_note_type() <=> note.get_note_type();
	else return this->cents <=> note.cents;
}

bool Note::operator==(const Note note) const
{
	return this->equals(note);
}

int Note::compare(const Note note) const
{
    if (*this == note) return 0;
    const int this_semitones = (octave * 12) + note_type.getOrdinal();
    const int other_semitones = (note.octave * 12) + note.get_note_type().getOrdinal();
    return other_semitones - this_semitones;
}

bool Note::equals(const Note note) const
{
	return equals_ignore_pitch(note) && cents == note.cents;
}

bool Note::equals_ignore_pitch(const Note note) const
{
	return note_type == note.note_type && octave == note.octave && instrument == note.instrument;
}