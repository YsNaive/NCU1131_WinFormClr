#include "BulletGenerator.h"

void BulletGenerator::Update() {
    position = Shooter->position;
    rotation = Shooter->rotation;
    auto shoot_cd = 1.0f / Shooter->entityInfo.AtkSpd;
    while (cooldown_shoot < 0) {
        cooldown_shoot += shoot_cd;
        remainWaves += WavePerShoot;
    }
    while (cooldown_wave < 0 && remainWaves > 0) {
        cooldown_wave += shoot_cd / (2.0f * WavePerShoot);
        remainWaves--;
        for (auto dir : BulletWave) {
            auto* bullet = CreateBullet();
            auto rngDir = dir + rotation + Rand::Float(-Shooter->entityInfo.DivDeg, Shooter->entityInfo.DivDeg);

            bullet->rotation = rngDir;
            bullet->position = position;
        }
    }
    cooldown_shoot -= Global::DeltaTime;
    if (remainWaves > 0)
        cooldown_wave -= Global::DeltaTime;
}