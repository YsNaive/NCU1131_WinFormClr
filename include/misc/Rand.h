#pragma once
#include "Core.h"
#include "PlayerUpgrade.h"

class Rand {
public:
    static float Float(float min, float max);
    static float Int(int min, int max);
    static int   Rarity(float chance_lengend, float chance_epic, float chance_rare);
    static const PlayerUpgrade& PlayerUpgrade();
};