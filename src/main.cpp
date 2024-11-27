
#include "App.h"

void AppLoop(Object^ sender, EventArgs^ e);
[STAThread]
void main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	RefGlobal::MainTimer = gcnew Timer();
	RefGlobal::MainTimer->Interval = 1;
	RefGlobal::MainTimer->Tick += gcnew System::EventHandler(&AppLoop);
	RefGlobal::MainTimer->Start();

	RefGlobal::MainWindow = gcnew Window();
	RefGlobal::MainWindow->MouseWheel += gcnew MouseEventHandler(&InputEvt::OnMouseWheel);
	RefGlobal::MainWindow->MouseDown  += gcnew MouseEventHandler(&InputEvt::OnMouseDown);
	RefGlobal::MainWindow->MouseUp	  += gcnew MouseEventHandler(&InputEvt::OnMouseUp);
	RefGlobal::MainWindow->KeyDown	  += gcnew KeyEventHandler  (&InputEvt::OnKeyDown);
	RefGlobal::MainWindow->KeyUp	  += gcnew KeyEventHandler  (&InputEvt::OnKeyUp);
	RefGlobal::MainWindow->Paint	  += gcnew PaintEventHandler(&InputEvt::OnPaint);
	RefGlobal::MainWindow->Width  = 800;
	RefGlobal::MainWindow->Height = 600;

	Start::Invoke();
	GameManager::Reset();
	GameManager::Resume();
	Application::Run(RefGlobal::MainWindow);
}

namespace {
	const int   maxFPS = 30;
	const float renderDelay = 1.0 / maxFPS;
	float lastRenderTime = 0;
}
void AppLoop(Object^ sender, EventArgs^ e)
{
	PreUpdate ::Invoke();
	Update	  ::Invoke();
	LateUpdate::Invoke();
	if (Global::Time - lastRenderTime > renderDelay)
		RefGlobal::MainWindow->Invalidate();
}

auto collide_ignore_setup = Start::Create([]() {
	Collider::AddIgnore(Tag::Exp, ~0 | Tag::Player);
	Collider::AddIgnore(Tag::Bullet, Tag::Bullet);

	});

bool DebugMode = false;
auto object_update = Update::Create([]() {
	auto objList = vector<GameObject*>(GameObject::GetInstances().begin(), GameObject::GetInstances().end());
	auto objCount = objList.size();
	if (Global::GetKeyDown(Keys::F3))
		DebugMode = !DebugMode;
	if (Global::GetKeyDown(Keys::Escape))
		Global::TimeScale = 1 - Global::TimeScale;
	Collider mouseCollider = Collider();
	mouseCollider.AddCircle({ Global::MousePosition, 1 });
	mouseCollider.Update();
	// do update
	for (auto* obj : objList) {
		if (!obj->enable)
			continue;
		obj->rigidbody.Update();
		obj->collider.Update();
		obj->Update();

		if (obj->tag.Contains(Tag::Clickable)) {
			auto clickable = (UI_Clickable*)obj;
			clickable->IsHovering = mouseCollider.CollideWith(obj->collider).is_collide;
			if (clickable->IsHovering && Global::GetKeyDown(MouseButtons::Left)) {
				for (auto func : clickable->OnClick)
					func();
			}
		}
	};

	// do collide test
	for (int i = 0; i < objCount; i++) {
		for (int j = i + 1; j < objCount; j++) {
			auto lhs = objList[i];
			auto rhs = objList[j];
			if (!lhs->enable || !rhs->enable)
				continue;
			if (lhs->collider.hitboxes.empty() || rhs->collider.hitboxes.empty())
				continue;
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

	if (Global::GetKeyDown(Keys::F5))
		GameManager::Reset(), GameManager::Resume();

	// delete destroy object
	vector<GameObject*> toDestroy;
	for (auto obj : GameObject::GetInstances()) {
		if (obj->is_Destroy())
			toDestroy.push_back(obj);
	}
	for (auto obj : toDestroy)
		delete obj;
	});

auto render_update = Render::Create([]() {
	vector<GameObject*> sorted_obj = vector<GameObject*>(GameObject::GetInstances().begin(), GameObject::GetInstances().end());
	sort(sorted_obj.begin(), sorted_obj.end(), [](GameObject* lhs, GameObject* rhs) { return lhs->render_layer < rhs->render_layer; });
	RefGlobal::CurrentGraphics->InterpolationMode = Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
	for (auto* obj : sorted_obj) {
		Drawer::SetRenderTarget(obj, Global::MainCamera);
		obj->Render();
	}
	if (DebugMode) {
		for (auto* obj : sorted_obj) {
			Drawer::SetRenderTarget(obj, Global::MainCamera);
			obj->collider.Render();
		}
		RefGlobal::CurrentGraphics->ResetTransform();
		Color textColor = { .2,.2,.2 };
		Drawer::AddText(textColor, to_string(Global::UpdatePerSecond) + " u/sec", { 0,0 });
		Drawer::AddText(textColor, Global::Player->position.to_string(), {0,15});
	}
	});