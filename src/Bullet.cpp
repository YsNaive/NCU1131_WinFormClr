#include "pch.h"
#include "Core.h"

Bullet::Bullet(float direction, float speed, float destoryDistance)
	: direction(direction), speed(speed), destoryDistance(destoryDistance)
{
	tag.Add(Tag::Bullet);
	collider.AddCircle(Circle({ 0,0 }, 3.5));
}

void Bullet::Update()
{
	auto theta = DEG2RAD * direction;
	position.x += speed * sin(theta);
	position.y += speed * -cos(theta);
	movedDistance += speed;
	if (movedDistance > destoryDistance)
		this->Destory();
}

void Bullet::Render()
{
	Drawer::AddFillPoly(Color(.9, .4, .4), collider.hitboxes[0]);
}

void Bullet::OnCollide(GameObject* other, CollideInfo collideInfo)
{
	if (find(hit_history.begin(), hit_history.end(), other) != hit_history.end())
		return;
	auto entity = dynamic_cast<Entity*>(other);
	if (!entity)
		return;

	entity->ReciveDamage(damage, this);
	penetrate--;
	if (penetrate < 0)
		Destory();
}
