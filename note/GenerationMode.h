#pragma once

#include <string>
#include <algorithm>
#include <stdexcept>

#include "Utility.h"

class GenerationMode
{
public:
	enum GenerationType
	{
		STATIC,
		LINEAR,
		QUADRATIC,
		RANDOM_NORMAL,
		RANDOM_LINEAR,
		RANDOM_QUADRATIC
	};
	GenerationMode() = default;
	constexpr GenerationMode(const GenerationType fade_type) : note_generation_type(fade_type) {}
	constexpr operator GenerationType() const { return note_generation_type; }
	explicit operator bool() const = delete;
	[[nodiscard]] int get_fade_id() const;
	static GenerationMode get_generation_mode(std::string name);
	bool is_random() const;
	bool operator==(GenerationMode other_fade) const;
	bool operator==(GenerationType other_fade_type) const;
private:
	GenerationType note_generation_type;
};

