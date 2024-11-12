#include "PlayerUpgrade.h"
#include "Global.h"
#include "Player.h"

map<int, vector<PlayerUpgrade*>> PlayerUpgrade::AllUpgrades;
map<int, vector<PlayerUpgrade*>> PlayerUpgrade::AvailableUpgrades;
map<PlayerUpgrade*, int>         PlayerUpgrade::AppliedCount;
PlayerUpgrade PlayerUpgrade::None = PlayerUpgrade("None", Rarity::None, "�S�o���", [](){});

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
            }, 5
        ));
    }

    for (auto& pair : { make_pair(Rarity::Common, 0.10),
                        make_pair(Rarity::Rare  , 0.20),
                        make_pair(Rarity::Epic  , 0.35),
                        make_pair(Rarity::Legend, 0.50) }) {
        PlayerUpgrade::AllUpgrades[pair.first].push_back(new PlayerUpgrade(
            "���ѯ����",
            pair.first,
            FormatString(" - Sp�^�_ + %.2f", pair.second),
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
            }, 5
        ));
    }

    for (auto& pair : { make_pair(Rarity::Epic  , 1),
                        make_pair(Rarity::Legend, 2) }) {
        PlayerUpgrade::AllUpgrades[pair.first].push_back(new PlayerUpgrade(
            "���Ťl�u�}�C",
            pair.first,
            FormatString(" - �l�u�o�g + %d\n", pair.second)     +
            FormatString(" - �l�u�ˮ` - %d%" , pair.second * 10),
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
        "��}�u�Y",
        Rarity::Epic,
        " - �l�u�e�� + 1",
        []() {
            for(auto& info : Global::Player->weapon_bulletInfo)
                info.Penetrate += 1;
        }, 2
    ));

    PlayerUpgrade::AllUpgrades[Rarity::Epic].push_back(new PlayerUpgrade(
        "�����Ƭ��f",
        Rarity::Epic,
        " - �����l�u + 1\n - �l�u�ˮ` - 10%",
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
        "�k���Ƭ��f",
        Rarity::Epic,
        " - �k���l�u + 1\n - �l�u�ˮ` - 10%",
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
        "��観��",
        Rarity::Legend,
        " - ���l�u + 1",
        []() {
            Global::Player->bulletGenerator->BulletWave.push_back(180);
        }, 1
    ));
    });
}

