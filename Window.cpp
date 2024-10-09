#include "pch.h"
#include "Window.h"
#include <string>
using CppCLRWinFormsProject::Window;

auto app_start = Start::Create([]() {
	auto player = new Player();
	});

auto app_update = Update::Create([]() {
	for (auto* obj : GameObject::GetInstances())
		obj->Update();
	});

auto app_render = OnPaint::Create([]() {
	for (auto* obj : GameObject::GetInstances()) {
		Drawer::SetTransform(obj);
		obj->Render();
	}
	});

auto value_setup = PreUpdate::Create([]() {
	auto window = Window::instance;
	auto screenMousePos = Window::instance->PointToClient(Control::MousePosition);

	mousePosition = { (float)screenMousePos.X, (float)screenMousePos.Y };
	screenSize	  = { (float)window->Size.Width,  (float)window->Size.Height };
	screenSize.x -= 14;
	screenSize.y -= 39;
	});
auto value_clear = LateUpdate::Create([]() {
	getKey.clear();
	Window::instance->Invalidate();
	});