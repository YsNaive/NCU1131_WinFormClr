#include "pch.h"
#include "Window.h"
#include <string>
using CppCLRWinFormsProject::Window;

auto obstacle_setup = Start::Create([]() {
	for (int i = 0; i < 15; i++) {
		auto obj = new NormalMonster();
		obj->position = { (float)i,(float)i };
	}
	});

auto app_start = Start::Create([]() {
	auto player = new Player();
	});

auto app_update = Update::Create([]() {
	auto objList = vector<GameObject*>(GameObject::GetInstances().begin(), GameObject::GetInstances().end());
	auto objCount = objList.size();
	if (Input::GetKeyDown(Keys::F3))
		DebugMode = !DebugMode;
	// do update
	for (auto* obj : objList) {
		obj->rigidbody.Update();
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
		auto g = Window::instance->graphics;
		g->ResetTransform();
		Drawer::AddText({ .2f,.2f,.2f }, "mouse position ", { 0,0 });
		Drawer::AddText({ .2f,.2f,.2f }, "    screen " + Input::MousePosition.to_string(), { 0,15 });
		Drawer::AddText({ .2f,.2f,.2f }, "    world  " + mainCamera.ScreenToWorld(Input::MousePosition).to_string(), {0,30});
	}
	});

auto value_setup = PreUpdate::Create([]() {
	auto window = Window::instance;
	auto screenMousePos = Window::instance->PointToClient(Control::MousePosition);

	Input::MousePosition = { (float)screenMousePos.X, (float)screenMousePos.Y };
	Input::ScreenSize    = { (float)window->Size.Width,  (float)window->Size.Height };
	Input::ScreenSize.x -= 14;
	Input::ScreenSize.y -= 39;
	});
auto value_clear = LateUpdate::Create([]() {
	Input::s_getKeyUp.clear();
	Input::s_getKeyDown.clear();
	Input::s_mouseState[0][0] = false;
	Input::s_mouseState[0][2] = false;
	Input::MouseScroller = 0;
	Window::instance->Invalidate();
	});