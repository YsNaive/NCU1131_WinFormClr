
#include "Drawer.h"

#include "Core.h"
#include "Camera.h";

namespace {
	auto refDrawer_setup = Start::Create([]() {
		RefDrawer::Font		  = gcnew Drawing::Font("·L³n¥¿¶ÂÅé", 14);
		RefDrawer::Pen		  = gcnew Pen(Color(0,0,0).ToDrawingColor());
		RefDrawer::SolidBrush = gcnew SolidBrush(Color(0, 0, 0).ToDrawingColor());
		RefDrawer::PathBuffer = gcnew GraphicsPath();
		});
}

void RefDrawer::SetPathBuffer(Polygon2D& poly)
{
	auto path = RefDrawer::PathBuffer;
	path->Reset();
	for (auto l : poly.get_lines()) {
		path->AddLine(l.p1.x, l.p1.y, l.p2.x, l.p2.y);
	}
}

void RefDrawer::SetStringBuffer(const string& str)
{
	if (RefDrawer::StringBuffer != nullptr)
		delete RefDrawer::StringBuffer;
	RefDrawer::StringBuffer = msclr::interop::marshal_as<String^>(str);
}

Vector2 RefDrawer::MeasureStringSize()
{
	auto size = RefGlobal::CurrentGraphics->MeasureString(RefDrawer::StringBuffer, RefDrawer::Font);
	return { size.Width, size.Height };
}

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
	if (!obj->tag.Contains(Tag::UI)) {
		AddScale({ camera->scale, camera->scale });
		AddPosition(-Global::MainCamera->position);
		AddPosition((Global::ScreenSize / 2.0f) / camera->scale);
	}
	AddPosition(obj->position);
	AddRotation(obj->rotation);
}

void Drawer::SetPen(Color color, float thickness)
{
	RefDrawer::Pen->Color = color.ToDrawingColor();
	RefDrawer::Pen->Width = thickness;
}

void Drawer::SetBrush(Color color)
{
	RefDrawer::SolidBrush->Color = color.ToDrawingColor();
	RefDrawer::UsingBrush = RefDrawer::SolidBrush;
}

void Drawer::SetBrush(Color begColor, Color endColor, Vector2 begPos, Vector2 endPos)
{
	if (RefDrawer::GradientBrush != nullptr)
		delete RefDrawer::GradientBrush;
	RefDrawer::GradientBrush = gcnew LinearGradientBrush(
		Drawing::Point(begPos.x, begPos.y),
		Drawing::Point(endPos.x, endPos.y),
		begColor.ToDrawingColor(),
		endColor.ToDrawingColor());
	RefDrawer::UsingBrush = RefDrawer::GradientBrush;
}

void Drawer::AddCircle(Circle circle)
{
	RefGlobal::CurrentGraphics->DrawEllipse(
		RefDrawer::Pen,
		circle.center.x - circle.radius,
		circle.center.y - circle.radius,
		circle.radius * 2,
		circle.radius * 2);
}

void Drawer::AddCircle(Color color, Circle circle, float thickness)
{
	SetPen(color, thickness);
	AddCircle(circle);
}

void Drawer::AddFillCircle(Circle circle)
{
	RefGlobal::CurrentGraphics->FillEllipse(
		RefDrawer::UsingBrush,
		circle.center.x - circle.radius,
		circle.center.y - circle.radius,
		circle.radius * 2,
		circle.radius * 2);
}

void Drawer::AddFillCircle(Color color, Circle circle)
{
	SetBrush(color);
	AddFillCircle(circle);
}

void Drawer::AddRect(Rect rect)
{
	Vector2 center = rect.get_center();
	RefGlobal::CurrentGraphics->DrawRectangle(
		RefDrawer::Pen, 
		rect.x		  , 
		rect.y		  , 
		rect.width	  , 
		rect.height	 );
}

void Drawer::AddRect(Color color, Rect rect, float thickness)
{
	SetPen(color, thickness);
	AddRect(rect);
}

void Drawer::AddFillRect(Rect rect)
{
	RefGlobal::CurrentGraphics->FillRectangle(
		RefDrawer::UsingBrush,
		rect.x,
		rect.y,
		rect.width,
		rect.height);
}

void Drawer::AddFillRect(Color color, Rect rect)
{
	SetBrush(color);
	AddFillRect(rect);
}

void Drawer::AddPoly(Polygon2D& poly)
{
	RefDrawer::SetPathBuffer(poly);
	RefGlobal::CurrentGraphics->DrawPath(
		RefDrawer::Pen,
		RefDrawer::PathBuffer);
}

void Drawer::AddPoly(Color color, Polygon2D& poly, float thickness)
{
	SetPen(color, thickness);
	AddPoly(poly);
}

void Drawer::AddFillPoly(Polygon2D& poly)
{
	RefDrawer::SetPathBuffer(poly);
	RefGlobal::CurrentGraphics->FillPath(
		RefDrawer::UsingBrush,
		RefDrawer::PathBuffer);
}

void Drawer::AddFillPoly(Color color, Polygon2D& poly)
{
	SetBrush(color);
	AddFillPoly(poly);
}

void Drawer::AddText(const string& text, Vector2 position, Anchor anchor)
{
	RefDrawer::SetStringBuffer(text);
	Rect bound = { {0,0}, RefDrawer::MeasureStringSize() };
	position -= anchor.GetPosition(bound);
	RefGlobal::CurrentGraphics->DrawString(
		RefDrawer::StringBuffer,
		RefDrawer::Font,
		RefDrawer::UsingBrush,
		position.x,
		position.y);
}

void Drawer::AddText(Color color, const string& text, Vector2 position, Anchor anchor)
{
	SetBrush(color);
	AddText(text, position, anchor);
}

void Drawer::AddText(Color color, const string& text, Vector2 position, float textSize, Anchor anchor)
{
	auto font_origin = RefDrawer::Font;
	auto font_temp   = gcnew Drawing::Font(font_origin ->OriginalFontName, textSize);
	RefDrawer::Font = font_temp;
	SetBrush(color);
	AddText(text, position, anchor);
	RefDrawer::Font = font_origin;
	delete font_temp;
}

void Drawer::AddImage(Image^ image, Rect position)
{
	RefGlobal::CurrentGraphics->DrawImage(
		image,
		Drawing::Rectangle(
			position.x,
			position.y,
			position.width,
			position.height));
}

void InputEvt::OnPaint(Object^ sender, PaintEventArgs^ e)
{
	RefGlobal::CurrentGraphics = e->Graphics;
	Render::Invoke();
}
