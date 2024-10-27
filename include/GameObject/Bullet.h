#pragma once
#include "Core.h"
#include "GameObject.h"

class Bullet : public GameObject {
public:
    Bullet(float direction, float speed, float destoryDistance = 200);
    float direction;
    float speed;
    float destoryDistance;
    float movedDistance = 0;
    float damage = 1;
    float penetrate = 0;
    vector<GameObject*> hit_history;

    virtual void Update() override;
    virtual void Render() override;
    virtual void OnCollide(GameObject* other, CollideInfo collideInfo);
};