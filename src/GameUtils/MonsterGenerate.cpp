#include "Core.h"
#include "Rand.h"
#include "Global.h"
#include "Player.h"
#include "Monster.h"

namespace {
	auto monster_generate_control =
	Update::Create([]() {
		auto monster_instances = GameObject::GetInstances(Tag::Monster);
		if (Global::Time < 180) {
			int size = monster_instances.size();
			while (size++ < 25) {
				float size = Rand::Float(10, 20);
				auto monster = new NormalMonster(size);
				float rate = size / 10.0;
				monster->entityInfo_origin.Spd   *= 1.0 / rate;
				monster->entityInfo_origin.MaxHp *= rate;
				monster->Hp = monster->entityInfo_origin.MaxHp;
				monster->position = Global::Player->position;
				Vector2 offset = Vector2::FromDegree(Rand::Float(-180, 180));
				offset.set_length(500);
				monster->position += offset;
			}
		}
	});
}