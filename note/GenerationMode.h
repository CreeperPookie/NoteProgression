#pragma once

#include <string>

class GenerationMode
{
public:
	enum GenerationType
	{
		STATIC,
		LINEAR,
		QUADRATIC,
		RANDOM
	};
	GenerationMode() = default;
	constexpr GenerationMode(const GenerationType fade_type) : note_generation_type(fade_type) {}
	constexpr operator GenerationType() const { return note_generation_type; }
	explicit operator bool() const = delete;
	[[nodiscard]] int get_fade_id() const;
	static GenerationMode get_generation_mode(std::string name);
	bool operator==(GenerationMode other_fade) const;
	bool operator==(GenerationType other_fade_type) const;
private:
	GenerationType note_generation_type;
};

