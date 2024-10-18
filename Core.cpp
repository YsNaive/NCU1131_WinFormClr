#include "pch.h"
#include "Core.h"
#include "Window.h"
#include <msclr/marshal_cppstd.h>
using CppCLRWinFormsProject::Window;

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
	AddPosition((Input::ScreenSize / 2.0f) / camera->scale);
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

void Drawer::AddText(Color color,const string& text, Vector2 position, float textSize)
{
	Graphics^ g = Window::instance->graphics;
	auto solidBrush = gcnew Drawing::SolidBrush(color.ToDrawingColor());
	auto font		= gcnew Drawing::Font("Consolas", textSize);

	String^ clrStr = msclr::interop::marshal_as<String^>(text);
	g->DrawString(clrStr, font, solidBrush, 10,10);

	delete solidBrush;
	delete font;
	delete clrStr;
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
