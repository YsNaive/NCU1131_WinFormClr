#pragma once
#include "Core.h"
#include "GameObject.h"
extern class Entity;

class DamageInfo {
public:
    static DamageInfo FromEntity(Entity* entity);

    GameObject* Sender = nullptr;

    float Damage   = 0;
    float Damage_M = 0;
    float Damage_E = 0;

    void Hit(Entity* target);
};