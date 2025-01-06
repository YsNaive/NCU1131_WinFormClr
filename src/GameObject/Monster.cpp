
#include "Monster.h"

#include "Drawer.h"
#include "Rand.h"
#include "Exp.h"
#include "Player.h"
#include "Resources.h"

namespace {
	auto monster_init =
	Start::Create([]() {
		ShooterMonster::BulletInfo.Penetrate = 0;
		ShooterMonster::BulletInfo.Speed     = 100;
		ShooterMonster::DamageInfo.Damage    = 15;
		ShooterMonster::DamageInfo.Damage_E  = 0;
		ShooterMonster::DamageInfo.Damage_M  = 0;
	});
}

BulletInfo ShooterMonster::BulletInfo = BulletInfo::DefaultMonster;
DamageInfo ShooterMonster::DamageInfo;

EntityModifier HealMonster::Modifier = EntityModifier(
	EntityModifierTypes::None,
	EntityModifierKey  ::RegHp,
	EntityModifierOP   ::ADD,
	5
);

Monster::Monster()
{
	render_layer = Layer::Monster;
	tag.Add(Tag::Monster);

	entityInfo_origin.Spd   = 6;
	entityInfo_origin.MaxHp = 15;
	entityInfo_origin.Atk   = 10;
	Hp = 25;
}

void Monster::Update()
{
	Entity::Update();
	if (Hp != lastHp) {
		lastHpChangedTime = Global::Time;
		lastHp = Hp;
	}
	if ((position - Global::Player->position).get_length() > 600) {
		this->RandomLocateOnPlayer();
	}
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
	float hit_dt = Global::Time - lastHpChangedTime;
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

void Monster::RandomLocateOnPlayer()
{
	this->position = Global::Player->position;
	Vector2 offset = Vector2::FromDegree(Rand::Float(-180, 180));
	offset.set_length(500);
	this->position += offset;
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

void NormalMonster::Update()
{
	Monster::Update();
	auto dir = Global::Player->position - position;
	rotation = Vector2::ToDegree(dir);
	rigidbody.AddForce(dir, entityInfo.Spd * Global::DeltaTime);
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
	Monster::Update();
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

	if ((Global::Time - lastShootTime) > 2.0f) {
		lastShootTime = Global::Time;
		auto bullet = new Bullet(&ShooterMonster::BulletInfo, &ShooterMonster::DamageInfo);
		bullet->collider.AddRect({ -3,-5,6,10 });
		bullet->color_h = 0;
		bullet->position = this->position;
		bullet->rotation = this->rotation;
	}

}

void ShooterMonster::Render()
{
	Monster::Render();
	Drawer::AddImage(RefResources::ShooterMonster, collider.hitboxes[0].get_boundingBox());
}

HealMonster::HealMonster(float size_radius)
{
	float width = size_radius;
	collider.AddRect({ -size_radius,-size_radius, size_radius * 2,size_radius * 2 });
	effectArea = new EffectArea(numeric_limits<float>().max(), Color(1, 0.35, 0, 0.075f));
	effectArea->tagFilter = Tag::Monster;
	effectArea->modifiers.push_back({ &HealMonster::Modifier , 0.5f});
	effectArea->collider.AddCircle({ {0,0}, 100 });
	effectArea->render_layer = Layer::Effect;
}

void HealMonster::Update()
{
	Monster::Update();

	Vector2 dir = { 0,0 };
	for (auto* obj : GameObject::GetInstances(Tag::Monster)) {
		auto* other = (Monster*)obj;
		dir += (other->position - this->position);
	}
	this->rotation += 90 * Global::DeltaTime;
	rigidbody.AddForce(dir, entityInfo.Spd * Global::DeltaTime);
	effectArea->position = this->position;
}

void HealMonster::Render()
{
	Monster::Render();
	Drawer::AddImage(RefResources::HealMonster, collider.hitboxes[0].get_boundingBox());
}

void HealMonster::OnDead()
{
	Monster::OnDead();
	effectArea->Destroy();
}