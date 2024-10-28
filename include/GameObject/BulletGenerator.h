#pragma once;
#include "Core.h"
#include "Global.h"
#include "GameObject.h"
#include "Entity.h"
#include "Bullet.h"
#include "Rand.h"

template<class T>
class BulletGenerator : public GameObject {
protected:
    int remainWaves = 0;
    float cooldown_wave  = 0;
    float cooldown_shoot = 0;
    DamageInfo damageInfo;
public:
    inline BulletGenerator(Entity* shooter)
        : Shooter(shooter) {}

    Entity* Shooter;
    vector<float> BulletWave;
    int WavePerShoot   = 1;
    int BulletSpeed    = 500;
    float OffsetRadius = 25;

    void Update() override {
        position = Shooter->position;
        rotation = Shooter->rotation;
        damageInfo = DamageInfo::FromEntity(Shooter);
        auto shoot_cd = 1.0f / Shooter->entityInfo.AtkSpd;
        while (cooldown_shoot < 0) {
            cooldown_shoot += shoot_cd;
            remainWaves += WavePerShoot;
        }
        while (cooldown_wave < 0 && remainWaves > 0) {
            cooldown_wave += shoot_cd /(2.0f * WavePerShoot);
            remainWaves--;
            for (auto dir : BulletWave) {
                auto* bullet = new T();
                auto rngDir = dir + rotation + Rand::RandomFloat(-Shooter->entityInfo.DivDeg, Shooter->entityInfo.DivDeg);
                Vector2 offset = Vector2::FromDegree(rngDir);
                offset.set_length(OffsetRadius);

                bullet->rotation   = rngDir;
                bullet->position   = position + offset;
                bullet->damageInfo = &damageInfo;
                bullet->speed      = BulletSpeed;
            }
        }
        cooldown_shoot -= Global::DeltaTime;
        if(remainWaves > 0)
            cooldown_wave  -= Global::DeltaTime;
    }
};