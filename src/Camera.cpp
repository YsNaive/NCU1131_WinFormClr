#include "pch.h"
#include "Core.h"

Vector2 Camera::WorldToScreen(const Vector2& pos)
{
	Vector2 ret = pos;
	ret -= position;
	ret += Input::ScreenSize / 2.0f;
	return ret;
}

Vector2 Camera::ScreenToWorld(const Vector2& pos)
{
	Vector2 ret = pos;
	ret -= Input::ScreenSize / 2.0f;
	ret += position;
	return ret;
}

Rect Camera::WorldToScreen(const Rect& rect)
{
	Rect ret = rect;
	ret.set_center(WorldToScreen(rect.get_center()));
	return ret;
}

Rect Camera::ScreenToWorld(const Rect& rect)
{
	Rect ret = rect;
	ret.set_center(ScreenToWorld(rect.get_center()));
	return ret;
}

void Camera::Update()
{
	auto dir = Player::instance->position - position;
	dir *= 0.1;
	position += dir;

	mainCamera.targetScale += sgn(Input::MouseScroller) * 0.1;
	mainCamera.targetScale = clamp(1.0f, 2.5f, mainCamera.targetScale);
	scale += (targetScale - scale) * 0.15;

}

void Camera::Render()
{
}
