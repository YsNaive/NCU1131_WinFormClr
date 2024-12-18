#pragma once;
#include "Core.h"
#include "Entity.h"
#include "BulletGenerator.h"
#include "UI.h"

class Player : public Entity {
private:
public:
    int Level = 0;
    float CurrentExp = 0;
    float LevelUpExp = 50;
    float attractExpRange = 70;

    BulletGenerator* bulletGenerator = nullptr;

    DamageInfo          weapon_damageInfo;
    BulletInfo          weapon_bulletInfo;
    function<Bullet*()> weapon_CreateBullet;

    Player();
    void ReciveExp(int value);
    virtual void Update() override;
    virtual void Render() override;
    virtual void OnDead() override;
};