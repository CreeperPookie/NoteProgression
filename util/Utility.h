#pragma once

#include <vector>
#include <bits/basic_string.h>

using std::vector;

namespace Utility
{
	vector<int> get_factors(int number);
	int get_gcf(int a);
	int get_gcd(int a, int b);
	int square(int x);
	double square(double x);
	void replaceAll(std::string& string, const std::string& str, const std::string& replacement);
	std::vector<std::string> split(const std::string& string, const std::string& delimiter = ",");
}
