//
// Created by CreeperPookie on 4/2/2025.
//

#include "Utility.h"

vector<int> Utility::get_factors(const int number)
{
	vector<int> factors;
	for (int i = 1; i <= number; i++)
	{
		if (number % i == 0) factors.push_back(i);
	}
	return factors;
}

int Utility::get_gcf(const int a)
{
	int gcf = 1;
	for (int i = 2; i <= a; i++)
	{
		if (a % i == 0) gcf = i;
	}
	return gcf;
}

int Utility::get_gcd(const int a, const int b)
{
	if (b == 0) return a;
	return get_gcd(b, a % b);
}

int Utility::square(const int x)
{
	return x * x;
}

int64_t Utility::square(const int64_t x)
{
	return x * x;
}

uint64_t Utility::square(const uint64_t x)
{
	return x * x;
}

double Utility::square(const double x)
{
	return x * x;
}

bool Utility::is_near(const double value, const double comparison, const double total_tolerance)
{
	return std::abs(value - comparison) <= std::abs(total_tolerance);
}

void Utility::replace_all(std::string& string, const std::string& str, const std::string& replacement)
{
	if (string.empty()) return;
	while (string.find(str) != std::string::npos)
	{
		const size_t pos = string.find(str);
		string.replace(pos, str.size(), replacement);
	}
}

std::vector<std::string> Utility::split(const std::string& string, const std::string& delimiter)
{
	std::vector<std::string> tokens;
	if (string.empty()) return tokens;
	size_t lastMatch = 0;
	while (string.find(delimiter, lastMatch) != std::string::npos)
	{
		size_t match = string.find(delimiter, lastMatch);
		tokens.push_back(string.substr(lastMatch, match - lastMatch));
		lastMatch = match + delimiter.size();
	}
	tokens.push_back(string.substr(lastMatch));
	return tokens;
}
