#pragma once
#include "Core.h"
#include "GameObject.h"
#include "DamageInfo.h"

class Entity;

// EntityModifier 算式
// Type 帶有 Unique 的話同類效果取最高
// Op   使用哪種算式取值
// Key  對於哪種屬性作操作
class EntityModifierTypes {
public:
    const static int None   = 0;
    const static int IsUnique = 1;
    const static int IsBuff = 2;

    const static int Fire   = 4;
    const static int Frost  = 8;
};
class EntityModifierOP {
public:
    const static int None = 0;
    const static int ADD  = 1;
    const static int MUL  = 2;
};
class EntityModifierKey {
public:
    const static int None = 0;

    const static int MaxHp  = (1 << 0);
    const static int Spd    = (1 << 1);
    const static int Def    = (1 << 2);
    const static int Atk    = (1 << 3);
    const static int Res_M  = (1 << 4);
    const static int Atk_M  = (1 << 5);
    const static int Res_E  = (1 << 6);
    const static int AtkSpd = (1 << 7);
    const static int DivDeg = (1 << 8);
    const static int CDR    = (1 << 9);
    const static int MaxSp  = (1 << 10);
    const static int RegSp  = (1 << 11);
    const static int RegHp  = (1 << 12);
};

class EntityInfo {
public:
    float MaxHp  = 0;
    float MaxSp  = 0;
    float RegHp = 0.0;
    float RegSp = 0.0;
    float Spd    = 0;
    float Def    = 0;
    float Atk    = 0;
    float Res_M  = 0;
    float Atk_M  = 0;
    float Res_E  = 0;
    float AtkSpd = 1.0f;
    float DivDeg = 10.0f;
    float CDR    = 0.0f;
    float& operator[] (int key);
    void MakeValid();
};

class EntityModifier {
public:
    inline EntityModifier(int types, int key, int op, float value)
        : Types(types), Key(key), OP(op), value(value) {}

    int Types = EntityModifierTypes::None;
    int Key   = EntityModifierKey  ::None;
    int OP    = EntityModifierOP   ::None;
    float value = 0;

    inline int GetPureType() { return (Types & (~(EntityModifierTypes::IsUnique | EntityModifierTypes::IsBuff))); };

    inline int IsBuff()      { return ((Types & EntityModifierTypes::IsBuff)   == EntityModifierTypes::IsBuff);   };
    inline int IsUnique()    { return ((Types & EntityModifierTypes::IsUnique) == EntityModifierTypes::IsUnique); };
};

class Entity : public GameObject {
public:
    Entity();

    float Hp = 0;
    float Sp = 0;
    float Mp = 0;
    bool  isDead = false;
    EntityInfo entityInfo;
    EntityInfo entityInfo_origin;
    unordered_map<EntityModifier*, float> entityModifiers;

    virtual void Update() override;

    void AddModifier   (EntityModifier* modifier, float addTime);
    void SetModifier   (EntityModifier* modifier, float setTime);
    void RemoveModifier(EntityModifier* modifier);

    inline virtual void OnDead() {}
    inline virtual void OnHit(const DamageInfo& info) {}

};