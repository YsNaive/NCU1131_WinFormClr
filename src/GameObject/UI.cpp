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
    Drawer::AddText(Color(.1,.1,.1), text, position, anchor);
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

