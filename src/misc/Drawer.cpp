
#include "Drawer.h"

#include "Core.h"
#include "Camera.h";

void Drawer::AddPosition(Vector2 position)
{
	RefGlobal::CurrentGraphics->TranslateTransform(position.x, position.y);
}

void Drawer::AddRotation(float rotation)
{
	RefGlobal::CurrentGraphics->RotateTransform(rotation);
}

void Drawer::AddScale(Vector2 scale)
{
	RefGlobal::CurrentGraphics->ScaleTransform(scale.x, scale.y);
}

void Drawer::SetRenderTarget(GameObject* obj, Camera* camera)
{
	RefGlobal::CurrentGraphics->ResetTransform();
	AddScale({ camera->scale, camera->scale });
	AddPosition(-Global::MainCamera->position);
	AddPosition((Global::ScreenSize / 2.0f) / camera->scale);
	AddPosition(obj->position);
	AddRotation(obj->rotation);
}

void Drawer::AddCircle(Color color, Circle circle, float thickness)
{
	Graphics^ g = RefGlobal::CurrentGraphics;

	Pen^ pen = gcnew Pen(color.ToDrawingColor(), thickness);
	g->DrawEllipse(pen, circle.center.x - circle.radius, circle.center.y - circle.radius, circle.radius * 2, circle.radius * 2);
	delete pen;
}

void Drawer::AddFillCircle(Color color, Circle circle)
{
	Graphics^ g = RefGlobal::CurrentGraphics;

	auto solidBrush = gcnew Drawing::SolidBrush(color.ToDrawingColor());
	g->FillEllipse(solidBrush, circle.center.x - circle.radius, circle.center.y - circle.radius, circle.radius * 2, circle.radius * 2);
	delete solidBrush;
}

void Drawer::AddRect(Color color, Rect rect, float thickness)
{
	Graphics^ g = RefGlobal::CurrentGraphics;
	Vector2 center = rect.get_center();
	Pen^ pen = gcnew Pen(color.ToDrawingColor(), thickness);
	g->DrawRectangle(pen, rect.x, rect.y, rect.width, rect.height);
	delete pen;
}

void Drawer::AddFillRect(Color color, Rect rect)
{
	Graphics^ g = RefGlobal::CurrentGraphics;
	Vector2 center = rect.get_center();
	auto solidBrush = gcnew Drawing::SolidBrush(color.ToDrawingColor());

	g->FillRectangle(solidBrush, rect.x, rect.y, rect.width, rect.height);
	delete solidBrush;
}

void Drawer::AddPoly(Color color, Polygon2D& poly, float thickness)
{
	Graphics^ g = RefGlobal::CurrentGraphics;

	Pen^ pen = gcnew Pen(color.ToDrawingColor(), thickness);
	auto path = gcnew Drawing::Drawing2D::GraphicsPath();
	for (auto l : poly.get_lines()) {
		path->AddLine(l.p1.x, l.p1.y, l.p2.x, l.p2.y);
	}
	g->DrawPath(pen, path);

	delete pen;
	delete path;
}

void Drawer::AddFillPoly(Color color, Polygon2D& poly)
{
	Graphics^ g = RefGlobal::CurrentGraphics;

	Brush^ brush = gcnew Drawing::SolidBrush(color.ToDrawingColor());
	auto path = gcnew Drawing::Drawing2D::GraphicsPath();
	for (auto l : poly.get_lines()) {
		path->AddLine(l.p1.x, l.p1.y, l.p2.x, l.p2.y);
	}
	g->FillPath(brush, path);
	delete brush;
	delete path;
}

void Drawer::AddText(Color color, const string& text, Vector2 position, Anchor anchor, float textSize)
{
	Graphics^ g = RefGlobal::CurrentGraphics;
	auto solidBrush = gcnew Drawing::SolidBrush(color.ToDrawingColor());
	auto font = gcnew Drawing::Font("Consolas", textSize);
	
	String^ clrStr = msclr::interop::marshal_as<String^>(text);
	auto size = g->MeasureString(clrStr, font);
	Rect bound = { 0,0, size.Width, size.Height };
	position -= anchor.GetPosition(bound);

	g->DrawString(clrStr, font, solidBrush, position.x, position.y);
	delete solidBrush;
	delete font;
	delete clrStr;
}

void InputEvt::OnPaint(Object^ sender, PaintEventArgs^ e)
{
	RefGlobal::CurrentGraphics = e->Graphics;
	Render::Invoke();
}
