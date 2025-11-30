#include "NoteProgression.h"

#include <cstring>
#include <windows.h>

#include "Error.h"
#include "OutputData.h"
#include "note/random/RandomData.h"
#include "note/random/RandomPattern.h"
#include "Utility.h"
#include "util/BinaryFile.h"

#include "note/Note.h"
#include "note/Panning.h"
#include "note/GenerationMode.h"

int main(int argc, char* argv[])
{
	string start_note;
	string end_note;
	int length = -1;
	bool delay_set = false;
	long delay = 5000;
	bool debug = false;
	bool no_output_info = false;
	bool random_pitches = false;
	bool nbs_export = false;
	bool include_base_note = false;
	bool include_end_note = false;
	bool flip_note_quadratic_functions = false;
	bool flip_fade_quadratic_functions = false;
	int custom_instrument_index = -1;
	int random_segment_period = -1;
	int fade_start_percent = -1;
	int fade_end_percent = -1;
	RandomPattern random_note_pattern;
	Random random;
	Instrument instrument = Instrument::PLING;
	GenerationMode fade_mode = GenerationMode::LINEAR;
	GenerationMode note_mode = GenerationMode::LINEAR;
	Panning panning_mode = Panning::PAN_NONE;
	for (int index = 1; index < argc; index++)
	{
		string argument = argv[index];
		if (is_argument(argument, {"no-delay", "no-cooldown"}))
		{
			if (delay_set && delay > 0) return Error::print_error(Error::INVALID_DELAY, "The completion delay cannot be set if the delay is disabled with " + argument + "!");
			else delay = 0;
		}
		else if (is_argument(argument, {"delay", "custom-delay"}))
		{
			if (delay == 0) return Error::print_error(Error::INVALID_DELAY, "The completion delay cannot be set as it was already disabled!");
			else if (argc == index + 1) return Error::print_error(Error::MISSING_DELAY);
			const char* delay_argument = argv[++index];
			size_t converted_chars;
			delay = std::stol(string(delay_argument), &converted_chars);
			if (converted_chars != strlen(delay_argument)) return Error::print_error(Error::INVALID_DELAY);
			else if (delay < 0) return Error::print_error(Error::DELAY_OUT_OF_RANGE);
			else delay_set = true;
		}
		else if (is_argument(argument, "debug")) debug = true;
		else if (is_argument(argument, {"no-info", "no-log-info", "no-output-info"})) no_output_info = true;
		else if (is_argument(argument, "help"))
		{
			print_help();
			return 0;
		}
		if (is_argument(argument, {"start", "note", "start-note", "first-note"}))
		{
			if (argc == index + 1) return Error::print_error(Error::MISSING_START_NOTE);
			start_note = argv[++index];
			continue;
		}
		if (is_argument(argument, {"end", "end-note", "last-note"}))
		{
			if (argc == index + 1) return Error::print_error(Error::MISSING_END_NOTE);
			end_note = argv[++index];
			continue;
		}
		if (is_argument(argument, {"length", "count"}))
		{
			if (argc == index + 1) return Error::print_error(Error::MISSING_LENGTH);
			const char* lengthArgument = argv[++index];
			size_t converted_chars;
			try
			{
				length = std::stoi(string(lengthArgument), &converted_chars);
			}
			catch (std::invalid_argument &e)
			{
				return Error::print_error(Error::INVALID_LENGTH);
			}
			if (converted_chars != strlen(lengthArgument)) return Error::print_error(Error::INVALID_LENGTH);
			else if (length < 1) return Error::print_error(Error::LENGTH_TOO_LOW);
		}
		if (is_argument(argument, "random-pitch")) random_pitches = true;
		if (is_argument(argument, {"only-major", "random-only-major"})) random_note_pattern = "C,D,E,F,G,A,B";
		if (is_argument(argument, {"only-minor", "random-only-minor"})) random_note_pattern = "C#,D#,F#,G#,A#";
		if (is_argument(argument, {"random-pattern", "random-note-pattern"}))
		{
			if (argc == index + 1) return Error::print_error(Error::MISSING_RANDOM_PATTERN);
			try
			{
				random_note_pattern = RandomPattern(argv[++index]);
			}
			catch (std::invalid_argument &e)
			{
				return Error::print_error(Error::INVALID_RANDOM_PATTERN);
			}
		}
		if (is_argument(argument, {"random-period", "random-note-period", "random-segment-period", "random-note-segment-length", "random-note-segment-period"}))
		{
			if (argc == index + 1) return Error::print_error(Error::MISSING_RANDOM_PERIOD);
			const char* random_segment_period_argument = argv[++index];
			size_t converted_chars;
			try
			{
				random_segment_period = std::stoi(string(random_segment_period_argument), &converted_chars);
			}
			catch (std::invalid_argument &e)
			{
				return Error::print_error(Error::INVALID_RANDOM_PERIOD);
			}
			if (converted_chars != strlen(random_segment_period_argument)) return Error::print_error(Error::INVALID_RANDOM_PERIOD);
			else if (random_segment_period < 1) return Error::print_error(Error::RANDOM_PERIOD_TOO_LOW);
			else if (random_segment_period > length) return Error::print_error(Error::RANDOM_PERIOD_TOO_HIGH);
		}
		if (is_argument(argument, {"mode", "note-mode"}))
		{
			if (argc == index + 1) return Error::print_error(Error::MISSING_NOTE_MODE);
			string note_mode_argument = argv[++index];
			try
			{
				note_mode = GenerationMode::get_generation_mode(note_mode_argument);
			}
			catch (std::invalid_argument &e)
			{
				return Error::print_error(Error::INVALID_NOTE_MODE);
			}
		}
		if (is_argument(argument, "fade-start"))
		{
			if (argc == index + 1) return Error::print_error(Error::MISSING_FADE_START);
			const char* fade_start_argument = argv[++index];
			size_t converted_chars;
			fade_start_percent = std::stoi(string(fade_start_argument), &converted_chars);
			if (converted_chars != strlen(fade_start_argument)) return Error::print_error(Error::INVALID_FADE_START);
			if (fade_start_percent < 1 || fade_start_percent > 100) return Error::print_error(Error::FADE_START_OUT_OF_RANGE);
		}
		if (is_argument(argument, "fade-end"))
		{
			if (argc == index + 1) return Error::print_error(Error::MISSING_FADE_END);
			const char* fade_end_argument = argv[++index];
			size_t converted_chars;
			fade_end_percent = std::stoi(string(fade_end_argument), &converted_chars);
			if (converted_chars != strlen(fade_end_argument)) return Error::print_error(Error::INVALID_FADE_END);
			if (fade_end_percent < 1 || fade_end_percent > 100) return Error::print_error(Error::FADE_END_OUT_OF_RANGE);
		}
		if (is_argument(argument, "fade-mode"))
		{
			if (argc == index + 1) return Error::print_error(Error::MISSING_FADE_MODE);
			string fade_mode_argument = argv[++index];
			try
			{
				fade_mode = GenerationMode::get_generation_mode(fade_mode_argument);
			}
			catch (std::invalid_argument &e)
			{
				return Error::print_error(Error::INVALID_FADE_MODE);
			}
		}
		if (is_argument(argument, "instrument"))
		{
			if (argc == index + 1) return Error::print_error(Error::MISSING_INSTRUMENT);
			string instrument_argument = argv[++index];
			try
			{
				instrument = Instrument::get_instrument(instrument_argument);
				if (instrument == Instrument::CUSTOM && custom_instrument_index == -1) custom_instrument_index = 0;
			}
			catch (std::invalid_argument &e)
			{
				return Error::print_error(Error::INVALID_INSTRUMENT);
			}
		}
		if (is_argument(argument, "custom-instrument-index"))
		{
			if (argc == index + 1) return Error::print_error(Error::MISSING_CUSTOM_INSTRUMENT);
		    string instrument_index_string = argv[++index];
		    size_t converted_chars;
			custom_instrument_index = std::stoi(string(instrument_index_string), &converted_chars);
			if (converted_chars != strlen(instrument_index_string.c_str()) || custom_instrument_index < 0 || custom_instrument_index > 255) return Error::print_error(Error::INVALID_CUSTOM_INSTRUMENT);
		}
		if (is_argument(argument, "panning"))
		{
			if (argc == index + 1) return Error::print_error(Error::MISSING_PANNING_MODE);
			string panning_argument = argv[++index];
			try
			{
				panning_mode = Panning::get_panning_type_by_name(panning_argument);
			}
			catch (std::invalid_argument &e)
			{
				return Error::print_error(Error::INVALID_PANNING_MODE);
			}
		}
		if (is_argument(argument, { "flip-note-quadratic-functions", "reverse-note-quadratic-functions" }))
		{
			flip_note_quadratic_functions = true;
			if (note_mode != GenerationMode::QUADRATIC)
			{
				if (!no_output_info) cout << "quadratic mode will be enabled as it's required for flipping functions!" << endl;
				note_mode = GenerationMode::QUADRATIC;
			}
		}
		if (is_argument(argument, { "flip-fade-quadratic-functions", "reverse-fade-quadratic-functions" }))
		{
			flip_fade_quadratic_functions = true;
			if (fade_mode != GenerationMode::QUADRATIC)
			{
				if (!no_output_info) cout << "quadratic mode will be enabled as it's required for flipping functions!" << endl;
				fade_mode = GenerationMode::QUADRATIC;
			}
		}
		if (is_argument(argument, "nbs")) nbs_export = true;
		if (is_argument(argument, { "include-start-note", "include-base-note", "add-start-note" })) include_base_note = true;
		if (is_argument(argument, { "include-end-note", "include-end-note", "add-end-note" })) include_end_note = true;
	}
	if (note_mode.is_random() && start_note.empty()) start_note = "A0";
	if (note_mode.is_random() && end_note.empty()) end_note = "C8";
	else if (start_note.empty()) return Error::print_error(Error::MISSING_START_NOTE);
	else if (note_mode == GenerationMode::LINEAR && end_note.empty() || start_note == end_note)
	{
		note_mode = GenerationMode::STATIC;
		if (!no_output_info && debug) cout << "End note not specified or duplicated, assuming static note generation!" << endl;
	}
	else if (end_note.empty()) return Error::print_error(Error::MISSING_END_NOTE);
	if (length == -1) return Error::print_error(Error::MISSING_LENGTH);
	int offset = 2;
	if (include_base_note) offset--;
	if (include_end_note) offset--;
	if (length + (2 - offset) < 2) return Error::print_error(Error::MISSING_LENGTH);
	else if (instrument == Instrument::CUSTOM && custom_instrument_index == -1) return Error::print_error(Error::CUSTOM_INSTRUMENT_INDEX_REQUIRED); // with the 0 fallback, this really shouldn't happen
	else if (custom_instrument_index >= 240) return Error::print_error(Error::CUSTOM_INSTRUMENT_OUT_OF_RANGE, "For NBS v5, this is 240 custom instruments! (indexes start at 0)");
	else if ((fade_start_percent == -1) != (fade_end_percent == -1)) return Error::print_error(Error::MISSING_FADE_RANGE);// Check if only one fade percentage is specified
	else if (!nbs_export && ((fade_start_percent != -1 || fade_end_percent != -1) || panning_mode != Panning::PAN_NONE || custom_instrument_index > -1)) return Error::print_error(Error::MISSING_NBS_EXPORT);
	if (note_mode != GenerationMode::QUADRATIC && flip_note_quadratic_functions) return Error::print_error(Error::NOTE_MODE_NOT_QUADRATIC);
	if (fade_mode != GenerationMode::QUADRATIC && flip_fade_quadratic_functions) return Error::print_error(Error::FADE_MODE_NOT_QUADRATIC);
	if (!note_mode.is_random() && random_pitches) return Error::print_error(Error::MISSING_RANDOM);
	std::vector<Note> notes;
	Note start_note_instance;
	Note end_note_instance;
	try
	{
		start_note_instance = Note(start_note);
		end_note_instance = Note(end_note);
	}
	catch (std::invalid_argument &e)
	{
		return Error::print_error(Error::INVALID_NOTES);
	}
	start_note_instance.set_instrument(instrument);
	end_note_instance.set_instrument(instrument);
	notes = get_note_progression(start_note_instance, end_note_instance, length, note_mode, include_base_note, include_end_note, flip_note_quadratic_functions, RandomData(random, random_pitches, random_segment_period, random_note_pattern), OutputData(!no_output_info, debug));
	/*
	if (note_mode == GenerationMode::STATIC || note_mode == GenerationMode::RANDOM_NORMAL)
	{
		for (int i = 0; i < length - offset; i++)
		{
			Note note;
			if (note_mode == GenerationMode::STATIC) note = current_note;
			else note = generate_random_note(random, start_note_instance, end_note_instance, instrument, random_pitches, random_note_pattern);
			notes.push_back(note);
		}
	}
	else if (note_mode == GenerationMode::RANDOM_LINEAR || note_mode == GenerationMode::RANDOM_QUADRATIC)
	{
		bool static_random_period = random_segment_period == -1;
		if (static_random_period) random_segment_period = random.get_random_int(2, 10);
		while (notes.size() < length - offset)
		{
			Note segment_start = generate_random_note(random, start_note_instance, end_note_instance, instrument, random_pitches, random_note_pattern);
			Note segment_end = generate_random_note(random, start_note_instance, end_note_instance, instrument, random_pitches, random_note_pattern);
			uint64_t clamped_random_segment_period = std::min(static_cast<uint64_t>(random_segment_period), length - offset - notes.size());
			std::vector<Note> random_segments = get_note_progression(segment_start, segment_end, length - offset, note_mode, include_base_note, clamped_random_segment_period == random_segment_period ? true : include_end_note, flip_note_quadratic_functions);
			notes.insert(notes.end(), random_segments.begin(), random_segments.end());
		}
	}
	else if (include_base_note) notes.push_back(current_note);
	if (note_mode == GenerationMode::LINEAR && abs(start_semitones) == length - offset)
	{
		for (int i = 0; i < length - offset; i++)
		{
			if (start_semitones < 0) current_note.decrease_note();
			else current_note.increase_note();
			if (current_note != end_note_instance) notes.push_back(current_note.clone());
		}
	}
	else if (note_mode == GenerationMode::LINEAR || note_mode == GenerationMode::QUADRATIC)
	{
		int totalLength = length;
		if (include_end_note) totalLength--;
		double current_precision_loss = 0;
		double cent_step_exact;
		int cent_step;
		if (note_mode == GenerationMode::LINEAR)
		{
			cent_step_exact = ((start_semitones / static_cast<double>(totalLength)) * 100.0);
			if (!no_output_info && debug) cout << "exact cent step: " << cent_step_exact << endl;
			cent_step = static_cast<int>(cent_step_exact < 0 ? ceil(cent_step_exact) : floor(cent_step_exact));
		}
		int cent_offset = 0;
		for (int i = include_base_note ? 1 : 0; i < totalLength; i++)
		{
			if (note_mode == GenerationMode::QUADRATIC)
			{
				// https://www.desmos.com/calculator/9jv1asddng
				double target_semitone;
				double lengthSquareReciprocal = 1.0 / Utility::square(totalLength);
				int note_distance = end_note_instance.get_id() - start_note_instance.get_id();
				if (flip_note_quadratic_functions) target_semitone = (-note_distance * lengthSquareReciprocal * Utility::square(i - totalLength) + end_note_instance.get_id());
				else target_semitone = (note_distance * lengthSquareReciprocal * Utility::square(i) + start_note_instance.get_id());
				cent_step_exact = -round((((notes.empty() ? start_note_instance.get_id() : notes[notes.size() - 1].get_id()) * 100) + (notes.empty() ? start_note_instance : notes[notes.size() - 1]).get_cents() - (target_semitone * 100)));
				cent_step = static_cast<int>(cent_step_exact < 0 ? ceil(cent_step_exact) : floor(cent_step_exact));
			}
			current_precision_loss += -abs(cent_step_exact - cent_step);
			if (!no_output_info && debug) cout << "new precision loss: " << current_precision_loss << endl;
			if (current_precision_loss != 0 && abs(current_precision_loss / 0.495) >= 1)
			{
				cent_offset = static_cast<int>((current_precision_loss / abs(current_precision_loss)) * ((cent_step_exact / abs(cent_step_exact)) * round(current_precision_loss + ((current_precision_loss / abs(current_precision_loss)) * 0.051))));
				if (!no_output_info && debug) cout << "detected precision loss, adding " << cent_offset << " cents" << endl;
				current_precision_loss += round(abs(current_precision_loss) + 0.051);
			}
			current_note.offset_cents(cent_step + cent_offset);
			cent_offset = 0;
			notes.push_back(current_note.clone());
		}
		if (include_end_note) notes.push_back(end_note_instance);
	}
	*/
	if (!nbs_export) print_output(notes, no_output_info);
	else
	{
		std::filesystem::path user_home = string(getenv("USERPROFILE"));
		std::filesystem::path full_path = user_home;
		int count = 0;
		if (!std::filesystem::exists(user_home / "Note Progressions")) std::filesystem::create_directory(full_path / "Note Progressions");
		do
		{
			full_path = user_home / "Note Progressions" / ("note-progression-" + std::to_string(count + 1) + ".nbs");
			count++;
		}
		while (count < 10000 && exists(full_path));
		if (count == 10000)
		{
			cerr << "Error: Unable to generate NBS file!" << endl;
			cerr << "To export, please clear your Note Progressions folder of \"note-progression-{index}.nbs\" files." << endl;
			cerr << "Textual representation: " << endl << endl;
			print_output(notes, no_output_info);
			return 0;
		}
		BinaryFile file(full_path.string());
		short layer_count = 33;
		file.write_short(0); // Song length in < NBS v5, otherwise *ALWAYS* 0
		file.write_byte(5); // Version
		file.write_byte(Instrument::MAX_VANILLA + 1); // Vanilla instrument count
		file.write_short(static_cast<short>(length)); // Song length
		file.write_short(layer_count); // Layer count
		for (int i = 0; i < 4; i++) file.write_c_string(""); // Song name, song author, song original author, song description
		file.write_short(1000); // Song tempo, (t/s) / 100
		file.write_bool(true); // Auto save
		file.write_byte(1); // Auto save interval
		file.write_byte(4); // Time signature (x / 4)
		for (int i = 0; i < 3; i++) file.write_int(0); // Minutes spent, left clicks, right clicks
		file.write_int(length); // Note blocks added
		file.write_int(0); // Note blocks removed
		file.write_c_string(""); // Imported MIDI/Schematic filename
		file.write_bool(false); // Looping
		file.write_byte(0); // Max loop count
		file.write_short(0); // Loop start tick
		double extended_semitones = 0;
		bool out_of_range = false;
		for (int index = 0; index < length - offset; index++)
		{
			Note note = notes[index];
			if (index == 0)
			{
				file.write_short(1); // Note on tick 0
				file.write_short(1); // Note on layer 0
			}
			else
			{
				file.write_short(0); // No more notes on the current layer
				file.write_short(1); // 1 note on the next tick
				file.write_short(1); // Note on layer 0
			}
			file.write_byte(static_cast<byte>(instrument != Instrument::CUSTOM ? note.get_instrument().get_instrument_id() : Instrument::MAX_VANILLA + 1 + custom_instrument_index)); // Set instrument
			if (!no_output_info && debug) cout << (note < Note("C9")) << ", " << (note > Note("C8")) << endl;
			if (!no_output_info && debug) cout << note.to_string() << endl;
			int id = note.get_id();
			if (note > Note("A-1") && note < Note("A0"))
			{
				if (!no_output_info && debug) cout << "note is in lower extended range" << endl;
				auto new_note = Note("A0");
				file.write_byte(new_note.get_id()); // NBS v5 note key is limited to 0-87, meaning we have to set to A0/C8 and adjust with pitch
				extended_semitones = new_note.compare(note);
			}
			else if (note < Note("C9") && note > Note("C8"))
			{
				if (!no_output_info && debug) cout << "note is in upper extended range" << endl;
				auto new_note = Note("C8");
				file.write_byte(new_note.get_id());
				extended_semitones = new_note.compare(note);
			}
			else if (note < Note("A-1") || note > Note("C9"))
			{
				if (!no_output_info && debug) cout << "id: " << id << endl;
				if (id < 0)
				{
					auto test = Note(id);
					while (test.get_note_type() < NoteType(0)) // A0
					{
						test.set_octave(test.get_octave() + 1);
					}
					file.write_byte(test.get_id());
				}
				else if (id > 87)
				{
					auto test = Note(id);
					while (test.get_note_type() > NoteType(87)) // C8
					{
						test.set_octave(test.get_octave() - 1);
					}
					file.write_byte(test.get_id());
				}
				else file.write_byte(id);
				out_of_range = true;
			}
			else file.write_byte(id); // Note key
			byte velocity = 100;
			if (fade_start_percent != -1 && fade_end_percent != -1)
			{
				// https://www.desmos.com/calculator/59t9zqhjqe
				if (fade_start_percent == fade_end_percent) velocity = fade_end_percent;
				else if (length > 1) switch (fade_mode)
				{
					case GenerationMode::LINEAR:
					{
						int totalLength = length - (include_end_note ? 1 : 0);
						velocity = static_cast<byte>(round((-index / static_cast<double>(totalLength)) * (fade_start_percent - fade_end_percent) + fade_start_percent));
						break;
					}
					case GenerationMode::QUADRATIC:
					{
						int totalLength = length - (include_end_note ? 1 : 0);
						double lengthSquareReciprocal = 1.0 / Utility::square(totalLength);
						int velocity_difference = fade_end_percent - fade_start_percent;
						if (flip_fade_quadratic_functions) velocity = static_cast<byte>(-velocity_difference * lengthSquareReciprocal * Utility::square(index - totalLength) + fade_end_percent);
						else velocity = static_cast<byte>(velocity_difference * lengthSquareReciprocal * Utility::square(index) + fade_start_percent);
						break;
					}
					case GenerationMode::RANDOM_NORMAL:
					{
						velocity = random.get_random_int(fade_start_percent, fade_end_percent);
						break;
					}
					default:
					{
						velocity = fade_end_percent;
					}
				}
			}
			file.write_byte(velocity); // Note velocity
			byte panning = 100; // Note panning, 100 is perfectly centered
			if (panning_mode != Panning::PAN_NONE)
			{
				if (int totalLength = length - (include_end_note ? 1 : 0); totalLength > 0) switch (panning_mode)
				{
					case Panning::PAN_LEFT_RIGHT:
						panning = static_cast<byte>(round((index / static_cast<double>(totalLength)) * 200));
						break;
					case Panning::PAN_RIGHT_LEFT:
						panning = static_cast<byte>(round(((-index / static_cast<double>(totalLength)) * 200)) + 200);
						break;
					case Panning::PAN_STEREO:
						panning = index % 2 == 0 ? 50 : 150; // Alternating from 50 panning left to 50 panning right every tick
						break;
					default:
						panning = 100; // Centered
						break;
				}
			}
			file.write_byte(panning); // Note panning
			auto new_cents = static_cast<short>(note.get_cents() + (extended_semitones * 100));
			if (new_cents < -1200)
			{
				extended_semitones = -32768;
				new_cents = -1200;
			}
			else if (new_cents > 1200)
			{
				extended_semitones = 32767;
				new_cents = 1200;
			}
			file.write_short(new_cents); // Note pitch
		}
		if (extended_semitones == -32768 || extended_semitones == 32767) cerr << "Warning: Generated notes' pitch was detected to use more than a 1200 cent offset and was adjusted to fit; note pitch on some notes may not be accurate!" << endl;
		if (out_of_range) cerr << "Warning: Some notes were out of range and have been adjusted to fit within the NBS v5 note key range (A0-C8)" << endl;
		for (int i = 0; i < 2; i++) file.write_short(0); // There are no more notes available
		for (int i = 0; i < layer_count; i++)
		{
			file.write_c_string(""); // Layer name
			file.write_bool(false); // Layer lock status
			file.write_byte(100); // Layer volume
			file.write_byte(100); // Layer panning
		}
		if (instrument != Instrument::CUSTOM) file.write_byte(0); // Custom instrument count
		else
		{
			file.write_byte(custom_instrument_index + 1); // Custom instrument count
			for (int i = 0; i < custom_instrument_index + 1; i++) // `<=` should be needed, as the number is actually the index of the instrument
			{
				file.write_c_string(("Custom " + std::to_string(custom_instrument_index + 1)).c_str()); // Instrument name
				file.write_c_string(""); // Sound file
				file.write_byte(45); // Custom instrument key, 45 = F#4
				file.write_bool(true); // Show playback on NBS piano
			}
		}
		file.flush();
		file.close();
		if (!no_output_info) cout << "File successfully saved as \"" << full_path.string() << "\"" << endl;
		if (!no_output_info) cout << "Closing in 5 seconds..." << endl;
		if (delay > 0) Sleep(delay);
	}
}

bool is_argument(const string& argument, const string& base_value)
{
	return is_argument(argument, std::initializer_list<std::string_view>{base_value});
}

// New: supports brace-lists like {"a","b"} directly
bool is_argument(string argument, const std::initializer_list<std::string_view> base_values)
{
	std::string_view prefixes[] = { "-", "--", "/" };
	int prefix_index = -1;
	for (const auto& prefix : prefixes)
	{
		if (argument.starts_with(prefix)) prefix_index++;
	}
	if (constexpr std::size_t length = std::size(prefixes); prefix_index < 0 || prefix_index >= static_cast<int>(length)) return false;
	argument = argument.substr(prefixes[prefix_index].size());
	if (argument.find('_') != std::string::npos)
	{
		for (char& argument_char : argument)
		{
			if (argument_char == '_') argument_char = '-';
		}
	}
	for (auto base_value_sv : base_values)
	{
		std::string base_value(base_value_sv);
		if (base_value.find('_') != std::string::npos)
		{
			for (char& base_value_char : base_value)
			{
				if (base_value_char == '_') base_value_char = '-';
			}
		}
		if (argument == base_value) return true;
	}
	return false;
}

// New: template that captures the array size N automatically (works with named arrays and temporary arrays)
template <std::size_t N>
bool is_argument(string argument, const char* const (&base_values)[N])
{
	std::string_view prefixes[] = { "-", "--", "/" };
	int prefix_index = -1;
	for (const auto& prefix : prefixes)
	{
		if (argument.starts_with(prefix)) prefix_index++;
	}
	if (constexpr std::size_t length = std::size(prefixes); prefix_index < 0 || prefix_index >= static_cast<int>(length)) return false;
	argument = argument.substr(prefixes[prefix_index].size());
	if (argument.find('_') != std::string::npos)
	{
		for (char& argument_char : argument)
		{
			if (argument_char == '_') argument_char = '-';
		}
	}
	for (std::size_t i = 0; i < N; ++i)
	{
		std::string base_value = base_values[i];
		if (base_value.find('_') != std::string::npos)
		{
			for (char& base_value_char : base_value)
			{
				if (base_value_char == '_') base_value_char = '-';
			}
		}
		if (argument == base_value) return true;
	}
	return false;
}

void print_help()
{
	cout << "Notes are standard musical notes followed by a number for their respective octave (i.e. F#4) with an optionally specified cent value preceding it (i.e: C4+32c)" << endl;
	cout << "Usage:" << endl;
	cout << "-no-delay: disables the 5 second delay on completion; useful for scripting!" << endl;
	cout << "-debug: enables debug mode, which outputs additional information about the generated progression" << endl;
	cout << "-no-info: disables outputting any printed text other than the generated progression (unless exporting as NBS, in which case this will output nothing) " << endl;
	cout << "\tand errors to the console; useful for parsing output with a script" << endl;
	cout << "-start: the first note to start the progression from (required)" << endl;
	cout << "-end: the last note to end the progression to (required)" << endl;
	cout << "-length: the length of the progression notes (required)" << endl;
	cout << "-include-base-note: includes the first note in the generated progression;" << endl;
	cout << "\tthis will also cause the progression to generate one less note (as the inputted first note will become the outputted first note)" << endl;
	cout << "-include-end-note: includes the last note in the generated progression;" << endl;
	cout << "\tlike with -include-base-note, this will also cause the progression to generate one less note (as the inputted last note will become the outputted last note)" << endl;
	cout << "\t-include-base-note and -include-end-note can be used together!" << endl << endl;
	cout << "-note-mode: allows you to change the note generation mode;" << endl;
	cout << "\tlinear: a generation type that changes the note key consistently over time; for example, for a full C scale: C, C#, D, D#, E, ..." << endl;
	cout << "\tquadratic: a generation type that changes the note key increasingly over time; for example, for the same scale: C, E-3c, F#-35c, G4+14, ... C-40c, C-10c, C+1" << endl;
	cout << "\t\tthis causes the per-note jumps start off extremely rapid, but significantly slow down toward the end;" << endl;
	cout << "\t\t-flip_note_quadratic_functions: reverses the functions used for quadratic note generation, effectively flipping the order of the generation;" << endl;
	cout << "\t\tthis can be useful for making the outputted note progression have a different feel, such as *starting* with a large drop-off instead of slowly changing to it" << endl << endl;
	cout << "\trandom: a generation type that generates notes randomly between the start and end note;" << endl;
	cout << "\trandom-linear: similar to random, except instead of generating entirely random notes," << endl;
	cout << "\t\tit generates random segments that linearly transition between two randomly generated notes;" << endl;
	cout << "\trandom-quadratic: similar to random-linear, except instead of generating a linear transition, this will generate a quadratic segments instead;" << endl;
	cout << "\t\tthese can help smooth the randomness and feel more natural" << endl;
	cout << "\tstatic: a generation type that outputs the same note repeatedly for the entire length of the progression" << endl;
	cout << "\tuseful if you the instrument only needs a fade or is percussion!" << endl;
	cout << "Random note generation options:" << endl;
	cout << "\t-random-segment-period: if generating notes randomly with random-linear or random-quadratic modes, this specifies the maximum length of each random segment;" << endl;
	cout << "\t\tif specfied, the segment length will be constant to what you set (unless the last segment is longer than the target length, in which case it will be clamped appropriately)" << endl;
	cout << "\t\totherwise, this will be randomly chosen *for each segment* between 4 and 10, again not exceeding the total provided length;" << endl;
	cout << "\t\tthis value cannot be greater than the total length of the progression minus any included base/end notes!" << endl;
	cout << "\t\t-random-pitch: if generating notes randomly, this will give the random notes a random pitch within [-49, 50] cents" << endl;
	cout << "NBS options:" << endl;
	cout << "-nbs: enables exporting as a Note Block Studio song (.nbs) instead of as text;" << endl;
	cout << "\t-nbs also enables other arguments for exporting:" << endl;
	cout << "-instrument: specifies the instrument to use for the notes;" << endl;
	cout << "\tMust be a valid note block instrument, such as pling or harp;" << endl;
	cout << "\tthis can also be \"custom\" for a custom instrument too; however, you may want to consider setting the next argument:" << endl;
	cout << "-custom-instrument-index: if using a custom instrument, this specifies the index of the custom instrument to output as;" << endl;
	cout << "\tif using the first custom instrument, this is actually implicit and automatically set;" << endl;
	cout << "\tkeep in mind that the first custom instrument starts at index 0!" << endl;
	cout << "Fade options:" << endl;
	cout << "-fade-start: specifies the minimum velocity the fade should start from; cannot be lower than 1 or greater than 100!" << endl;
	cout << "-fade-end: specifies the maximum velocity the fade should end on; cannot be less than one, less than the fade start percent, or greater than 100!" << endl;
	cout << "If outputting with a fade, *both* -fade-start and -fade-end must be specified!" << endl;
	cout << "-fade-mode: specifies the type of fade to output as; this works in the same way as for the note generation mode, but for note velocity:" << endl;
	cout << "\tlinear: a type of fade that changes the velocity consistently; for example for 4 notes: 25%, 50%, 75%, 100%" << endl;
	cout << "\tquadratic: a type of fade that changes the velocity increasingly over time; for example for 4 notes: 12%, 25%, 50%, 100%" << endl;
	cout << "\t\tcan be more effective for rapid build ups!" << endl;
	cout << "\t\t-flip_fade_quadratic_functions: reverses the functions used for quadratic fade generation;" << endl;
	cout << "\t\tthis has exactly the same effect as -flip_note_quadratic_functions, except for velocity generation instead" << endl;
	cout << "Panning options:" << endl;
	cout << "-panning: specifies the type of panning to output as;" << endl;
	cout << "\tleft_to_right: pans all the notes from the left channel to the right" << endl;
	cout << "\tright_to_left: pans all the notes from the right channel to the left" << endl;
	cout << "\tstereo: alternates notes from the left channel and the right, creating a stereo effect when played" << endl;
}

void print_output(const std::vector<Note>& notes, bool no_output_info)
{
	if (!no_output_info) cout << "Requested transition: " << endl;
	for (int i = 0; i < notes.size(); i++)
	{
		cout << notes[i].to_string();
		if (i < notes.size() - 1) cout << "; ";
	}
	cout << endl;
}

Note generate_random_note(Random &random, Note start, Note end, const Instrument instrument, const bool random_pitch, const RandomPattern& random_pattern)
{
	Note result;
	if (end < start)
	{
		const Note temp = start;
		start = end;
		end = temp;
	}
	if (!random_pitch && start.equals_ignore_pitch(end) || start == end) result = end;
	else
	{
		do
		{
			result = Note(random.get_random_int(start.get_id(), end.get_id()));
		}
		while (!random_pattern.is_allowed(result));
	}
	result.set_instrument(instrument);
	if (random_pitch) do
	{
		result.set_cents(random.get_random_int(-49, 50));
	}
	while ((result.equals_ignore_pitch(start) && start.get_cents() < 0 ? result.get_cents() < start.get_cents() : result.get_cents() > start.get_cents()) || (result.equals_ignore_pitch(end) && result.get_cents() > 0 && result.get_cents() > end.get_cents()));
	else result.set_cents(static_cast<int>(std::round((start.get_cents() + end.get_cents()) / 2.0)));
	return result;
}

vector<Note> get_note_progression(const Note segment_start, const Note segment_end, const int segment_length, const GenerationMode note_mode, const bool include_base_note, const bool include_end_note,
                                  const bool flip_quadratic_functions, const RandomData& random_data, const OutputData& output_data)
{
	vector<Note> notes;
	Note start_note = segment_start.clone();
	Note end_note = segment_end.clone();
	int offset = 2;
	if (include_base_note) offset--;
	if (include_end_note) offset--;
	switch (note_mode)
	{
		case GenerationMode::STATIC:
		case GenerationMode::RANDOM_NORMAL:
		{
			for (int i = 0; i < segment_length - offset; i++)
			{
				Note note;
				if (note_mode == GenerationMode::STATIC) note = segment_start.clone();
				else note = generate_random_note(random_data.random, segment_start, segment_end, segment_start.get_instrument(), random_data.random_pitches, random_data.pattern);
				notes.push_back(note);
			}
			break;
		}
		case GenerationMode::LINEAR:
		case GenerationMode::QUADRATIC:
		case GenerationMode::RANDOM_LINEAR:
		case GenerationMode::RANDOM_QUADRATIC:
		{
			while (notes.size() < segment_length - (include_end_note ? 1 : 0))
			{
				int64_t total_length = note_mode.is_random() ? std::min(static_cast<int64_t>(random_data.random_segment_period), static_cast<int64_t>(segment_length - notes.size())) : segment_length;
				int64_t random_length = -1;
				if (note_mode.is_random() && total_length < 0) total_length = random_length = random_data.random.get_random_int64(4, std::min(10LL, static_cast<int64_t>(segment_length - notes.size())));
				if (random_length != -1 && output_data.output_enabled && output_data.debug) cout << "Generated random segment length of " << random_length << " notes" << endl;
				if (notes.size() + total_length == segment_length - 1) total_length++;
				const bool is_first_segment = notes.empty();
				const bool is_last_segment = notes.size() + total_length == segment_length;
				if (is_first_segment || is_last_segment)
				{
					if (is_first_segment && !include_base_note) total_length++;
					if (is_last_segment && !include_end_note) total_length++;
				}
				if (!is_last_segment && note_mode.is_random()) while (true)
				{
					if (Note random_note = generate_random_note(random_data.random, segment_start, segment_end, segment_start.get_instrument(), random_data.random_pitches, random_data.pattern); !end_note.equals_ignore_pitch(random_note))
					{
						end_note = random_note;
						break;
					}
				}
				else if (note_mode.is_random()) end_note = segment_end.clone();
				if (total_length == 2 && total_length == random_length)
				{
					notes.push_back(start_note);
					notes.push_back(end_note);
					start_note = end_note;
					continue;
				}
				double current_precision_loss = 0;
				double cent_step_exact = 0;
				const double semitone_difference = start_note.compare(end_note);
				int cent_step = 0;
				if (note_mode == GenerationMode::LINEAR || note_mode == GenerationMode::RANDOM_LINEAR)
				{
					cent_step_exact = ((semitone_difference / static_cast<double>(total_length - (include_end_note ? 1 : 0))) * 100.0);
					if (output_data.output_enabled && output_data.debug) cout << "exact cent step: " << cent_step_exact << endl;
					cent_step = static_cast<int>(cent_step_exact < 0 ? ceil(cent_step_exact) : floor(cent_step_exact));
				}
				Note current_note = start_note.clone();
				int cent_offset = 0;
				const uint64_t loop_start = !is_first_segment ? 0 : include_base_note ? 0 : 1;
				const uint64_t loop_end = total_length - (!is_last_segment ? 0 : (include_end_note ? 0 : 1));
				for (uint64_t i = loop_start; i < loop_end; i++)
				{
					Note last_note;
					if (note_mode == GenerationMode::QUADRATIC || note_mode == GenerationMode::RANDOM_QUADRATIC)
					{
						// https://www.desmos.com/calculator/9jv1asddng
						double target_semitone;
						const double lengthSquareReciprocal = 1.0 / static_cast<double>(Utility::square(total_length));
						const int note_distance = end_note.get_id() - start_note.get_id();
						if (flip_quadratic_functions) target_semitone = (-note_distance * lengthSquareReciprocal * static_cast<double>(Utility::square(i - total_length)) + end_note.get_id());
						else target_semitone = (note_distance * lengthSquareReciprocal * static_cast<double>(Utility::square(i)) + start_note.get_id());
						cent_step_exact = -round((((notes.empty() ? start_note.get_id() : notes[notes.size() - 1].get_id()) * 100) + (notes.empty() ? start_note : notes[notes.size() - 1]).get_cents() - (target_semitone * 100)));
						cent_step = static_cast<int>(cent_step_exact < 0 ? ceil(cent_step_exact) : floor(cent_step_exact));
					}
					last_note = current_note.clone();
					if ((i > 0 || !include_base_note))
					{
						current_precision_loss += cent_step_exact - cent_step;
						if (current_precision_loss != 0 && abs(current_precision_loss / 0.495) >= 1)
						{
							if (output_data.output_enabled && output_data.debug) cout << "new precision loss: " << current_precision_loss << endl;
							cent_offset = static_cast<int>(round(current_precision_loss + 0.051));
							if (output_data.output_enabled && output_data.debug) cout << "detected precision loss, adding " << cent_offset << " cents" << endl;
							current_precision_loss -= cent_offset;
						}
						current_note.offset_cents(cent_step + cent_offset);
						cent_offset = 0;
					}
					notes.push_back(current_note.clone());
				}
				if (note_mode.is_random() && !is_last_segment) start_note = notes.back().clone();
				if (output_data.output_enabled && output_data.debug && random_length != -1)
				{
					cout << "Created segment of length " << random_length << ": ";
					for (uint64_t i = notes.size() - random_length; i < notes.size(); i++)
					{
						cout << notes[i].to_string();
						if (i < notes.size() - 1) cout << ", ";
						else cout << endl;
					}
				}
				if ((!include_end_note && !Utility::is_near(notes.back().compare(end_note), cent_step_exact / 100.0, 0.01)) || (include_end_note && notes.back() != end_note))
				{
					int total_cent_offset = end_note.get_cents() - notes.back().get_cents();
					if (!include_end_note) total_cent_offset -= cent_step;
					uint64_t start_point = 0;
					for (int local_cent_offset = 0; local_cent_offset < abs(total_cent_offset); local_cent_offset++)
					{
						start_point = static_cast<uint64_t>(0.75 * static_cast<double>(random_data.random.get_random_uint64(start_point, random_length != -1 ? random_length : notes.size())));
						if (start_point == notes.size()) start_point = 0;
						int note_offset = total_cent_offset / abs(total_cent_offset);
						notes.back().offset_cents(note_offset);
						std::size_t fixer_loop_start = (random_length == -1 ? start_point : notes.size() - random_length);
						if (is_first_segment && include_base_note && fixer_loop_start == 0) fixer_loop_start++;
						std::size_t fixer_loop_end = notes.size();
						if (is_last_segment && include_end_note) fixer_loop_end--;
						for (uint64_t i = fixer_loop_start; i < fixer_loop_end; i++) notes[i].offset_cents(note_offset);
					}
				}
			}
			break;
		}
		default: {}
	}
	return notes;
}
