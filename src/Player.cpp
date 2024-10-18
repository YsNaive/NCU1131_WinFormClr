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
}

void Player::Update()
{
	curSpeed *= 0.9;
	if (curSpeed < 0.1 && curSpeed > -0.1)
		curSpeed = 0;

	if (Input::GetKey(Keys::W)) {
		curSpeed += 0.25;
	}
	if (Input::GetKey(Keys::S)) {
		curSpeed -= 0.25;
	}
	if (Input::GetKey(Keys::A)) {
		rotation -= 3;
	}
	if (Input::GetKey(Keys::D)) {
		rotation += 3;
	}
	curSpeed = clamp(-maxSpeed, maxSpeed, curSpeed);

	if (curSpeed != 0) {
		auto rad = rotation * DEG2RAD;
		Vector2 offset = { (float)sin(rad), -(float)cos(rad) };
		offset *= curSpeed;
		position += offset;
	}

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
