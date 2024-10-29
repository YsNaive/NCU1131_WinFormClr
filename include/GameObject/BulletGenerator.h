#pragma once;
#include "Core.h"
#include "Global.h"
#include "GameObject.h"
#include "Entity.h"
#include "Bullet.h"
#include "Rand.h"

class BulletGenerator : public GameObject {
protected:
    int   remainWaves    = 0;
    float cooldown_wave  = 0;
    float cooldown_shoot = 0;
public:
    inline BulletGenerator(Entity* shooter, function<Bullet*()> createBullet)
        : Shooter(shooter), CreateBullet(createBullet) {}

    Entity* Shooter;
    function<Bullet* ()> CreateBullet;
    vector<float> BulletWave;
    int WavePerShoot = 1;

    void Update() override;
};