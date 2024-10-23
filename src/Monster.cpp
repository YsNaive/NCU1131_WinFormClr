#include "pch.h"
#include "Core.h"

Monster::Monster()
{
	tag.Add(Tag_Monster);
	speed = 0.1;
}

void Monster::Update()
{
	rigidbody.AddForce(Player::instance->position - position, speed);
}

void Monster::OnCollide(GameObject* other, CollideInfo collideInfo)
{
	if (!collideInfo.is_collide)
		return;
	if(other->tag.Contains(Tag_Monster))
		rigidbody.AddForce(position - other->position, speed * 2.5f);
	if(other->tag.Contains(Tag_Player))
		rigidbody.AddForce(position - other->position, speed * 4.5f);
}

NormalMonster::NormalMonster()
{
	collider.circles.push_back(Circle({ 0,0 }, 15));
}

void NormalMonster::Render()
{
	Drawer::AddFillCircle({ 1,0,0 }, collider.circles[0]);
}
