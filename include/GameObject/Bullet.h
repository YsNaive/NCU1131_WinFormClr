#pragma once
#include "Core.h"
#include "GameObject.h"
#include "DamageInfo.h"

class BulletInfo {
public:
    static const BulletInfo DefaultPlayer;
    static const BulletInfo DefaultMonster;
    inline BulletInfo() : BulletInfo(0) {}
    inline BulletInfo(float speed, int penetrate = 0, int destroyDistance = 1000, int ignoreTag = 0)
        : Speed(speed), Penetrate(penetrate), DestroyDistance(destroyDistance), IgnoreTag(ignoreTag) {}

    float Speed   = 0;
    int Penetrate = 0;
    int DestroyDistance = 0;
    int IgnoreTag = 0;
    float DestroyTimeSec = 600;
};

class Bullet : public GameObject {
public:
    inline Bullet(const BulletInfo* bulletInfo, const DamageInfo* damageInfo)
        : bulletInfo(bulletInfo), damageInfo(damageInfo) {
        tag.Add(Tag::Bullet);
    };

    float movedDistance = 0;
    float aliveTime = 0;
    vector<GameObject*> hit_history;

    const BulletInfo* bulletInfo = nullptr;
    const DamageInfo* damageInfo = nullptr;

    virtual void Update() override;
    virtual void Render() override;
    virtual void OnCollide(GameObject* other, CollideInfo collideInfo);
};
