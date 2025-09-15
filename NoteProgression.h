#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <filesystem>

#include "Random.h"
#include "RandomPattern.h"
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
using std::string;
using std::cout;
using std::cin;

bool is_argument(const string& argument, const string& base_value);
bool is_argument(string argument, std::initializer_list<std::string_view> base_values);
void print_help();
void print_output(const std::vector<Note>& notes, bool no_output_info = false);
[[nodiscard]] Note generate_random_note(Random &random, Note start, Note end, Instrument instrument, bool random_pitch, RandomPattern random_pattern);
