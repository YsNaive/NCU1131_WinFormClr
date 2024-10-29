#include "DamageInfo.h"
#include "Entity.h"

DamageInfo DamageInfo::FromEntity(Entity* entity)
{
    auto ret = DamageInfo();
    ret.Sender   = entity;
    ret.Damage   = entity->entityInfo.Atk;
    ret.Damage_M = entity->entityInfo.Atk_M;
    return ret;
}

void DamageInfo::Hit(Entity* target) const
{
    if (target->Hp == numeric_limits<float>().min())
        return;
    float totalDamage = 0.0f;
    totalDamage += max(0.0f, Damage - target->entityInfo.Def);
    totalDamage += Damage_M * (1.0f - target->entityInfo.Res_M);
    totalDamage += Damage_E * (1.0f - target->entityInfo.Res_E);
    target->Hp -= totalDamage;
    target->OnHit(*this);

    if (target->Hp < 0) {
        target->Hp = numeric_limits<float>().min();
        target->OnDead();
    }
}
