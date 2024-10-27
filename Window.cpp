#include "pch.h"
#include "Window.h"
#include <chrono>
#include <string>
using CppCLRWinFormsProject::Window;

auto obstacle_setup = Start::Create([]() {
	for (int i = 0; i < 10; i++) {
		auto obj = new NormalMonster(10 + (rand()%5));
		obj->position = { (float)i * 1,(float)i * 1 };
	}
	});

auto app_start = Start::Create([]() {
	auto player = new Player();
	});

auto collide_ignore_setup = Start::Create([]() {
	Collider::AddIgnore(Tag::Exp, ~0 | Tag::Player);
	Collider::AddIgnore(Tag::Bullet, Tag::Bullet);
						   
	});

float time_update  = 0;
float time_collide = 0;
float time_render  = 0;
auto app_update = Update::Create([]() {
	auto time_begin = chrono::system_clock::now();
	auto objList = vector<GameObject*>(GameObject::GetInstances().begin(), GameObject::GetInstances().end());
	auto objCount = objList.size();
	if (Input::GetKeyDown(Keys::F3))
		DebugMode = !DebugMode;
	// do update
	for (auto* obj : objList) {
		obj->rigidbody.Update();
		obj->collider .Update();
		obj->Update();
	};
	chrono::duration<double> duration = (chrono::system_clock::now() - time_begin);
	time_update = duration.count();
	time_begin  = chrono::system_clock::now();
	// do collide test
	for (int i = 0; i < objCount; i++) {
		for (int j = i + 1; j < objCount; j++) {
			auto lhs = objList[i];
			auto rhs = objList[j];
			if (Collider::IsIgnore(lhs, rhs))
				continue;
			auto collideInfo = (lhs->rigidbody.movement.get_length() > rhs->rigidbody.movement.get_length()) 
				? lhs->collider.CollideWith(rhs->collider)
				: rhs->collider.CollideWith(lhs->collider);
			if (collideInfo.is_collide) {
				lhs->OnCollide(rhs, collideInfo);
				rhs->OnCollide(lhs, collideInfo);
			}
		}
	}
	duration = (chrono::system_clock::now() - time_begin);
	time_collide = duration.count();
	});

auto app_render = OnPaint::Create([]() {
	auto time_begin = chrono::system_clock::now();
	vector<GameObject*> sorted_obj = vector<GameObject*>(GameObject::GetInstances().begin(), GameObject::GetInstances().end());
	sort(sorted_obj.begin(), sorted_obj.end(), [](GameObject* lhs, GameObject* rhs) { return lhs->render_layer < rhs->render_layer; });
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
		Color textColor = { .2f,.2f,.2f };
		float y = 0;
		auto time_total = time_update + time_collide + time_render;
		Drawer::AddText(textColor, FormatString("time_update  : %d", (int)((time_update  / time_total) * 100)), { 0, (y++) * 15 });
		Drawer::AddText(textColor, FormatString("time_collide : %d", (int)((time_collide / time_total) * 100)), { 0, (y++) * 15 });
		Drawer::AddText(textColor, FormatString("time_render  : %d", (int)((time_render  / time_total) * 100)), { 0, (y++) * 15 });
		Drawer::AddText(textColor, FormatString("time_loop    : %.3f" , time_total), {0, (y++) * 15});
		/*
		Drawer::AddText({ .2f,.2f,.2f }, "mouse position ", { 0,0 });
		Drawer::AddText({ .2f,.2f,.2f }, "    screen " + Input::MousePosition.to_string(), { 0,15 });
		Drawer::AddText({ .2f,.2f,.2f }, "    world  " + mainCamera.ScreenToWorld(Input::MousePosition).to_string(), {0,30});
		*/
	}
	chrono::duration<double> duration = (chrono::system_clock::now() - time_begin);
	time_render = duration.count();
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