#pragma once

class Rand {
public:
    static float RandomFloat(float min, float max);
    static float RandomInt(int include_min, int exclude_max);
};