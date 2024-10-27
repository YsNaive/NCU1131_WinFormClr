#pragma once;
#include "Core.h"
#include "Entity.h"

class Player : public Entity {
public:
    float attractExpRange = 70;

    Player();
    void ReciveExp(int value);
    virtual void Update() override;
    virtual void Render() override;
};