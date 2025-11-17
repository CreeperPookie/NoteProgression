#include "Random.h"

Random::Random() : generator(std::mt19937{ device() }) {}

bool Random::get_random_bool()
{
	return get_random_int(0, 1) == 1;
}

int Random::get_random_int(int min, int max)
{
	if (min > max) std::swap(min, max);
	std::uniform_int_distribution distribution(min, max);
	return distribution(generator);
}

int64_t Random::get_random_int64(int64_t min, int64_t max)
{
	if (min > max) std::swap(min, max);
	std::uniform_int_distribution distribution(min, max);
	return distribution(generator);
}

uint64_t Random::get_random_uint64(uint64_t min, uint64_t max)
{
	if (min > max) std::swap(min, max);
	std::uniform_int_distribution distribution(min, max);
	return distribution(generator);
}

double Random::get_random_double(double min, double max)
{
	if (min > max) std::swap(min, max);
	std::uniform_real_distribution distribution
	(
		min,
		std::nextafter(max, std::numeric_limits<double>::infinity())
	);
	return distribution(generator);
}

