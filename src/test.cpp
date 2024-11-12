#include "App.h"
#include "GameManager.h"
#include "UI.h"

namespace {
	NormalMonster* monster;
}

EntityModifier FrostModifier(
	EntityModifierTypes::Frost,
	EntityModifierKey  ::Spd,
	EntityModifierOP   ::MUL,
	-0.5f);

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
	//return;
	int count = 0;
	for (auto obj : GameObject::GetInstances())
		if (obj->tag.Contains(Tag::Monster))
			count++;
	while (count++ < 20)
	{
		auto monster = new NormalMonster(Rand::Float(10, 20));
		monster->position  = { Rand::Float(200,450), Rand::Float(200,450) };
		monster->position += Global::Player->position;
	}
	});

auto test_render =
Render::Create([]() {

	});