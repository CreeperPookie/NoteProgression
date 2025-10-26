#pragma once

#include <string>

class Panning
{
public:
	enum PanningType
	{
		PAN_NONE,
		PAN_LEFT_RIGHT,
		PAN_RIGHT_LEFT,
		PAN_STEREO
	};
	Panning() = default;
	constexpr Panning(const PanningType panning_type) : panning(panning_type) {}
	constexpr operator PanningType() const { return panning; }
	explicit operator bool() const = delete;
	int get_panning_id() const;
	static Panning get_panning_type_by_name(std::string name);
	bool operator==(Panning other_panning) const;
	bool operator==(PanningType other_panning_type) const;
private:
	PanningType panning;
};

