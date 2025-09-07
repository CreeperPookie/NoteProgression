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
