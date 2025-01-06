#include "Core.h"
#include "Rand.h"
#include "Global.h"
#include "Player.h"
#include "Monster.h"

namespace {
	int normal_monster_count  = 5;
	int shooter_monster_count = 0;
	int heal_monster_count    = 0;

	void apply_time_effect(Monster* monster) {
		float r = 1.0 + Global::Time / 150.0f;
		monster->entityInfo_origin.MaxHp *= r;
		monster->entityInfo_origin.Atk   *= r;
		monster->entityInfo_origin.Spd   *= 1.0 + Global::Time / 300.0f;
		monster->Hp = monster->entityInfo_origin.MaxHp;
	}

	auto monster_generate_flow =
	Update::Create([]() {
		normal_monster_count = 5 + (int)((Global::Time + 1) / 20.0);
		if (Global::Time > 120)
			shooter_monster_count = (int)((Global::Time - 119) / 20.0);
		if (Global::Time > 240)
			heal_monster_count    = (int)((Global::Time - 239) / 20.0);

		ShooterMonster::DamageInfo.Damage = (Global::Time / 150.0f) * 20;
		ShooterMonster::BulletInfo.Speed  = (Global::Time / 300.0f) * 150;
		HealMonster::Modifier.value = (Global::Time / 300.0f) * 20;
	});

	auto monster_generate_control =
	Update::Create([]() {
		int count = GameObject::GetInstancesByType<NormalMonster>(Tag::Monster).size();
		while (count++ < normal_monster_count) {
			float size = Rand::Float(17.5, 22.5);
			auto monster = new NormalMonster(size);
			float rate = size / 17.5;
			monster->entityInfo_origin.Spd   *= 1.0 / rate;
			monster->entityInfo_origin.MaxHp *= rate;
			apply_time_effect(monster);
			monster->RandomLocateOnPlayer();
		}

		count = GameObject::GetInstancesByType<ShooterMonster>(Tag::Monster).size();
		while (count++ < shooter_monster_count) {
			float size = Rand::Float(17.5, 22.5);
			auto monster = new ShooterMonster(size);
			float rate = size / 17.5;
			monster->entityInfo_origin.Spd   *= 1.0 / rate;
			monster->entityInfo_origin.MaxHp *= rate;
			apply_time_effect(monster);
			monster->RandomLocateOnPlayer();
		}

		count = GameObject::GetInstancesByType<HealMonster>(Tag::Monster).size();
		while (count++ < heal_monster_count) {
			float size = Rand::Float(17.5, 22.5);
			auto monster = new HealMonster(size);
			float rate = size / 17.5;
			monster->entityInfo_origin.Spd *= 1.0 / rate;
			monster->entityInfo_origin.MaxHp *= rate;
			apply_time_effect(monster);
			monster->RandomLocateOnPlayer();
		}
	});
}