
#include "Camera.h"

#include "Global.h"

Vector2 Camera::WorldToScreen(const Vector2& pos)
{
	Vector2 ret = pos;
	ret -= position;
	ret += Global::ScreenSize / 2.0f;
	return ret;
}

Vector2 Camera::ScreenToWorld(const Vector2& pos)
{
	Vector2 ret = pos;
	ret -= Global::ScreenSize / 2.0f;
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
	auto dir = targetPosition - position;
	dir *= Global::DeltaTime * 3.0f;
	position += dir;
	if (Global::MouseScroller != 0) {
		targetScale += sgn(Global::MouseScroller) * 0.1;
		targetScale = clamp(1.0f, 2.5f, targetScale);
	}
	scale += (targetScale - scale) * Global::DeltaTime * 2.0f;
}
