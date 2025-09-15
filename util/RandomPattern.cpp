#include "RandomPattern.h"

#include "RandomPatternSegment.h"
#include "Utility.h"

RandomPattern::RandomPattern(const std::string& pattern)
{
	add_segments_from_pattern(pattern);
}

RandomPattern& RandomPattern::operator=(const std::string& newPattern)
{
	this->segments.clear();
	this->pattern = newPattern;
	add_segments_from_pattern(newPattern);
	return *this;
}

void RandomPattern::add_segments_from_pattern(const std::string& pattern_string)
{
	for (const string segment : Utility::split(pattern_string, ","))
	{
		try
		{
			this->segments.push_back(RandomPatternSegment(segment));
		}
		catch (std::invalid_argument &e)
		{
			throw std::invalid_argument("Invalid pattern segment: " + segment);
		}
	}
}

bool RandomPattern::validatePattern(std::string pattern)
{
	Utility::replaceAll(pattern, ";", ",");
	Utility::replaceAll(pattern, " ", "");
	if (pattern.find(";") != std::string::npos)
	{
		for (string segment : Utility::split(pattern, ","))
		{
			if (segment.starts_with("!")) segment = segment.substr(1);
			if (string note_type = segment.substr(0, segment.size() >= 2 && segment[1] == '#' ? 2 : 1); !NoteType::validate_note(note_type)) return false;
			try
			{
				auto temp = Note(segment);
			}
			catch (std::invalid_argument &e)
			{
				return false;
			}
		}
	}
	return true;
}

bool RandomPattern::is_allowed(const Note& note) const
{
	if (segments.empty()) return true;
	for (const RandomPatternSegment& segment : segments)
	{
		if (segment.matches(note)) return true;
	}
	return false;
}
