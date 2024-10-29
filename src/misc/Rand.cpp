
#include "Rand.h"

#include <random>

float Rand::Float(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(min, max);

    return dis(gen);
}

float Rand::Int(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dis(min, max);

    return dis(gen);
}

int Rand::Rarity(float chance_lengend, float chance_epic, float chance_rare)
{
    float rng = Rand::Float(0.0f, 1.0f);
    if (rng <= chance_lengend)
        return Rarity::Legend;
    rng -= chance_lengend;
    if (rng <= chance_epic)
        return Rarity::Epic;
    rng -= chance_epic;
    if (rng <= chance_rare)
        return Rarity::Rare;
    return Rarity::Common;
}

const PlayerUpgrade& Rand::PlayerUpgrade()
{
        auto& pool = PlayerUpgrade::AvailableUpgrades[Rand::Rarity(0.02f, 0.10f, 0.30f)];
        if (pool.empty())
            return PlayerUpgrade::None;
        int i = Rand::Int(0, pool.size() - 1);
        return pool[i];
}
