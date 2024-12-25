#pragma once
#include "Core.h"
#include "Entity.h"

class Monster : public Entity {
protected:
        float lastHitTime = -999;
public:
    Monster();
    virtual void Update() override;
    virtual void OnCollide(GameObject* other, CollideInfo collideInfo) override;
    virtual void OnDead() override;
    virtual void Render() override;
    virtual void OnHit(const DamageInfo& info) override;
};

class NormalMonster : public Monster {
public:
    NormalMonster(float size_radius = 15);
    virtual void Render() override;
};

class ShooterMonster : public Monster {
public:
    ShooterMonster(float size_radius = 15);
    virtual void Update() override;
    virtual void Render() override;
};