#include "Core.h"
#include "Rand.h"
#include "Global.h"
#include "Player.h"
#include "Monster.h"

namespace {
	auto monster_generate_control =
	Update::Create([]() {
		if (Global::Time < 180) {
			int size = GameObject::GetInstancesByType<NormalMonster>(Tag::Monster).size();
			while (size++ < 15) {
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
			while (size++ < 5) {
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
		}
	});
}