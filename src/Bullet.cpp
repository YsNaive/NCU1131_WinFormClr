#include "pch.h"
#include "Core.h"

Bullet::Bullet(float direction, float speed, float destoryDistance)
	: direction(direction), speed(speed), destoryDistance(destoryDistance)
{
	tag.Add(Tag_Bullet);
	collider.circles.push_back(Circle({ 0,0 }, 3.5));
}

void Bullet::Update()
{
	auto theta = DEG2RAD * direction;
	position.x += speed * sin(theta);
	position.y += speed * -cos(theta);
	movedDistance += speed;
	if (movedDistance > destoryDistance)
		this->Destory();
}

void Bullet::Render()
{
	Drawer::AddFillCircle(Color(.9, .4, .4), collider.circles[0]);
}
