#include "pch.h"
#include "Core.h"

Player* Player::instance;
Player::Player()
{
	instance = this;
	render_layer = Layer_Player;
	tag.Add(Tag::Player);
	position = { 200,200 };
	rotation = 45;
	auto scale = 35.0f;
	collider.AddRect({ -scale / 2.0f, -scale / 2.0f , scale, scale });
	speed = 0.2;
}

void Player::ReciveExp(int value)
{
	cout << "GET Exp " << value << '\n';
}

void Player::Update()
{
	// movement
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
		auto bullet = new Bullet(rotation, 20);
		auto offset = Vector2::FromDegree(rotation);
		offset.set_length(25);
		bullet->position = this->position + offset;
	}

	// attract exp
	for (auto exp : Collider::FindObject({ position, attractExpRange }, [](GameObject* m) {return m->tag.Contains(Tag::Exp); })) {
		auto attractForce = position - exp->position;
		attractForce.set_length(speed / 1.5f);
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

Exp::Exp(float value)
	:value(value)
{
	render_layer = Layer_Exp;
	tag.Add(Tag::Exp);
	rigidbody.decelerate = 0.95;
	collider.AddCircle({ {0,0},value * 3.0f });
}

void Exp::OnCollide(GameObject* other, CollideInfo collideInfo)
{
	if (other->tag.Contains(Tag::Player)) {
		auto player = (Player*)other;
		player->ReciveExp(value);
		Destory();
	}
}

void Exp::Render()
{
	Drawer::AddFillCircle(Color(0.3, 1.0, 0.3), { {0,0}, value * 3.0f });
	Drawer::AddCircle(Color(0.1, 0.6, 0.1), { {0,0}, value * 3.0f });
}
