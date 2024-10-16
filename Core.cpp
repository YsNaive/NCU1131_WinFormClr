#include "pch.h"
#include "Core.h"
#include "Window.h"
using CppCLRWinFormsProject::Window;

Vector2	screenSize;
Vector2	mousePosition;
set<int> getKey;
set<int> getKeyDown;
set<int> getKeyUp;
bool DebugMode = true;

Camera mainCamera;

void Drawer::AddPosition(Vector2 position)
{
	Window::instance->graphics->TranslateTransform(position.x, position.y);
}

void Drawer::AddRotation(float rotation)
{
	Window::instance->graphics->RotateTransform(rotation);
}

void Drawer::AddScale(Vector2 scale)
{
	Window::instance->graphics->ScaleTransform(scale.x, scale.y);
}

void Drawer::SetRenderTarget(GameObject* obj, Camera* camera)
{
	Window::instance->graphics->ResetTransform();
	AddScale({ camera->scale, camera->scale });
	AddPosition(-mainCamera.position);
	AddPosition((screenSize / 2.0f) / camera->scale);
	AddPosition(obj->position);
	AddRotation(obj->rotation);
}

void Drawer::AddCircle(Color color, Circle circle, float thickness)
{
	Graphics^ g = Window::instance->graphics;

	Pen^ pen = gcnew Pen(color.ToDrawingColor(), thickness);
	g->DrawEllipse(pen, circle.center.x - circle.radius, circle.center.y - circle.radius, circle.radius * 2, circle.radius * 2);
	delete pen;
}

void Drawer::AddFillCircle(Color color, Circle circle)
{
	Graphics^ g = Window::instance->graphics;

	auto solidBrush = gcnew Drawing::SolidBrush(color.ToDrawingColor());
	g->FillEllipse(solidBrush, circle.center.x - circle.radius, circle.center.y - circle.radius, circle.radius * 2, circle.radius * 2);
	delete solidBrush;
}

void Drawer::AddRect(Color color, Rect rect, float thickness)
{
	Graphics^ g = Window::instance->graphics;
	Vector2 center = rect.get_center();
	Pen^ pen = gcnew Pen(color.ToDrawingColor(), thickness);
	g->DrawRectangle(pen, rect.x, rect.y, rect.width, rect.height);
	delete pen;
}

void Drawer::AddFillRect(Color color, Rect rect)
{
	Graphics^ g = Window::instance->graphics;
	Vector2 center = rect.get_center();
	auto solidBrush = gcnew Drawing::SolidBrush(color.ToDrawingColor());

	g->FillRectangle(solidBrush, rect.x, rect.y, rect.width, rect.height);
	delete solidBrush;
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
	auto dir = Player::instance->position - position;
	dir *= 0.1;
	position += dir;
	scale += (targetScale - scale)*0.15;
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

Matrix2x2 GameObject::get_rotateMatrix()
{
	Matrix2x2 ret;
	auto theta = rotation * DEG2RAD;
	ret.m00 = cos(theta);
	ret.m10 = sin(theta);
	ret.m01 = -ret.m10;
	ret.m11 =  ret.m00;
	return ret;
}

GameObject::GameObject()
{
	m_GetInstances().insert(this);
	collider.gameObject = this;
}

GameObject::~GameObject()
{
	m_GetInstances().erase(this);
}
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

	if (getKey.count((int)Keys::W)) {
		curSpeed += 0.25;
	}
	if (getKey.count((int)Keys::S)) {
		curSpeed -= 0.25;
	}
	if (getKey.count((int)Keys::A)) {
		rotation -= 3;
	}
	if (getKey.count((int)Keys::D)) {
		rotation += 3;
	}
	curSpeed = clamp(-maxSpeed, maxSpeed, curSpeed);

	if (curSpeed != 0) {
		auto rad = rotation * DEG2RAD;
		Vector2 offset = { (float)sin(rad), -(float)cos(rad) };
		offset *= curSpeed;
		position += offset;
	}

	if (GetKeyDown(Keys::Space)) {
		auto bullet = new Bullet(rotation, 8);
		bullet->position = this->position;
	}
}

void Player::Render()
{
	const float bodySize = 40;
	const float wheelExtend = 5;
	const Rect  body	   = { -bodySize / 2.0f, -bodySize / 2.0f , bodySize,bodySize };
	const Rect  leftWheel  = { body.x - wheelExtend, body.y - wheelExtend , wheelExtend * 2,bodySize + wheelExtend * 2 };
	const Rect  rightWheel = { body.get_xMax() - wheelExtend, body.y - wheelExtend , leftWheel.width, leftWheel.height };
	const Rect  gun		   = { -wheelExtend/2.0, -bodySize , wheelExtend, bodySize };

	Drawer::AddFillRect(Color(0,0,0), leftWheel);
	Drawer::AddFillRect(Color(0, 0, 0), rightWheel);
	Drawer::AddFillRect(Color(.2, .2, .2), body);
	Drawer::AddFillRect(Color(.4, .4, .4), gun);
}

bool GetKey(Keys keyCode)
{
	return getKey.count((int)keyCode);
}

bool GetKeyDown(Keys keyCode)
{
	return getKeyDown.count((int)keyCode);
}

bool GetKeyUp(Keys keyCode)
{
	return getKeyUp.count((int)keyCode);
}

void Obstacle::Render()
{
	for (auto box : collider.boxes) {
		Drawer::AddFillRect(Color(.1, .2, .1), box);
	}
	for (auto circle : collider.circles) {
		Drawer::AddFillCircle(Color(.1, .2, .1), circle);
	}
}

void Obstacle::OnCollide(GameObject* other, CollideInfo info)
{
}

Entity::Entity()
{
	tag.Add(Tag_Entity);
}

void Collider::Render()
{
	for (auto rect : boxes) {
		Drawer::AddRect(Color(0, 1, 0), rect, 2);
	}
	for (auto circle : circles) {
		Drawer::AddCircle(Color(0, 1, 0), circle, 2);
	}
}

CollideInfo Collider::CollideWith(Collider& other)
{
	auto self_world_boxes    = this->GetWorldPositionBoxes();
	auto self_world_circles  = this->GetWorldPositionCircles();
	auto other_world_boxes   = other.GetWorldPositionBoxes();
	auto other_world_circles = other.GetWorldPositionCircles();

	auto box_box = [](Polygon2D& lhs, Polygon2D& rhs, CollideInfo* ret) {
		for (auto p : lhs.vertices) {
			if (Graph2D::is_collide(p, rhs)) {
				ret->is_collide = true;
				break;
			}
		}
		if (!ret->is_collide) {
			for (auto l : lhs.get_lines()) {
				if (Graph2D::is_collide(l, rhs)) {
					ret->is_collide = true;
					break;
				}
			}
		}
		return ret->is_collide;
		};
	auto box_circle = [](Polygon2D& box, Circle& circle, CollideInfo* ret) {
		float min_distance = numeric_limits<float>().max();
		for (auto l : box.get_lines()) {
			auto distance = (Graph2D::get_closest(circle.center, l) - circle.center).get_length();
			min_distance = min(min_distance, distance);
		}
		if (min_distance <= circle.radius) {
			ret->is_collide = true;
			return true;
		}
		return false;
		};
	auto circle_circle = [](Circle& lhs, Circle& rhs, CollideInfo* ret) {
		auto distance = (lhs.center - rhs.center).get_length();
		if (distance <= (lhs.radius + rhs.radius)) {
			ret->is_collide = true;
			return true;
		}
		return false;
		};

	CollideInfo ret;
	ret.is_collide = false;
	for (auto selfBox : self_world_boxes) {
		for (auto otherBox : other_world_boxes) {
			if (box_box(selfBox, otherBox, &ret))
				return ret;
		}
		for (auto otherCircle : other_world_circles) {
			if (box_circle(selfBox, otherCircle, &ret))
				return ret;
		}
	}
	for (auto selfCircle : self_world_circles) {
		for (auto otherBox : other_world_boxes) {
			if (box_circle(otherBox, selfCircle, &ret))
				return ret;
		}
		for (auto otherCircle : other_world_circles) {
			if (circle_circle(selfCircle, otherCircle, &ret))
				return ret;
		}
	}
	return ret;
}

vector<Polygon2D> Collider::GetWorldPositionBoxes()
{
	auto rotateMatrix = gameObject->get_rotateMatrix();
	vector<Polygon2D> polys;
	polys.reserve(boxes.size());
	for (auto rect : boxes) {
		vector<Vector2> path;
		for (auto point : {
			Vector2(rect.get_xMin(), rect.get_yMin()),
			Vector2(rect.get_xMin(), rect.get_yMax()),
			Vector2(rect.get_xMax(), rect.get_yMax()),
			Vector2(rect.get_xMax(), rect.get_yMin()),
			}) path.push_back((rotateMatrix * point) + gameObject->position);
		polys.push_back(Polygon2D(path));
	}
	return polys;
}

vector<Circle> Collider::GetWorldPositionCircles()
{
	auto rotateMatrix = gameObject->get_rotateMatrix();
	vector<Circle> ret;
	ret.reserve(circles.size());
	for (auto circle : circles)
		ret.push_back({ (rotateMatrix * circle.center) + gameObject->position, circle.radius });
	return ret;
}

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
		is_destory = true;
}

void Bullet::Render()
{
	Drawer::AddFillCircle(Color(.9, .4, .4), collider.circles[0]);
}
