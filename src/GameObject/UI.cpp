#include "UI.h"

#include "Drawer.h"
#include "Global.h"
#include "PlayerUpgrade.h"

namespace {
    static constexpr float AnimationRate = 10.0f;
    static constexpr float BorderPadding = 2.5f;
}

void UI_Text::Render()
{
    Drawer::AddText(color, text, { 0,0 }, anchor);
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

void UI_Card::Render()
{
    auto waveFunc = [](float x) { return ((sin(x * 180 * DEG2RAD) + 1) / 2.0);  };
    float c1sv = waveFunc(Global::RealTime      ) * 0.5f + 0.25f;
    float c2sv = waveFunc(Global::RealTime + 0.5) * 0.5f + 0.25f;
    Color color1 = Color::FromHSV(hue, c1sv, c1sv);
    Color color2 = Color::FromHSV(hue, c2sv, c2sv);

    Rect bound = { {0,0}, size };
    Drawer::SetBrush(color1, color2, { 0,0 }, { 0,size.y });
    Drawer::AddFillRect(bound);
    Vector2 vec2pad = { padding, padding };
    bound = { vec2pad, size - vec2pad * 2.0f };

    Color bgColor   = Color::FromHSV(hue, .45, .15);
    Color textColor = Color::FromHSV(hue, 0.25, 0.9);
    Drawer::AddFillRect(bgColor, bound);
    if (IsHovering)
        Drawer::AddRect(textColor, { vec2pad * 0.5f, size - vec2pad }, 2.0f);

    Rect rect_text = bound;
    rect_text.height *= labelHeightPercent;

    Drawer::AddText(textColor, label, rect_text.get_center(), 16, Anchor::MiddleCenter);
    Drawer::AddText(textColor, description, { bound.x, rect_text.get_yMax() }, 12);
}

void UI_Card::AssignPlayerUpgrade(PlayerUpgrade& info)
{
    label       = info.Name;
    description = info.Description;
    OnClick.push_back([&]() {info.ApplyOnPlayer(); });
    hue = Rarity::GetColorHue(info.Rarity);
}

void UI_Button::Render()
{
    Drawer::AddFillRect(Color(.25, .25, .25), bound);
    Drawer::AddRect    (Color(.15, .15, .15), bound, 2.5);

    Drawer::AddText(Color(.85, .85, .85), label, bound.get_center(), 12, Anchor::MiddleCenter);
}
