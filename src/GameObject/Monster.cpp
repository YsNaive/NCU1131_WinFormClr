#include "pch.h"
#include "Monster.h"

#include "Drawer.h"
#include "Rand.h"
#include "Exp.h"

Monster::Monster()
{
	render_layer = Layer::Monster;
	tag.Add(Tag::Monster);
	speed = 12;
}

void Monster::Update()
{
	rigidbody.AddForce(Global::Player->position - position, speed * Global::DeltaTime);
}

void Monster::OnCollide(GameObject* other, CollideInfo collideInfo)
{
	if (other->tag.Any(Tag::Monster | Tag::Player)) {
		auto offset = position - other->position;
		auto offset_spd = speed * Global::DeltaTime * 3.0f;
		if (other->tag.Contains(Tag::Player))
			offset_spd *= 2.5f;
		offset.set_length(offset_spd);
		rigidbody.movement += offset;
	}
}

void Monster::OnDead()
{
	for (int i = 0, imax = Rand::RandomInt(1, 3); i < imax; i++) {
		auto obj = new Exp(1);
		obj->position = position;
		obj->rigidbody.AddForce(Rand::RandomFloat(0, 360), Rand::RandomFloat(0.25, 1.5));
	}
	Destroy();
}

NormalMonster::NormalMonster(float size_radius)
{
	collider.AddCircle(Circle({ 0,0 }, size_radius));
}

void NormalMonster::Render()
{
	Drawer::AddFillPoly({ 1,0,0 }, collider.hitboxes[0]);
}
