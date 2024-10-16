#include "pch.h"
#include "Window.h"
#include <string>
using CppCLRWinFormsProject::Window;

auto obstacle_setup = Start::Create([]() {
	auto obj = new Obstacle();
	obj->collider.boxes.push_back({ 100,100,200,10 });
	obj->collider.boxes.push_back({ 100,100,10,200 });
	obj->collider.circles.push_back({ { 250,250 }, 40 });
	});

auto app_start = Start::Create([]() {
	auto player = new Player();
	});

auto app_update = Update::Create([]() {
	auto objList = vector<GameObject*>(GameObject::GetInstances().begin(), GameObject::GetInstances().end());
	auto objCount = objList.size();
	if (GetKeyDown(Keys::F3))
		DebugMode = !DebugMode;
	// do update
	for (auto* obj : objList) {
		obj->Update();
	};
	// do collide test
	for (int i = 0; i < objCount; i++) {
		for (int j = i + 1; j < objCount; j++) {
			auto lhs = objList[i];
			auto rhs = objList[j];
			auto collideInfo = lhs->collider.CollideWith(rhs->collider);
			if (collideInfo.is_collide) {
				lhs->OnCollide(rhs, collideInfo);
				rhs->OnCollide(lhs, collideInfo);
			}
		}
	}
	});

auto app_render = OnPaint::Create([]() {
	vector<GameObject*> sorted_obj = vector<GameObject*>(GameObject::GetInstances().begin(), GameObject::GetInstances().end());
	sort(sorted_obj.begin(), sorted_obj.end(), [](GameObject* lhs, GameObject* rhs) { return lhs->render_layer > rhs->render_layer; });
	for (auto* obj : sorted_obj) {
		Drawer::SetRenderTarget(obj, &mainCamera);
		obj->Render();
	}
	if (DebugMode) {
		for (auto* obj : sorted_obj) {
			Drawer::SetRenderTarget(obj, &mainCamera);
			obj->collider.Render();
		}
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
	getKeyUp.clear();
	getKeyDown.clear();
	Window::instance->Invalidate();
	});