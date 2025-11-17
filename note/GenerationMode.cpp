#include "GenerationMode.h"

int GenerationMode::get_fade_id() const
{
    return note_generation_type;
}

GenerationMode GenerationMode::get_generation_mode(std::string name)
{
    std::ranges::transform(name, name.begin(), toupper);
	Utility::replace_all(name, "-", "_");
	if (name == "STATIC") return STATIC;
	else if (name == "LINEAR") return LINEAR;
	else if (name == "QUADRATIC") return QUADRATIC;
	else if (name == "RANDOM" || name == "RANDOM_NORMAL") return RANDOM_NORMAL;
	else if (name == "RANDOM_LINEAR") return RANDOM_LINEAR;
	else if (name == "RANDOM_QUADRATIC") return RANDOM_QUADRATIC;
	else throw std::invalid_argument("The inputted mode \"" + name + "\" does not correspond to any valid fade mode");
}

bool GenerationMode::is_random() const
{
	return note_generation_type == RANDOM_NORMAL || note_generation_type == RANDOM_LINEAR || note_generation_type == RANDOM_QUADRATIC;
}

bool GenerationMode::operator==(const GenerationMode other_fade) const
{
	return note_generation_type == other_fade.note_generation_type;
}

bool GenerationMode::operator==(const GenerationType other_fade_type) const
{
	return note_generation_type == other_fade_type;
}
