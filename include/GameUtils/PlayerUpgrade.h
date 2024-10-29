#pragma once
#include "Core.h"

class PlayerUpgrade {
public:
    // key = Rarity
    static map<int, vector<PlayerUpgrade>> AvailableUpgrades;
    static const PlayerUpgrade None;

    inline PlayerUpgrade(string name, int rarity, string description, function<void()> onInvoke)
        :Name(name), Rarity(rarity), Description(description), Invoke(onInvoke) {}

    const int Rarity;
    const string Name;
    const string Description;
    const function<void()> Invoke;
};