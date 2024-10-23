#include "pch.h"
#include "Core.h"
#include "Window.h"
#include <msclr/marshal_cppstd.h>
using CppCLRWinFormsProject::Window;

bool DebugMode = true;

Camera mainCamera;

void Obstacle::Render()
{
}

void Obstacle::OnCollide(GameObject* other, CollideInfo info)
{
}

Entity::Entity()
{
	tag.Add(Tag_Entity);
}

void Entity::ReciveDamage(float value, GameObject* sender)
{
	if (hp == numeric_limits<float>().min())
		return;
	value -= defence;
	if (value < 0)
		return;

	hp -= value;
	OnHurt();
	if (hp < 0) {
		OnDead();
		hp = numeric_limits<float>().min();
	}
}

bool Collider::IsIgnore(GameObject* lhs, GameObject* rhs)
{
	auto table = GetIgnoreCollideList();
	for (auto& t1 : lhs->tag.tags) {
		for (auto& t2 : rhs->tag.tags) {
			if (find(table.begin(), table.end(), pair<string, string>(t1, t2)) != table.end())
				return true;
		}
	}
	return false;
}

vector<pair<string, string>>& Collider::GetIgnoreCollideList() {
	static vector<pair<string, string>> instance; 
	return instance;
}

unordered_set<GameObject*> Collider::FindObject(const Circle& range, function<bool(GameObject*)> filter)
{
	unordered_set<GameObject*> ret;
	Collider tempCollider;
	tempCollider.AddCircle(range);
	for (auto obj : GameObject::GetInstances()) {
		if (!filter(obj))
			continue;
		if (tempCollider.CollideWith(obj->collider).is_collide)
			ret.insert(obj);
	}
	return ret;
}

unordered_set<GameObject*> Collider::FindObject(const Circle& range)
{ 
	static auto filter = [](GameObject*) {return true; };
	return FindObject(range, filter);
}

void Collider::AddIgnore(const string lhs, const string rhs) {
	GetIgnoreCollideList().push_back({ lhs, rhs }); if (lhs != rhs) GetIgnoreCollideList().push_back({ rhs, lhs });
}

void Collider::UpdateBoundingBox()
{
	boundingBox = { 0,0,0,0 };
	for (auto& poly : hitboxes) {
		auto box = poly.get_boundingBox();
		if (boundingBox.width == 0 || boundingBox.height == 0) {
			boundingBox = box;
			continue;
		}
		if (boundingBox.get_xMin() > box.get_xMin())
			boundingBox.set_xMin(box.get_xMin());
		if (boundingBox.get_xMax() < box.get_xMax())
			boundingBox.set_xMax(box.get_xMax());
		if (boundingBox.get_yMin() > box.get_yMin())
			boundingBox.set_yMin(box.get_yMin());
		if (boundingBox.get_yMax() < box.get_yMax())
			boundingBox.set_yMax(box.get_yMax());
	}
}

void Collider::AddRect(const Rect& rect)
{
	hitboxes.push_back(Polygon2D({
		{rect.x,		      rect.y			  },
		{rect.x + rect.width, rect.y			  },
		{rect.x + rect.width, rect.y + rect.height},
		{rect.x,			  rect.y + rect.height},
		}));
}

void Collider::AddCircle(const Circle& circle)
{
	const float step = (360.0 / 16.0) * DEG2RAD;
	Polygon2D poly;
	poly.vertices.resize(16);
	float rad = 0.0;
	for (int i = 0; i < 16; i++) {
		poly.vertices[i] = Vector2( sin(rad), cos(rad) );
		poly.vertices[i].set_length(circle.radius);
		poly.vertices[i] += circle.center;
		rad += step;
	}
	hitboxes.push_back(poly);
}

void Collider::Render()
{
	for (auto& poly : hitboxes) {
		Drawer::AddPoly(Color(0, 1, 0), poly, 1);
	}
}

CollideInfo Collider::CollideWith(Collider& other)
{
	auto self_world_polys    = this->GetWorldPositionHitboxes();
	auto other_world_polys   = other.GetWorldPositionHitboxes();

	auto hitCheck = [](Polygon2D& lhs, Polygon2D& rhs, CollideInfo* ret) {
		for (auto p : lhs.vertices) {
			if (Graph2D::is_collide(p, rhs)) {
				ret->is_collide = true;
				break;
			}
		}
		if (ret->is_collide) {
			for (auto l : lhs.get_lines()) {
				if (Graph2D::is_collide(l, rhs)) {
					ret->is_collide = true;
					break;
				}
			}
		}
		return ret->is_collide;
		};

	CollideInfo ret;
	ret.is_collide = false;
	for (auto& p1 : self_world_polys) {
		for (auto& p2 : other_world_polys) {
			if (hitCheck(p1, p2, &ret))
				break;
			if (hitCheck(p2, p1, &ret))
				break;
		}
		if (ret.is_collide)
			break;
	}

	// find hit line (surface)
	if (ret.is_collide) {
		float min_distance = numeric_limits<float>().max();
		for (auto& self_poly  : self_world_polys) {
		for (auto& self_point : self_poly.vertices) {
		for (auto& other_poly : other_world_polys) {
		for (auto& other_line : other_poly.get_lines()) {
			float distance = Graph2D::get_distance(self_point, other_line);
			if (distance < min_distance) {
				min_distance = distance;
				ret.hitLine  = other_line;
				ret.hitPoint = self_point;
			}
		}}}};
	}
	return ret;
}

vector<Polygon2D> Collider::GetWorldPositionHitboxes()
{
	auto rotateMatrix = gameObject ? gameObject->get_rotateMatrix() : Matrix2x2::FromRotation(0);
	Vector2 offset    = gameObject ? gameObject->position : Vector2(0, 0);

	vector<Polygon2D> polys;
	polys.reserve(hitboxes.size());
	for (auto& poly : hitboxes) {
		Polygon2D worldPoly(poly);
		for (int i = 0, imax = poly.vertices.size(); i < imax; i++)
			worldPoly.vertices[i] = (rotateMatrix * poly.vertices[i]) + offset;
		polys.push_back(worldPoly);
	}
	return polys;
}

void Rigidbody::Update()
{
	if (!enable)
		return;
	if (!gameObject)
		return;
	auto offset = movement;
	if (relate_rotation)
		offset = gameObject->get_rotateMatrix() * offset;
	
	gameObject->position += offset;
	movement *= decelerate;
}

void Rigidbody::AddForce(Vector2 force)
{
	movement += force;
	if (movement.get_length() > maxSpeed)
		movement.set_length(maxSpeed);
}

void Rigidbody::AddForce(Vector2 direction, float force)
{
	direction.set_length(force);
	AddForce(direction);
}

void Rigidbody::AddForce(float rotation, float force)
{
	Vector2  vec_force = Vector2::FromDegree(rotation);
	vec_force.set_length(force);
	AddForce(vec_force);
}

Matrix2x2 Matrix2x2::FromRotation(float degree)
{
	Matrix2x2 ret;
	auto theta = degree * DEG2RAD;
	ret.m00 = cos(theta);
	ret.m10 = sin(theta);
	ret.m01 = -ret.m10;
	ret.m11 = ret.m00;
	return ret;
}
