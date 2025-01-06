#include "EffectArea.h"

#include "Drawer.h"

EffectArea::EffectArea(float remainTime, Color color)
    :remainTime(remainTime), color(color)
{
    tag.Add(Tag::Bullet);
}

void EffectArea::Update()
{
    remainTime -= Global::DeltaTime;
    if (remainTime <= 0)
        Destroy();
}

void EffectArea::Render()
{
    for (auto& poly : collider.hitboxes)
        Drawer::AddFillPoly(color, poly);
}

void EffectArea::OnCollide(GameObject* other, CollideInfo collideInfo)
{
    if (!other->tag.Contains(Tag::Entity))
        return;
    if (!other->tag.Any(this->tagFilter))
        return;

    auto entity = (Entity*)other;
    for (auto& pair : modifiers)
        entity->SetModifier(pair.first, pair.second);
}
