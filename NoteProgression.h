#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <filesystem>

#include "GenerationMode.h"
#include "OutputData.h"
#include "util/Random.h"
#include "note/random/RandomData.h"
#include "note/random/RandomPattern.h"
#include "note/Note.h"

using std::cerr;

enum FadeType
{
	FADE_NONE,
	FADE_IN,
	FADE_OUT,
	FADE_IN_OUT
};

using std::floor;
using std::round;
using std::ceil;
using std::pow;
using std::vector;
using std::string;
using std::cout;
using std::cin;

bool is_argument(const string& argument, const string& base_value);
bool is_argument(string argument, std::initializer_list<std::string_view> base_values);
void print_help();
void print_output(const vector<Note>& notes, bool no_output_info = false);
[[nodiscard]] Note generate_random_note(Random &random, Note start, Note end, Instrument instrument, bool random_pitch, const RandomPattern& random_pattern);
[[nodiscard]] vector<Note> get_note_progression(Note segment_start, Note segment_end, int segment_length, GenerationMode note_mode, bool include_base_note, bool include_end_note,
                                                bool flip_quadratic_functions, const RandomData& random_data, const OutputData& output_data);
