#include "Instrument.h"

#include <algorithm>
#include <stdexcept>

int Instrument::get_instrument_id() const
{
	return instrument_type;
}

Instrument Instrument::get_instrument(std::string name)
{
	std::ranges::transform(name, name.begin(), ::toupper);
	if (name == "HARP") return HARP;
	else if (name == "DOUBLE_BASS") return DOUBLE_BASS;
	else if (name == "BASS_DRUM") return BASS_DRUM;
	else if (name == "SNARE") return SNARE;
	else if (name == "CLICK") return CLICK;
	else if (name == "GUITAR") return GUITAR;
	else if (name == "FLUTE") return FLUTE;
	else if (name == "BELL") return BELL;
	else if (name == "CHIME") return CHIME;
	else if (name == "XYLOPHONE") return XYLOPHONE;
	else if (name == "IRON_XYLOPHONE") return IRON_XYLOPHONE;
	else if (name == "COW_BELL") return COW_BELL;
	else if (name == "DIDGERIDOO") return DIDGERIDOO;
	else if (name == "BIT") return BIT;
	else if (name == "BANJO") return BANJO;
	else if (name == "PLING") return PLING;
	else throw std::invalid_argument("The inputted instrument does not correspond to a valid Minecraft note block instrument");
}

bool Instrument::operator==(const Instrument instrument) const
{
	return instrument_type == instrument.instrument_type;
}
