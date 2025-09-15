#pragma once

#include <string>
#include <algorithm>

class NoteType
{
public:
	enum Notes
	{
		_ = -1, // used for invalid checking
		C,
		C_SHARP,
		D,
		D_SHARP,
		E,
		F,
		F_SHARP,
		G,
		G_SHARP,
		A,
		A_SHARP,
		B,
		MAX = B
	};
	explicit NoteType(std::string note);
	explicit NoteType(int key);
	NoteType(const Notes notes) : note(notes) {}
	static int getMaxOrdinal();
	[[nodiscard]] int getOrdinal() const;
	[[nodiscard]] std::string getName() const;
	[[nodiscard]] Notes getNote() const;
	void decreaseNote();
	void increaseNote();
	std::strong_ordering operator<=>(NoteType note_type) const;
	bool operator==(NoteType note_type) const;
	bool operator==(Notes note_type) const;
	[[nodiscard]] int compare(NoteType note_type) const;
	static bool validate_note(const std::string& note);

private:
	Notes note;
};
