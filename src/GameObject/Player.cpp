
#include "Player.h"

#include "Resources.h"
#include "GameManager.h"
#include "Global.h"
#include "Drawer.h"
#include "Bullet.h"

namespace {
	auto player_reset = GameReset::Create([]() {
		auto player = Global::Player;
		player->position = { 0,0 };
		player->rotation = 0;

		player->Level = 1;
		player->LevelUpExp = 30;
		player->CurrentExp = 0;

		player->entityModifiers.clear();

		player->entityInfo_origin.Spd = 15;
		player->entityInfo_origin.MaxHp = 100;
		player->entityInfo_origin.MaxSp = 100;
		player->entityInfo_origin.RegHp = 0;
		//player->entityInfo_origin.RegSp = 5;
		player->entityInfo_origin.DivDeg = 5;
		player->entityInfo_origin.Atk = 10;
		player->entityInfo_origin.Atk_M = 0;
		player->entityInfo_origin.AtkSpd = 1.25;
		player->entityInfo_origin.Def = 0;
		player->entityInfo_origin.Res_M = 0;
		player->entityInfo_origin.Res_E = 0;
		player->Hp = player->entityInfo_origin.MaxHp;
		player->Sp = player->entityInfo_origin.MaxSp;
		player->Mp = player->entityInfo_origin.MaxSp;
		player->Entity::Update();

		player->isDead = false;
		});
}

Player::Player()
{
	render_layer = Layer::Player;
	tag.Add(Tag::Player | Tag::DontDestroyOnReset);
	position = { 200,200 };
	rotation = 45;
	auto scale = 45.0f;
	collider.AddRect({ -scale / 2.0f, -scale / 2.0f , scale, scale });

}

void Player::ReciveExp(int value)
{
	CurrentExp += value;
	if (CurrentExp >= LevelUpExp) {
		Level++;
		CurrentExp -= LevelUpExp;
		entityInfo_origin.Spd *= 2;
		OnLevelUp::Invoke();

		auto text = new Fade_Text("Level UP!", Anchor::UpperCenter);
		text->color = { .2,.5,.2 };
		text->InitInWorld({ position.x, position.y - 45 });
	}
}

void Player::SetUsingWeapon(int index)
{
}

void Player::Update()
{
	Entity::Update();

	weapon_damageInfo[0] = DamageInfo::FromEntity(this);
	weapon_damageInfo[1] = DamageInfo::FromEntity(this);
	weapon_damageInfo[1].Damage *= 0.35;
	
	// movement
	float force = 0;
	if (Global::GetKey(Keys::W)) {
		force += entityInfo.Spd;
	}
	if (Global::GetKey(Keys::S)) {
		force -= entityInfo.Spd;
	}
	if (Global::GetKey(Keys::A)) {
		rotation -= 150 * Global::DeltaTime;
	}
	if (Global::GetKey(Keys::D)) {
		rotation += 150 * Global::DeltaTime;
	}
	rigidbody.AddForce(rotation, force * Global::DeltaTime);

	if (Global::GetKeyDown(Keys::D1))
		SetUsingWeapon(0);
	if (Global::GetKeyDown(Keys::D2))
		SetUsingWeapon(1);

	// attract exp
	for (auto exp : Collider::FindObject({ position, attractExpRange }, [](GameObject* m) {return m->tag.Contains(Tag::Exp); })) {
		auto attractForce = position - exp->position;
		attractForce.set_length(entityInfo.Spd * Global::DeltaTime * 2.0f);
		exp->rigidbody.AddForce(attractForce);
	}
}

void Player::Render()
{
	const float bodySize = 60;
	const float wheelExtend = 7.5;
	const Rect  body = { -bodySize / 2.1f, -bodySize / 2.1f , bodySize,bodySize };
	const Rect  gun = { -wheelExtend / 2.0, -bodySize*0.6f , wheelExtend, bodySize/2.0f };
	int i = 0;
	if(rigidbody.movement.get_length() > 1)
		i = ((int)(Global::Time * 6.0f)) % 2;
	Drawer::AddImage(RefResources::TankBodys[i], body);
	Drawer::AddFillRect(Color(.4, .4, .4), gun);
}

void Player::OnDead()
{
	GameManager::Pause();
	//auto msg = new UI_Text("Game Over !", Anchor::MiddleCenter);
	//msg->position = Global::ScreenSize / 4.0;
}
