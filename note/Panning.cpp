#include "Panning.h"

#include <algorithm>
#include <stdexcept>

int Panning::get_panning_id() const
{
    return panning;
}

Panning Panning::get_panning_type_by_name(std::string name)
{
    std::ranges::transform(name, name.begin(), ::toupper);
	if (name.starts_with("PAN_")) name = name.substr(4);
	if (name == "NONE") return PAN_NONE;
	else if (name == "LEFT_RIGHT") return PAN_LEFT_RIGHT;
	else if (name == "RIGHT_LEFT") return PAN_RIGHT_LEFT;
	else if (name == "STEREO") return PAN_STEREO;
	else throw std::invalid_argument("The inputted panning type does not correspond to a valid panning type");
}

bool Panning::operator==(const Panning other_panning) const
{
	return panning == other_panning.panning;
}

bool Panning::operator==(const PanningType other_panning_type) const
{
	return panning == other_panning_type;
}
