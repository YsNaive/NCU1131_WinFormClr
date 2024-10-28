#include "UI.h"

#include "Drawer.h"
#include "Global.h"

namespace {
    static constexpr float AnimationRate = 10.0f;
    static constexpr float BorderPadding = 2.5f;
}

void UI_ProgressBar::Update()
{
    currentValue += (Value - currentValue) * AnimationRate * Global::DeltaTime;
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
