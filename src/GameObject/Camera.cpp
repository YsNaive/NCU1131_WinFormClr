
#include "Camera.h"

#include "Global.h"
#include "Player.h"
#include "Drawer.h"
#include "Resources.h"

namespace {
	ref class ImgBuffer {
	public:
		static Image^ f0;
	};
	auto render_bg = 
	Render::Create([](){
		RefGlobal::CurrentGraphics->ResetTransform();
		float scale = Global::MainCamera->scale;
		float width  = 200 * scale;
		float gapOffset = width * 0.015f;
		Vector2 begin_pos = Global::Player->position * scale;
		begin_pos.x -= ((int)(begin_pos.x / width)) * width;
		begin_pos.x = -begin_pos.x - width;
		begin_pos.y -= ((int)(begin_pos.y / width)) * width;
		begin_pos.y = -begin_pos.y - width;
		Vector2 pos = begin_pos;
		int c = 0;
		if (ImgBuffer::f0 == nullptr ||
			ImgBuffer::f0->Size.Width != width)
			ImgBuffer::f0 = RefResources::ResizeImage(RefResources::ForestBackground[0], width, width);
		while (pos.x < Global::ScreenSize.x) {
			pos.y = begin_pos.y;
			while (pos.y < Global::ScreenSize.y) {
				Drawer::AddImage(ImgBuffer::f0, {pos, {width,width}});
				pos.y += width - gapOffset;
				c++;
			}
			pos.x += width - gapOffset;
		}
	});
}

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
	position = targetPosition;
	if (Global::MouseScroller != 0) {
		targetScale += sgn(Global::MouseScroller) * 0.1;
		targetScale = clamp(1.0f, 2.5f, targetScale);
	}
	scale += (targetScale - scale) * Global::DeltaTime * 2.0f;
}

