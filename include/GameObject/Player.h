#pragma once;
#include "Core.h"
#include "Entity.h"
#include "BulletGenerator.h"
#include "UI.h"

class Player : public Entity {
public:
    int Level = 0;
    float CurrentExp = 0;
    float LevelUpExp = 50;
    float attractExpRange = 70;

    DamageInfo damageInfo;
    BulletInfo bulletInfo;
    BulletGenerator* bulletGenerator = nullptr;

    Player();
    void ReciveExp(int value);
    virtual void Update() override;
    virtual void Render() override;
    virtual void OnDead() override;
};