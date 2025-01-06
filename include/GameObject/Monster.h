#pragma once
#include "Core.h"
#include "Entity.h"
#include "Bullet.h"
#include "EffectArea.h"

class Monster : public Entity {
protected:
    float lastHp = 0;
    float lastHpChangedTime = -999;
public:
    Monster();
    virtual void Update() override;
    virtual void OnCollide(GameObject* other, CollideInfo collideInfo) override;
    virtual void OnDead() override;
    virtual void Render() override;

    void RandomLocateOnPlayer();
};

class NormalMonster : public Monster {
public:
    NormalMonster(float size_radius = 15);
    virtual void Update() override;
    virtual void Render() override;
};

class ShooterMonster : public Monster {
    float lastShootTime = 0;
public:
    static DamageInfo DamageInfo;
    static BulletInfo BulletInfo;

    ShooterMonster(float size_radius = 15);
    virtual void Update() override;
    virtual void Render() override;
};

class HealMonster : public Monster {
public:
    static EntityModifier Modifier;

    HealMonster(float size_radius = 15);

    EffectArea* effectArea;

    virtual void Update() override;
    virtual void Render() override;
    virtual void OnDead() override;
};