#include "NoteType.h"
#include <stdexcept>

NoteType::NoteType(std::string note)
{
	std::ranges::transform(note, note.begin(), toupper);
	if (note == "_") this->note = _;
	else if (note == "C") this->note = C;
	else if (note == "C#") this->note = C_SHARP;
	else if (note == "D") this->note = D;
	else if (note == "D#") this->note = D_SHARP;
	else if (note == "E") this->note = E;
	else if (note == "F") this->note = F;
	else if (note == "F#") this->note = F_SHARP;
	else if (note == "G") this->note = G;
	else if (note == "G#") this->note = G_SHARP;
	else if (note == "A") this->note = A;
	else if (note == "A#") this->note = A_SHARP;
	else if (note == "B") this->note = B;
	else throw std::invalid_argument("Provided note string is not a valid musical note");
}

NoteType::NoteType(const int key)
{
	note = static_cast<Notes>((key + 9) % 12);
}

int NoteType::getMaxOrdinal()
{
	return MAX;
}

int NoteType::getOrdinal() const
{
	return note;
}

std::string NoteType::getName() const
{
	switch (note)
	{
		case C: return "C";
		case C_SHARP: return "C#";
		case D: return "D";
		case D_SHARP: return "D#";
		case E: return "E";
		case F: return "F";
		case F_SHARP: return "F#";
		case G: return "G";
		case G_SHARP: return "G#";
		case A: return "A";
		case A_SHARP: return "A#";
		case B: return "B";
		default: return ""; // should never occur
	}
}

NoteType::Notes NoteType::getNote() const
{
	return note;
}

void NoteType::decreaseNote()
{
	int noteOrdinal = note;
	if (noteOrdinal == 0) noteOrdinal = MAX;
	else noteOrdinal--;
	note = static_cast<Notes>(noteOrdinal);
}

void NoteType::increaseNote()
{
	int noteOrdinal = note;
	if (noteOrdinal == MAX) noteOrdinal = 0;
	else noteOrdinal++;
	note = static_cast<Notes>(noteOrdinal);
}

std::strong_ordering NoteType::operator<=>(const NoteType note_type) const
{
	return this->getOrdinal() <=> note_type.getOrdinal();
}

bool NoteType::operator==(const NoteType note_type) const
{
	return note_type.getNote() == note;
}

bool NoteType::operator==(const Notes note_type) const
{
	return note_type == note;
}

int NoteType::compare(const NoteType note_type) const
{
	int difference = 0;
	if (*this == note_type) return difference;
	else if (*this < note_type) // rising
	{
		NoteType temp = *this;
		while (true)
		{
			temp.increaseNote();
			difference++;
			if (temp == note_type || temp == _) break;
		}
	}
	else
	{
		NoteType temp = note_type;
		while (true)
		{
			temp.decreaseNote();
			difference++;
			if (temp == note_type || temp == MAX) break;
		}
	}
	return difference;
}
