#pragma once
#include "Core.h"
#include "GameObject.h"
#include "DamageInfo.h"

class Bullet : public GameObject {
public:
    inline Bullet() : Bullet(0, nullptr) {};
    Bullet(float speed, DamageInfo* damageInfo, float destoryDistance = 500);
    float speed           = 0;
    float destoryDistance = 1000;
    float movedDistance   = 0;
    float penetrate       = 0;
    vector<GameObject*> hit_history;

    DamageInfo* damageInfo;

    virtual void Update() override;
    virtual void Render() override;
    virtual void OnCollide(GameObject* other, CollideInfo collideInfo);
};