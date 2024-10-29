
#include "Monster.h"

#include "Drawer.h"
#include "Rand.h"
#include "Exp.h"
#include "Player.h"

Monster::Monster()
{
	render_layer = Layer::Monster;
	tag.Add(Tag::Monster);

	entityInfo_origin.Spd   = 12;
	entityInfo_origin.MaxHp = 25;
	Hp = 25;
}

void Monster::Update()
{
	Entity::Update();
	rigidbody.AddForce(Global::Player->position - position, entityInfo.Spd * Global::DeltaTime);
}

void Monster::OnCollide(GameObject* other, CollideInfo collideInfo)
{
	if (other->tag.Any(Tag::Monster | Tag::Player)) {
		auto offset = position - other->position;
		auto offset_spd = entityInfo.Spd * Global::DeltaTime;
		if (other->tag.Contains(Tag::Player)) {
			auto entity = (Entity*)other;
			offset_spd *= 160.0f;
		}
		offset.set_length(offset_spd);
		rigidbody.movement += offset;
	}
}

void Monster::OnDead()
{
	for (int i = 0, imax = Rand::Int(1, 3); i < imax; i++) {
		auto obj = new Exp(1);
		obj->position = position;
		obj->rigidbody.AddForce(Rand::Float(0, 360), Rand::Float(0.25, 1.5));
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
