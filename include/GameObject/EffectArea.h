#pragma once
#include "GameObject.h"
#include "Entity.h"

// �ظm����B�~�K�[������ EntityModifier
// �v�T�d��N�̾� collider �өw
class EffectArea : public GameObject {
public :
    float remainTime = 0;
    vector<pair<EntityModifier*, float>> modifiers;
    Color color;

    EffectArea(float remainTime, Color color = { 0,0,0,0.1f });

    virtual void Update() override;
    virtual void Render() override;
    virtual void OnCollide(GameObject* other, CollideInfo collideInfo) override;
};