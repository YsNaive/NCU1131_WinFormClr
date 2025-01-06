#include "Core.h"
#include "Rand.h"
#include "Global.h"
#include "Player.h"
#include "Monster.h"

namespace {
	int normal_monster_count  = 5;
	int shooter_monster_count = 0;

	auto monster_generate_flow =
	Update::Create([]() {
		if (Global::Time < 180)
			normal_monster_count = 5 + (int)(Global::Time / 20.0);
	});

	auto monster_generate_control =
	Update::Create([]() {
		int size = GameObject::GetInstancesByType<NormalMonster>(Tag::Monster).size();
		while (size++ < normal_monster_count) {
			float size = Rand::Float(17.5, 22.5);
			auto monster = new NormalMonster(size);
			float rate = size / 17.5;
			monster->entityInfo_origin.Spd   *= 1.0 / rate;
			monster->entityInfo_origin.MaxHp *= rate;
			monster->Hp = monster->entityInfo_origin.MaxHp;
			monster->position = Global::Player->position;
			Vector2 offset = Vector2::FromDegree(Rand::Float(-180, 180));
			offset.set_length(500);
			monster->position += offset;
		}

		size = GameObject::GetInstancesByType<ShooterMonster>(Tag::Monster).size();
		while (size++ < shooter_monster_count) {
			float size = Rand::Float(17.5, 22.5);
			auto monster = new ShooterMonster(size);
			float rate = size / 17.5;
			monster->entityInfo_origin.Spd   *= 1.0 / rate;
			monster->entityInfo_origin.MaxHp *= rate;
			monster->Hp = monster->entityInfo_origin.MaxHp;
			monster->position = Global::Player->position;
			Vector2 offset = Vector2::FromDegree(Rand::Float(-180, 180));
			offset.set_length(500);
			monster->position += offset;
		}
	});
}