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
	if (other->tag.Contains(Tag_Monster) || other->tag.Contains(Tag_Player)) {
		// reflect movement
		Vector2 v = rigidbody.movement;
		auto surface = collideInfo.hitLine;
		Vector2 n = Vector2(surface.p2.y - surface.p1.y, -(surface.p2.x - surface.p1.x));
		n = n.normalize();
		Vector2 r = v - n * (v.dot(n));
		rigidbody.movement = r;

		// collide out
		auto hitCoord = Graph2D::get_closest(collideInfo.hitPoint, collideInfo.hitLine);
		//auto offset = collideInfo.hitPoint - hitCoord;
		//position += offset;
		auto dir = (collideInfo.hitPoint - hitCoord);
		if ((collideInfo.hitPoint - position).get_length() > (hitCoord - position).get_length())
			dir = -dir;
		position += dir;
	}
}

void Monster::OnDead()
{
	for (int i = 0, imax = Input::RandomInt(1, 3); i < imax; i++) {
		auto obj = new Exp(1);
		obj->position = position;
		obj->rigidbody.AddForce(Input::RandomFloat(0, 360), Input::RandomFloat(0.25, 1.5));
	}
	Destory();
}

NormalMonster::NormalMonster(float size_radius)
{
	collider.AddCircle(Circle({ 0,0 }, size_radius));
}

void NormalMonster::Render()
{
	Drawer::AddFillPoly({ 1,0,0 }, collider.hitboxes[0]);
}
