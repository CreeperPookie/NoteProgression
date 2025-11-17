#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include "NoteType.h"
#include "Instrument.h"

using std::string;
using std::endl;

class Note
{
	NoteType note_type { "C" };
	Instrument instrument = Instrument::PLING;
	int octave;
	int cents{ 0 };
public:
	Note();
	explicit Note(string note);
	explicit Note(string note, Instrument instrument);
	explicit Note(int key);
	static Note invalid;
	[[nodiscard]] NoteType get_note_type() const;
	void decrease_note();
	void increase_note();
	[[nodiscard]] Instrument get_instrument() const;
	void set_instrument(Instrument instrument);
	[[nodiscard]] int get_octave() const;
	void set_octave(int octave);
	[[nodiscard]] int get_cents() const;
	void set_cents(int cents);
	void offset_cents(int cents);
	[[nodiscard]] int get_id() const;
	[[nodiscard]] Note clone() const;
	[[nodiscard]] string to_string() const;
	[[nodiscard]] double compare(Note note) const;
	[[nodiscard]] bool equals(Note note) const;
	[[nodiscard]] bool equals_ignore_pitch(Note note) const;

	//bool operator<(Note note);
	//bool operator>(Note note);
	std::strong_ordering operator<=>(Note note) const;
	bool operator==(Note note) const;
};
