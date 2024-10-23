#include "pch.h"
#include "Core.h"

// test code
auto monster_test = LateUpdate::Create([]() {
	int count = 0;
	for (auto obj : GameObject::GetInstances()) {
		if (obj->tag.Contains(Tag_Monster))
			count++;
	}
	while (count < 10) {
		auto obj = new NormalMonster(Input::RandomInt(12, 19));
		auto offset = Vector2::FromDegree(Input::RandomFloat(0, 360));
		offset.set_length(max(Input::ScreenSize.x, Input::ScreenSize.y));
		obj->position = Player::instance->position + offset;
		count++;
	}
	});

Monster::Monster()
{
	render_layer = Layer_Monster;
	tag.Add(Tag_Monster);
	speed = 0.15;
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

void Monster::OnDead()
{
	for (int i = 0, imax = Input::RandomInt(1, 4); i < imax; i++) {
		auto obj = new Exp(1);
		obj->position = position;
		obj->rigidbody.AddForce(Input::RandomFloat(0, 360), 1.5);
	}
	Destory();
}

NormalMonster::NormalMonster(float size_radius)
{
	collider.circles.push_back(Circle({ 0,0 }, size_radius));
}

void NormalMonster::Render()
{
	Drawer::AddFillCircle({ 1,0,0 }, collider.circles[0]);
}
