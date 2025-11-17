#pragma once
#include <random>

class Random
{
    std::random_device device;
    std::mt19937 generator;
public:
    Random();
    // Prevent accidental copying (duplicating RNG state); allow moves.
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;
    Random(Random&&) noexcept = delete;
    Random& operator=(Random&&) noexcept = delete;
    bool get_random_bool();
    int get_random_int(int min = 0, int max = 1);
    int64_t get_random_int64(int64_t min = 0, int64_t max = 1);
    uint64_t get_random_uint64(uint64_t min = 0, uint64_t max = 1);
    double get_random_double(double min = 0.0, double max = 1.0);
};
