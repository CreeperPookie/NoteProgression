#pragma once

#include <string>

class Instrument
{
public:
	enum InstrumentType
	{
		HARP,
		DOUBLE_BASS,
		BASS_DRUM,
		SNARE,
		CLICK,
		GUITAR,
		FLUTE,
		BELL,
		CHIME,
		XYLOPHONE,
		IRON_XYLOPHONE,
		COW_BELL,
		DIDGERIDOO,
		BIT,
		BANJO,
		PLING,
		CUSTOM,
		MAX_VANILLA = PLING, // comparison only!
		MAX = CUSTOM
	};
	Instrument() = default;
	constexpr Instrument(const InstrumentType instrument_type) : instrument_type(instrument_type) {}
	int get_instrument_id() const;
	static Instrument get_instrument(std::string name);
	bool operator==(Instrument instrument) const;
private:
	InstrumentType instrument_type;
};