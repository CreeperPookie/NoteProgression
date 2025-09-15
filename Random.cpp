#include "Random.h"

Random::Random() : generator(std::mt19937{ device() }) {}

int Random::get_random_int(int min, int max)
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

