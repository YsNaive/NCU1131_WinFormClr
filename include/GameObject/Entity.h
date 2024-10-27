#pragma once
#include "Core.h"
#include "GameObject.h"

class Entity : public GameObject {
public:
    Entity();
    float max_hp;
    float min_hp;
    float hp;
    float defence;
    float speed;

    void ReciveDamage(float value, GameObject* sender);
    inline void SendDamage(float value, Entity* reciver) { reciver->ReciveDamage(value, this); };
    inline virtual void OnDead() {}
    inline virtual void OnHurt() {}
};