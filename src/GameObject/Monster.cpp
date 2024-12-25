
#include "Monster.h"

#include "Drawer.h"
#include "Rand.h"
#include "Exp.h"
#include "Player.h"
#include "Resources.h"

Monster::Monster()
{
	render_layer = Layer::Monster;
	tag.Add(Tag::Monster);

	entityInfo_origin.Spd   = 10;
	entityInfo_origin.MaxHp = 10;
	entityInfo_origin.Atk   = 5;
	Hp = 10;
}

void Monster::Update()
{
	Entity::Update();
	auto dir = Global::Player->position - position;
	rotation = Vector2::ToDegree(dir);
	rigidbody.AddForce(dir, entityInfo.Spd * Global::DeltaTime);
}

void Monster::OnCollide(GameObject* other, CollideInfo collideInfo)
{
	if (other->tag.Any(Tag::Monster | Tag::Player)) {
		auto offset = position - other->position;
		auto offset_spd = entityInfo.Spd * Global::DeltaTime;
		if (other->tag.Contains(Tag::Player)) {
			auto player = (Entity*)other;
			offset_spd *= 160.0f;
			DamageInfo damage = DamageInfo::FromEntity(this);
			damage.Hit(player);
		}
		offset.set_length(offset_spd);
		rigidbody.movement += offset;
		position += offset * 0.1f;
	}
}

void Monster::OnHit(const DamageInfo& info)
{
	lastHitTime = Global::Time;
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

void Monster::Render() {
	float hit_dt = Global::Time - lastHitTime;
	const Rect hp_bar_bound = { -15,-25,30,7.5 };
	if (hit_dt < 1.5f) {
		Drawer::AddRotation(-rotation);
		Color color = { 0.25,0.15,0.15, 1.0f };
		if (hit_dt > 2.5)
			color.a = 1.0f - ((hit_dt - 2.5f) / 0.5f);
		Drawer::AddFillRect(color, hp_bar_bound);
		Color color2 = { 0.8f, 0.4f, 0.4f, color.a };
		Rect hp_bar = hp_bar_bound;
		hp_bar.width *= Hp / entityInfo.MaxHp;
		Drawer::AddFillRect(color2, hp_bar);
		Drawer::AddRect(color, hp_bar_bound);
		Drawer::AddRotation(rotation);
	}
}

NormalMonster::NormalMonster(float size_radius)
{
	float width = size_radius;
	collider.hitboxes.push_back(Polygon2D({
		{-width,width},
		{width,width},
		{0,-width},
		}));
}

void NormalMonster::Render()
{
	Monster::Render();
	Drawer::AddImage(RefResources::NormalMonster, collider.hitboxes[0].get_boundingBox());

}

ShooterMonster::ShooterMonster(float size_radius)
{
	collider.AddCircle(Circle({ 0,0 }, size_radius));
	entityInfo_origin.Spd *= 1.25f;
}

void ShooterMonster::Update()
{
	Entity::Update();
	auto dir = Global::Player->position - position;
	rotation = Vector2::ToDegree(dir);
	auto distance = dir.get_length();
	float f = entityInfo.Spd * Global::DeltaTime;
	float hold_distance = 250;
	f *= min(fabs(distance - hold_distance), 100.0f) / 100;
	if (distance < hold_distance)
		dir *= -1;
	rigidbody.AddForce(dir, f);

	dir = { 0,0 };
	for (auto* obj : Collider::FindObject(Circle(position, hold_distance*2))) {
		if (!obj->tag.Contains(Tag::Monster))
			continue;
		auto* other = (Monster*)obj;
		dir += (position - other->position);
	}
	rigidbody.AddForce(dir, entityInfo.Spd * 0.5 * Global::DeltaTime);
}

void ShooterMonster::Render()
{
	Monster::Render();
	Drawer::AddImage(RefResources::ShooterMonster, collider.hitboxes[0].get_boundingBox());
}
