#include "PlayerUpgrade.h"
#include "Global.h"
#include "Player.h"

map<int, vector<PlayerUpgrade*>> PlayerUpgrade::AllUpgrades;
map<int, vector<PlayerUpgrade*>> PlayerUpgrade::AvailableUpgrades;
map<PlayerUpgrade*, int>         PlayerUpgrade::AppliedCount;
PlayerUpgrade PlayerUpgrade::None = PlayerUpgrade("None", Rarity::None, "沒得抽啦", [](){});

void PlayerUpgrade::ApplyOnPlayer()
{
    invoke();
    PlayerUpgrade::AppliedCount[this]++;
    if (ApplyLimit == -1)
        return;
    if (PlayerUpgrade::AppliedCount[this] >= ApplyLimit) {
        auto& arr = AvailableUpgrades[Rarity];
        auto todelete = find(arr.begin(), arr.end(), this);
        if(todelete != arr.end())
            arr.erase(todelete);
    }

}

namespace {
    vector<EntityModifier*> modifierBuffer;
    auto playerUpgrade_reset =
    GameReset::Create([]() {
        for (auto* obj : modifierBuffer)
            delete obj;
        modifierBuffer.clear();
        for (auto& pair : PlayerUpgrade::AllUpgrades) {
            for (auto& upgrade : pair.second)
                PlayerUpgrade::AppliedCount[upgrade] = 0;
        }
        PlayerUpgrade::AvailableUpgrades = PlayerUpgrade::AllUpgrades;
    });

    auto playerUpgrade_start =
    Start::Create([]() {

    for (auto& pair : { make_pair(Rarity::Common, 5 ),
                        make_pair(Rarity::Rare  , 10),
                        make_pair(Rarity::Epic  , 15),
                        make_pair(Rarity::Legend, 20)}){
        PlayerUpgrade::AllUpgrades[pair.first].push_back(new PlayerUpgrade(
            "子彈改良", 
            pair.first,
            FormatString(" - 造成傷害 + %d", pair.second),
            [pair]() {
                auto modifier = new EntityModifier(
                    EntityModifierTypes::None | EntityModifierTypes::IsBuff,
                    EntityModifierKey  ::Atk,
                    EntityModifierOP   ::ADD, pair.second);
                modifierBuffer.push_back(modifier);
                Global::Player->AddModifier(modifier, numeric_limits<float>().max());
            }, 5
        ));
    }

    for (auto& pair : { make_pair(Rarity::Common, 0.10),
                        make_pair(Rarity::Rare  , 0.20),
                        make_pair(Rarity::Epic  , 0.35),
                        make_pair(Rarity::Legend, 0.50) }) {
        PlayerUpgrade::AllUpgrades[pair.first].push_back(new PlayerUpgrade(
            "高供能引擎",
            pair.first,
            FormatString(" - Sp回復 + %.2f", pair.second),
            [pair]() {
                auto modifier = new EntityModifier(
                    EntityModifierTypes::None | EntityModifierTypes::IsBuff,
                    EntityModifierKey  ::RegSp,
                    EntityModifierOP   ::ADD, pair.second);
                modifierBuffer.push_back(modifier);
                Global::Player->AddModifier(modifier, numeric_limits<float>().max());
            }, 5
        ));
    }

    for (auto& pair : { make_pair(Rarity::Common, 0.25),
                        make_pair(Rarity::Rare  , 0.50),
                        make_pair(Rarity::Epic  , 0.75),
                        make_pair(Rarity::Legend, 1.00) }) {
        PlayerUpgrade::AllUpgrades[pair.first].push_back(new PlayerUpgrade(
            "履帶改良",
            pair.first,
            FormatString(" - 移動速度 + %.2f", pair.second),
            [pair]() {
                auto modifier = new EntityModifier(
                    EntityModifierTypes::None | EntityModifierTypes::IsBuff,
                    EntityModifierKey  ::Spd,
                    EntityModifierOP   ::ADD, pair.second);
                modifierBuffer.push_back(modifier);
                Global::Player->AddModifier(modifier, numeric_limits<float>().max());
            }, 5
        ));
    }

    for (auto& pair : { make_pair(Rarity::Epic  , 1),
                        make_pair(Rarity::Legend, 2) }) {
        PlayerUpgrade::AllUpgrades[pair.first].push_back(new PlayerUpgrade(
            "高級子彈陣列",
            pair.first,
            FormatString(" - 子彈發射 + %d\n", pair.second)     +
            FormatString(" - 子彈傷害 - %d%" , pair.second * 10),
            [pair]() {
                auto modifier = new EntityModifier(
                    EntityModifierTypes::None | EntityModifierTypes::IsBuff,
                    EntityModifierKey  ::Atk,
                    EntityModifierOP   ::MUL, pair.second * -0.1f);
                modifierBuffer.push_back(modifier);
                Global::Player->AddModifier(modifier, numeric_limits<float>().max());
                Global::Player->bulletGenerator->WavePerShoot += pair.second;
            }, 1
        ));
    }

    PlayerUpgrade::AllUpgrades[Rarity::Epic].push_back(new PlayerUpgrade(
        "改良彈頭",
        Rarity::Epic,
        " - 子彈貫穿 + 1",
        []() {
            for(auto& info : Global::Player->weapon_bulletInfo)
                info.Penetrate += 1;
        }, 2
    ));

    PlayerUpgrade::AllUpgrades[Rarity::Epic].push_back(new PlayerUpgrade(
        "左側副炮口",
        Rarity::Epic,
        " - 左側子彈 + 1\n - 子彈傷害 - 10%",
        []() {
            auto modifier = new EntityModifier(
                EntityModifierTypes::None | EntityModifierTypes::IsBuff,
                EntityModifierKey  ::Atk,
                EntityModifierOP   ::MUL, -0.1f);
            modifierBuffer.push_back(modifier);
            Global::Player->AddModifier(modifier, numeric_limits<float>().max());

            Global::Player->bulletGenerator->BulletWave.push_back(15);
        }, 1
    ));

    PlayerUpgrade::AllUpgrades[Rarity::Epic].push_back(new PlayerUpgrade(
        "右側副炮口",
        Rarity::Epic,
        " - 右側子彈 + 1\n - 子彈傷害 - 10%",
        []() {
            auto modifier = new EntityModifier(
                EntityModifierTypes::None | EntityModifierTypes::IsBuff,
                EntityModifierKey::Atk,
                EntityModifierOP::MUL, -0.1f);
            modifierBuffer.push_back(modifier);
            Global::Player->AddModifier(modifier, numeric_limits<float>().max());

            Global::Player->bulletGenerator->BulletWave.push_back(-15);
        }, 1
    ));

    PlayerUpgrade::AllUpgrades[Rarity::Legend].push_back(new PlayerUpgrade(
        "後方有炮",
        Rarity::Legend,
        " - 後方子彈 + 1",
        []() {
            Global::Player->bulletGenerator->BulletWave.push_back(180);
        }, 1
    ));
    });
}

