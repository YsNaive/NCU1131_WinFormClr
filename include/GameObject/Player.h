#pragma once;
#include "Core.h"
#include "Entity.h"
#include "BulletGenerator.h"
#include "UI.h"


class OnLevelUp : public GlobalEvent<OnLevelUp, void> {};

class Player : public Entity {
private:
    static constexpr int weapon_count = 2;

    int using_weapon = 0;
public:
    int Level = 0;
    float CurrentExp = 0;
    float LevelUpExp = 50;
    float attractExpRange = 70;
    float bullet_spd_mul = 1.0f;

    BulletGenerator* bulletGenerator = nullptr;

    DamageInfo          weapon_damageInfo   [weapon_count];
    BulletInfo          weapon_bulletInfo   [weapon_count];
    int                 weapon_SpPerSec     [weapon_count];
    function<Bullet*()> weapon_CreateBullet [weapon_count];

    Player();
    void ReciveExp(int value);
    void SetUsingWeapon(int index);
    virtual void Update() override;
    virtual void Render() override;
    virtual void OnDead() override;
};