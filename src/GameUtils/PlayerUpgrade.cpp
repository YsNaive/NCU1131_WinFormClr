#include "PlayerUpgrade.h"
#include "Global.h"
#include "Player.h"

map<int, vector<PlayerUpgrade>> PlayerUpgrade::AvailableUpgrades = map<int, vector<PlayerUpgrade>>();
const PlayerUpgrade PlayerUpgrade::None = PlayerUpgrade("None", Rarity::None, "�o�F�褣�ӥX�{", [](){});

namespace {
    vector<EntityModifier*> modifierBuffer;
    auto playerUpgrade_reset =
    GameReset::Create([]() {
        for (auto* obj : modifierBuffer)
            delete obj;
        modifierBuffer.clear();
    });

    auto playerUpgrade_start =
    Start::Create([]() {
    for (auto& pair : { make_pair(Rarity::Common, 5 ),
                        make_pair(Rarity::Rare  , 10),
                        make_pair(Rarity::Epic  , 15),
                        make_pair(Rarity::Legend, 20)}){
        PlayerUpgrade::AvailableUpgrades[pair.first].push_back(PlayerUpgrade(
            "�l�u��}", 
            pair.first,
            FormatString(" - �y���ˮ` + %d", pair.second),
            [pair]() {
                auto modifier = new EntityModifier(
                    EntityModifierTypes::None | EntityModifierTypes::IsBuff,
                    EntityModifierKey  ::Atk,
                    EntityModifierOP   ::ADD, pair.second);
                modifierBuffer.push_back(modifier);
                Global::Player->AddModifier(modifier, numeric_limits<float>().max());
            }
        ));
    }
    for (auto& pair : { make_pair(Rarity::Common, 0.25),
                        make_pair(Rarity::Rare  , 0.50),
                        make_pair(Rarity::Epic  , 0.75),
                        make_pair(Rarity::Legend, 1.00) }) {
        PlayerUpgrade::AvailableUpgrades[pair.first].push_back(PlayerUpgrade(
            "�i�a��}",
            pair.first,
            FormatString(" - ���ʳt�� + %.2f", pair.second),
            [pair]() {
                auto modifier = new EntityModifier(
                    EntityModifierTypes::None | EntityModifierTypes::IsBuff,
                    EntityModifierKey  ::Spd,
                    EntityModifierOP   ::ADD, pair.second);
                modifierBuffer.push_back(modifier);
                Global::Player->AddModifier(modifier, numeric_limits<float>().max());
            }
        ));
    }
    });
}

