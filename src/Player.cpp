#include "pch.h"
#include "Core.h"

Player* Player::instance;
Player::Player()
{
	instance = this;
	tag.Add(Tag_Player);
	position = { 200,200 };
	rotation = 45;
	auto scale = 35.0f;
	collider.boxes.push_back({ -scale / 2.0f, -scale / 2.0f , scale, scale });
	speed = 0.15;
}

void Player::Update()
{
	float force = 0;
	if (Input::GetKey(Keys::W)) {
		force += speed;
	}
	if (Input::GetKey(Keys::S)) {
		force -= speed;
	}
	if (Input::GetKey(Keys::A)) {
		rotation -= 3;
	}
	if (Input::GetKey(Keys::D)) {
		rotation += 3;
	}
	rigidbody.AddForce(rotation, force);

	if (Input::GetKey(MouseButtons::Left)) {
		auto bullet = new Bullet(rotation, 8);
		bullet->position = this->position;
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
