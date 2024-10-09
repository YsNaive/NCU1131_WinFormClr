#include "pch.h"
#include "Core.h"
#include "Window.h"
using CppCLRWinFormsProject::Window;

Vector2	screenSize;
Vector2	mousePosition;
set<char> getKey;

Camera mainCamera;

void Drawer::SetPosition(Vector2 position)
{
	Window::instance->graphics->TranslateTransform(position.x, position.y);
}

void Drawer::SetRotation(float rotation)
{
	Window::instance->graphics->RotateTransform(rotation);
}

void Drawer::SetScale(Vector2 scale)
{
	Window::instance->graphics->ScaleTransform(scale.x, scale.y);
}

void Drawer::SetTransform(GameObject* obj)
{
	SetPosition(obj->position);
	SetRotation(obj->rotation);
	SetScale   (obj->scale);
}

void Drawer::AddCircle(Color color, Vector2 center, float radius, float thickness)
{
	Graphics^ g = Window::instance->graphics;

	Pen^ pen = gcnew Pen(color.ToDrawingColor(), thickness);
	g->DrawEllipse(pen, center.x - radius, center.y - radius, radius * 2, radius * 2);
	delete pen;
}

void Drawer::AddRect(Color color, Rect rect, float thickness)
{
	Graphics^ g = Window::instance->graphics;
	Vector2 center = rect.get_center();
	Pen^ pen = gcnew Pen(color.ToDrawingColor(), thickness);
	g->DrawRectangle(pen, rect.x, rect.y, rect.width, rect.height);
	delete pen;
}

Vector2 Camera::WorldToScreen(const Vector2& pos)
{
	Vector2 ret = pos;
	ret -= position; 
	ret += screenSize / 2.0f;
	return ret;
}

Vector2 Camera::ScreenToWorld(const Vector2& pos)
{
	Vector2 ret = pos;
	ret -= screenSize / 2.0f;
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
}

void Camera::Render()
{
}


unordered_set<GameObject*>& GameObject::m_GetInstances()
{
	static unordered_set<GameObject*> instances;
	return instances;
}

const unordered_set<GameObject*>& GameObject::GetInstances()
{
	return m_GetInstances();
}

GameObject::GameObject()
{
	m_GetInstances().insert(this);
}

GameObject::~GameObject()
{
	m_GetInstances().erase(this);
}

void Player::Update()
{
	position = mousePosition;
	rotation += 0.5;
}

void Player::Render()
{
	const float bodySize = 40;
	const float wheelExtend = 5;
	const Rect  body	   = { -bodySize / 2.0f, -bodySize / 2.0f , bodySize,bodySize };
	const Rect  leftWheel  = { body.x - wheelExtend, body.y - wheelExtend , wheelExtend * 2,bodySize + wheelExtend * 2 };
	const Rect  rightWheel = { body.get_xMax() - wheelExtend, body.y - wheelExtend , leftWheel.width, leftWheel.height };

	Drawer::AddRect(Color(.2, .8, .2), body);
	Drawer::AddRect(Color(.2, .8, .2), leftWheel);
	Drawer::AddRect(Color(.2, .8, .2), rightWheel);
}

