#include "pch.h"
#include "Player.h"

#include "Global.h"
#include "Drawer.h"
#include "Bullet.h"

Player::Player()
{
	render_layer = Layer::Player;
	tag.Add(Tag::Player);
	position = { 200,200 };
	rotation = 45;
	auto scale = 35.0f;
	collider.AddRect({ -scale / 2.0f, -scale / 2.0f , scale, scale });
	speed = 15;
}

void Player::ReciveExp(int value)
{
	cout << "GET Exp " << value << '\n';
}

void Player::Update()
{
	// movement
	float force = 0;
	if (Global::GetKey(Keys::W)) {
		force += speed;
	}
	if (Global::GetKey(Keys::S)) {
		force -= speed;
	}
	if (Global::GetKey(Keys::A)) {
		rotation -= 150 * Global::DeltaTime;
	}
	if (Global::GetKey(Keys::D)) {
		rotation += 150 * Global::DeltaTime;
	}
	rigidbody.AddForce(rotation, force * Global::DeltaTime);

	if (Global::GetKey(MouseButtons::Left)) {
		auto bullet = new Bullet(rotation, 20);
		auto offset = Vector2::FromDegree(rotation);
		offset.set_length(25);
		bullet->position = this->position + offset;
	}

	// attract exp
	for (auto exp : Collider::FindObject({ position, attractExpRange }, [](GameObject* m) {return m->tag.Contains(Tag::Exp); })) {
		auto attractForce = position - exp->position;
		attractForce.set_length(speed * Global::DeltaTime * 2.0f);
		exp->rigidbody.AddForce(attractForce);
	}
}

void Player::Render()
{
	const float bodySize = 40;
	const float wheelExtend = 5;
	const Rect  body = { -bodySize / 2.0f, -bodySize / 2.0f , bodySize,bodySize };
	const Rect  leftWheel = { body.x - wheelExtend, body.y - wheelExtend , wheelExtend * 2,bodySize + wheelExtend * 2 };
	const Rect  rightWheel = { body.get_xMax() - wheelExtend, body.y - wheelExtend , leftWheel.width, leftWheel.height };
	const Rect  gun = { -wheelExtend / 2.0, -bodySize , wheelExtend, bodySize };

	Drawer::AddFillRect(Color(0, 0, 0), leftWheel);
	Drawer::AddFillRect(Color(0, 0, 0), rightWheel);
	Drawer::AddFillRect(Color(.2, .2, .2), body);
	Drawer::AddFillRect(Color(.4, .4, .4), gun);
}