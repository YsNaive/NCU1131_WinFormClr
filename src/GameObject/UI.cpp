#include "UI.h"

#include "Drawer.h"
#include "Global.h"
#include "GameManager.h"

namespace {
    static constexpr float AnimationRate = 10.0f;
    static constexpr float BorderPadding = 2.5f;

}

void UI_Text::Render()
{
    if(size == -1)
        Drawer::AddText(color, text, { 0,0 }, anchor);
    else
        Drawer::AddText(color, text, { 0,0 }, size, anchor);
}

void Fade_Text::InitInWorld(Vector2 pos)
{
    is_init = true;
    tag.Remove(Tag::UI);
    position = pos;
    start_time = Global::Time;
}

void Fade_Text::Render()
{
    auto tempColor = color;
    float dt = Global::Time - start_time;
    if (dt > beginFadeTime) {
        dt -= beginFadeTime;
        float r = dt / fadeTime;
        if (r > 1.0f) {
            Destroy();
            tempColor.a = 0;
        }
        tempColor.a = color.a * (1.0f - r);
    }
    Drawer::AddText(tempColor, text, { 0,0 }, anchor);
}

void UI_ProgressBar::Update()
{
    currentValue += (Value - currentValue) * AnimationRate * Global::RealDeltaTime;
    currentValue = clamp(MinValue, MaxValue, currentValue);
}

void UI_ProgressBar::Render()
{
    float fillRate = (currentValue - MinValue) / (MaxValue - MinValue);
    Rect fillRect = Bound;
    if (IsHorizontal) {
        auto offset = fillRate * Bound.width;
        if (IsReverse)
            fillRect.set_xMin(fillRect.get_xMax() - offset);
        else
            fillRect.set_xMax(fillRect.get_xMin() + offset);
    }
    else
    {
        auto offset = fillRate * Bound.height;
        if (IsReverse)
            fillRect.set_yMin(fillRect.get_yMax() - offset);
        else
            fillRect.set_yMax(fillRect.get_yMin() + offset);
    }

    auto center = fillRect.get_center();
    fillRect.width  -= BorderPadding * 2.0f;
    fillRect.height -= BorderPadding * 2.0f;
    fillRect.set_center(center);

    Drawer::AddFillRect(EmptyColor, Bound);
    Drawer::AddFillRect(FillColor , fillRect);
}


void UI_Button::SetBound(const Rect& bound)
{
    this->bound = bound;
    collider.hitboxes.clear();
    collider.AddRect(bound);
}

void UI_Button::Render()
{

    Drawer::AddFillRect(Color::FromHSV(color_h, color_s, 0.25), bound);
    Drawer::AddRect(Color::FromHSV(color_h, color_s, 0.15), bound, 2.5);

    Drawer::AddText(Color::FromHSV(color_h, color_s, 0.85), label, bound.get_center(), 12, Anchor::MiddleCenter);
}
