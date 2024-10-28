#pragma once;
#include "Core.h";
#include "GameObject.h";

class UI : public GameObject
{
public:
    inline UI() { tag.Add(Tag::UI); render_layer = Layer::UI; rigidbody.enable = false; }
};

class UI_ProgressBar : public UI {
private:
    float currentValue;
public:
    inline UI_ProgressBar(float min, float max, Color empty, Color fill)
        : MinValue(min), MaxValue(max), currentValue(min), Value(min), EmptyColor(empty), FillColor(fill) {}

    bool  IsHorizontal = true;
    bool  IsReverse    = false;
    float MinValue;
    float MaxValue;
    float Value;

    Color EmptyColor;
    Color FillColor;
    Rect Bound;

    void Update() override;
    void Render() override;
};