#pragma once
#include "Core.h"
#include "Entity.h"

class Monster : public Entity {
public:
    Monster();
    virtual void Update() override;
    virtual void OnCollide(GameObject* other, CollideInfo collideInfo) override;
    virtual void OnDead() override;
};

class NormalMonster : public Monster {
public:
    NormalMonster(float size_radius = 15);
    void Render() override;
};