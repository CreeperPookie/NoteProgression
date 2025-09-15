#include "GenerationMode.h"

#include <algorithm>
#include <stdexcept>

int GenerationMode::get_fade_id() const
{
    return note_generation_type;
}

GenerationMode GenerationMode::get_generation_mode(std::string name)
{
    std::ranges::transform(name, name.begin(), toupper);
	if (name == "LINEAR") return LINEAR;
	else if (name == "QUADRATIC") return QUADRATIC;
	else throw std::invalid_argument("The inputted mode \"" + name + "\" does not correspond to any valid fade mode");
}

bool GenerationMode::operator==(const GenerationMode other_fade) const
{
	return note_generation_type == other_fade.note_generation_type;
}

bool GenerationMode::operator==(const GenerationType other_fade_type) const
{
	return note_generation_type == other_fade_type;
}
