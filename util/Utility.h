#pragma once

#include <cstdint>
#include <vector>
#include <bits/basic_string.h>

using std::vector;

namespace Utility
{
	vector<int> get_factors(int number);
	int get_gcf(int a);
	int get_gcd(int a, int b);
	int square(int x);
	int64_t square(int64_t x);
	uint64_t square(uint64_t x);
	double square(double x);
	bool is_near(double value, double comparison, double total_tolerance);
	void replace_all(std::string& string, const std::string& str, const std::string& replacement);
	std::vector<std::string> split(const std::string& string, const std::string& delimiter = ",");
}
