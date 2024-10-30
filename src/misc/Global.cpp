
#include "Global.h"

#include "Core.h"
#include "Camera.h"
#include "Player.h"
#include "MouseHandler.h"
#include "KeyBoardHandler.h"

float   Global::Time          = 0.0f;
float   Global::RealTime      = 0.0f;
float   Global::DeltaTime     = 0.0f;
float   Global::RealDeltaTime = 0.0f;
float   Global::TimeScale     = 1.0f;
Vector2	Global::ScreenSize    = { 0.0f,0.0f };
Vector2	Global::MousePosition = { 0.0f,0.0f };
float   Global::MouseScroller = 0.0f;
int     Global::UpdatePerSecond = 0;
long long int Global::UpdateCount = 0;

Camera* Global::MainCamera = nullptr;
Player* Global::Player     = nullptr;

namespace {
	vector<float> ups_buffer;
	chrono::system_clock::time_point last_update_time = chrono::system_clock::now();

	auto value_preset = Start::Create([]() {
		ups_buffer.resize(10);
		Global::MainCamera = new Camera();
		Global::Player = new Player();
		});

	auto value_set = PreUpdate::Create([]() {
		// mouse and screen
		auto window = RefGlobal::MainWindow;
		auto screenMousePos = window->PointToClient(Control::MousePosition);
		Global::MousePosition = { (float)screenMousePos.X, (float)screenMousePos.Y };
		Global::ScreenSize = { (float)window->Size.Width,  (float)window->Size.Height };
		Global::ScreenSize.x -= 14;
		Global::ScreenSize.y -= 39;

		// time
		chrono::system_clock::time_point current = chrono::system_clock::now();
		chrono::duration<double> update_duration = current - last_update_time;
		last_update_time = current;
		Global::RealDeltaTime = update_duration.count();
		Global::DeltaTime     = Global::RealDeltaTime * Global::TimeScale;
		Global::Time	 += Global::DeltaTime;
		Global::RealTime += Global::RealDeltaTime;
		ups_buffer[Global::UpdateCount % 10] = (1.0f / Global::DeltaTime) / Global::TimeScale;
		float ups_sum = 0;
		for (float ups : ups_buffer)
			ups_sum += ups;
		Global::UpdatePerSecond = (int)(ups_sum / 10.0f);

		// camera
		Global::MainCamera->targetPosition = Global::Player->position;
		});

	auto value_clear = LateUpdate::Create([]() {
		InputEvt::ClearMouseInput();
		InputEvt::ClearKeyBoardInput();
		Global::UpdateCount++;
		});
}