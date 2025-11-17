#pragma once

#include <string>
#include "../../util/Random.h"
#include "RandomPattern.h"

using std::string;

struct RandomData
{
	Random& random;
	bool random_pitches;
	int random_segment_period;
	RandomPattern pattern;
	RandomData(Random& random, const bool random_pitches, const int random_segment_period, const RandomPattern& random_pattern) : random(random), random_pitches(random_pitches), random_segment_period(random_segment_period), pattern(random_pattern) {}
};
