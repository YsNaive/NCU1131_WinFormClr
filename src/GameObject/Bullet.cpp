#include "Bullet.h"

#include "Drawer.h"
#include "Entity.h"

Bullet::Bullet(float speed, DamageInfo* damageInfo, float destoryDistance)
	: speed(speed), damageInfo(damageInfo), destoryDistance(destoryDistance)
{
	tag.Add(Tag::Bullet);
	collider.AddRect({ -3,-5,6,10 });
}

void Bullet::Update()
{
	auto offset = Vector2::FromDegree(rotation);
	offset.set_length(speed * Global::DeltaTime);
	position += offset;
	movedDistance += speed * Global::DeltaTime;
	if (movedDistance > destoryDistance)
		this->Destroy();
}

void Bullet::Render()
{
	for (auto& poly : collider.hitboxes) {
		Drawer::AddFillPoly(Color(.9, .4, .4), poly);
		Drawer::AddPoly(Color(.6, .4, .4), poly);
	}
}

void Bullet::OnCollide(GameObject* other, CollideInfo collideInfo)
{
	if (find(hit_history.begin(), hit_history.end(), other) != hit_history.end())
		return;
	auto entity = dynamic_cast<Entity*>(other);
	if (!entity)
		return;

	damageInfo->Hit(entity);
	penetrate--;
	if (penetrate < 0)
		this->Destroy();
}
