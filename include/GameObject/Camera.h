#pragma once;
#include "GameObject.h"

class Camera : public GameObject {
public:
    float scale = 1;
    float targetScale = 1;
    Vector2 targetPosition = { 0,0 };

    Vector2 WorldToScreen(const Vector2& pos);
    Vector2 ScreenToWorld(const Vector2& pos);
    Rect    WorldToScreen(const Rect& rect);
    Rect    ScreenToWorld(const Rect& rect);

    void Update() override;
};