#pragma once
#include "Core.h"
#include "GameObject.h"

class Exp : public GameObject {
public:
    Exp(float value);
    float value;

    virtual void OnCollide(GameObject* other, CollideInfo collideInfo);
    virtual void Render() override;
};