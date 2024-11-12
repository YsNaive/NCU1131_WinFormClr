#include "App.h"
#include "GameManager.h"
#include "UI.h"

auto test_start = 
Start::Create([]() {
	});

auto test_update = 
Update::Create([]() {
	if (Global::GetKeyDown(Keys::E))
		Global::Player->Hp = 10;
	if (Global::GetKeyDown(Keys::Q))
		Global::Player->Hp = 100;
	if (Global::GetKeyDown(Keys::F5))
		GameManager::Reset(), GameManager::Resume();
	if (Global::GetKeyDown(MouseButtons::Right)) {
		auto text = new Fade_Text("TestGG", Anchor::MiddleCenter);
		text->InitInWorld(Global::Player->position);
	}
	});

auto test_render =
Render::Create([]() {
	RefGlobal::CurrentGraphics->ResetTransform();
	});