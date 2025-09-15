#pragma once
#include <string>
#include <vector>

#include "Note.h"
#include "RandomPatternSegment.h"

class RandomPattern
{
	std::vector<RandomPatternSegment> segments;
	std::string pattern;
public:
	RandomPattern() : RandomPattern("") {}
	RandomPattern(const std::string& pattern);
	std::string getPattern() const { return pattern; }
	RandomPattern& operator=(const std::string& newPattern);
	void add_segments_from_pattern(const std::string& pattern_string);
	static bool validatePattern(std::string pattern);
	bool is_allowed(const Note& note) const;
};
