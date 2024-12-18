
#include "GameObject.h"

#include "Drawer.h"

bool Collider::IsIgnore(GameObject* lhs, GameObject* rhs)
{
	static auto& table = GetIgnoreCollideList();
	for (auto flag : table) {
		int collide_flag = lhs->tag.flag | rhs->tag.flag;
		if ((collide_flag ^ flag) == flag)
			return true;
	}
	return false;
}

vector<int>& Collider::GetIgnoreCollideList() {
	static vector<int> instance;
	return instance;
}

unordered_set<GameObject*> Collider::FindObject(const Circle& range, function<bool(GameObject*)> filter)
{
	unordered_set<GameObject*> ret;
	Collider tempCollider;
	tempCollider.AddCircle(range);
	tempCollider.Update();
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

void Collider::AddIgnore(const int lhs, const int rhs) {
	GetIgnoreCollideList().push_back(lhs | rhs);
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
	const int v_count = 8;
	const float step = (360.0 / (float)v_count) * DEG2RAD;
	Polygon2D poly;
	poly.vertices.resize(v_count);
	float rad = 0.0;
	for (int i = 0; i < v_count; i++) {
		poly.vertices[i] = Vector2(sin(rad), cos(rad));
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
	if (!Graph2D::is_collide(this->boundingBox, other.boundingBox))
		return ret;

	for (auto& p1 : this->hitboxes_world) {
		for (auto& p2 : other.hitboxes_world) {
			if (hitCheck(p1, p2, &ret))
				break;
			if (hitCheck(p2, p1, &ret))
				break;
		}
		if (ret.is_collide)
			break;
	}
	return ret;
}

void Collider::Update() {
	if (hitboxes.empty())
		return;
	auto rotateMatrix = gameObject ? gameObject->get_rotateMatrix() : Matrix2x2::FromRotation(0);
	Vector2 offset = gameObject ? gameObject->position : Vector2(0, 0);

	hitboxes_world.resize(hitboxes.size());
	int p = 0;
	for (auto& poly : hitboxes) {
		auto& target = hitboxes_world[p]; p++;
		target = Polygon2D(poly);
		for (int i = 0, imax = poly.vertices.size(); i < imax; i++)
			target.vertices[i] = (rotateMatrix * target.vertices[i]) + offset;
	}

	boundingBox = hitboxes_world[0].get_boundingBox();
	for (int i = 1; i < hitboxes_world.size(); i++) {
		auto poly_bbox = hitboxes_world[i].get_boundingBox();
		boundingBox.set_xMin(min(boundingBox.get_xMin(), poly_bbox.get_xMin()));
		boundingBox.set_yMin(min(boundingBox.get_yMin(), poly_bbox.get_yMin()));
		boundingBox.set_xMax(max(boundingBox.get_xMax(), poly_bbox.get_xMax()));
		boundingBox.set_yMax(max(boundingBox.get_yMax(), poly_bbox.get_yMax()));
	}
}

void Rigidbody::Update()
{
	if (Global::TimeScale == 0)
		return;
	if (!enable)
		return;
	if (!gameObject)
		return;
	auto offset = movement;
	if (relate_rotation)
		offset = gameObject->get_rotateMatrix() * offset;

	gameObject->position += offset;
	movement *= decelerate ;
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

unordered_set<GameObject*>& GameObject::m_GetInstances()
{
	static unordered_set<GameObject*> instances;
	return instances;
}
const unordered_set<GameObject*>& GameObject::GetInstances()
{
	return m_GetInstances();
}

unordered_set<GameObject*> GameObject::GetInstances(int tagMask)
{
	unordered_set<GameObject*> ret;
	for (auto* obj : m_GetInstances()) {
		if (obj->tag.Contains(tagMask)) {
			ret.insert(obj);
		}
	}
	return ret;
}

void GameObject::Destroy()
{
	mark_destory = true;
}

Matrix2x2 GameObject::get_rotateMatrix()
{
	return Matrix2x2::FromRotation(rotation);
}

GameObject::GameObject()
{
	m_GetInstances().insert(this);
	collider.gameObject = this;
	rigidbody.gameObject = this;
}

GameObject::~GameObject()
{
	m_GetInstances().erase(this);
}