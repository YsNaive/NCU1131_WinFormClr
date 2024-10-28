
#include "Rand.h"

#include <random>

float Rand::RandomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(min, max);

    return dis(gen);
}

float Rand::RandomInt(int include_min, int exclude_max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dis(include_min, exclude_max - 1);

    return dis(gen);
}