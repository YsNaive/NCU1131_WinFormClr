#pragma once
#include "Core.h"

class PlayerUpgrade {
private:
    function<void()> invoke;
public:
    // key = Rarity
    static map<int, vector<PlayerUpgrade*>> AllUpgrades;
    static map<int, vector<PlayerUpgrade*>> AvailableUpgrades;
    static map<PlayerUpgrade*, int>         AppliedCount;
    static PlayerUpgrade None;

    inline PlayerUpgrade(string name, int rarity, string description, function<void()> onInvoke, int applyLimit = -1)
        :Name(name), Rarity(rarity), Description(description), invoke(onInvoke), ApplyLimit(applyLimit) {}

    void ApplyOnPlayer();
    const int ApplyLimit;
    const int Rarity;
    const string Name;
    const string Description;
};