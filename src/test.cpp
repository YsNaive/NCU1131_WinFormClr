
#include "App.h"
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
	auto area = new EffectArea(numeric_limits<float>().max(), Color(.2, .2, 1, 0.15));
	area->modifiers.push_back({ &FrostModifier, 0.1 });
	area->collider.AddRect({ -100,-100,200,200 });

	for (int i = 0; i < 50; i++) {
		monster = new NormalMonster(Rand::RandomInt(10,20));
		monster->position.x += i * 1;
		monster->position.y += i * 1.01f;
		monster->entityInfo.MaxHp = 20;
		}
	});

auto test_update = 
Update::Create([]() {
	if (Global::GetKeyDown(Keys::E))
		Global::Player->Hp = 10;
	if (Global::GetKeyDown(Keys::Q))
		Global::Player->Hp = 100;
	});