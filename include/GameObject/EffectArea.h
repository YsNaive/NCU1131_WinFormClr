#pragma once
#include "GameObject.h"
#include "Entity.h"

// 建置後需額外添加相應的 EntityModifier
// 影響範圍將依據 collider 而定
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