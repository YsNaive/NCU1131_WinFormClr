
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
		player->LevelUpExp = 5;
		player->CurrentExp = 0;

		player->entityModifiers.clear();

		player->entityInfo_origin.Spd = 15;
		player->entityInfo_origin.MaxHp = 100;
		player->entityInfo_origin.MaxSp = 100;
		player->entityInfo_origin.RegHp = 0;
		player->entityInfo_origin.RegSp = 5;
		player->entityInfo_origin.DivDeg = 5;
		player->entityInfo_origin.Atk = 10;
		player->entityInfo_origin.Atk_M = 0;
		player->entityInfo_origin.AtkSpd = 1.25;
		player->entityInfo_origin.Def = 0;
		player->entityInfo_origin.Res_M = 0;
		player->entityInfo_origin.Res_E = 0;
		player->Hp = player->entityInfo_origin.MaxHp;
		player->Sp = 0;
		player->Entity::Update();

		player->bulletGenerator->WavePerShoot = 1;
		player->bulletGenerator->BulletWave = { 0 };

		player->weapon_bulletInfo[0] = BulletInfo::DefaultPlayer;
		auto info = BulletInfo(0, 999, 1, Tag::Player);
		info.DestroyTimeSec = 0.25;
		player->weapon_bulletInfo[1] = info;
		player->SetUsingWeapon(0);
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

	weapon_CreateBullet[0] = [&]() {
		auto bullet = new Bullet(&weapon_bulletInfo[0], &weapon_damageInfo[0]);
		bullet->collider.AddRect({ -3,-5,6,10 });
		return bullet;
		};
	weapon_CreateBullet[1] = [&]() {
		auto bullet = new Bullet(&weapon_bulletInfo[1], &weapon_damageInfo[1]);
		bullet->collider.AddRect({ -5,-2000,10,2000 });
		bullet->render_layer = -99;
		return bullet;
		};
	weapon_SpPerSec[0] = 2.5;
	weapon_SpPerSec[1] = 15;
	bulletGenerator = new BulletGenerator(this, weapon_CreateBullet[1]);
	bulletGenerator->tag.Add(Tag::DontDestroyOnReset);
}

void Player::ReciveExp(int value)
{
	CurrentExp += value;
	if (CurrentExp >= LevelUpExp) {
		Level++;
		CurrentExp -= LevelUpExp;
		LevelUpExp += 1;

		// do level up
		GameManager::Pause();
		entityInfo_origin.Atk   += 1;
		entityInfo_origin.MaxHp += 10;
		this->Entity::Update();
		Hp = entityInfo.MaxHp;

		Vector2 cardSize = { 180,300 };
		Vector2 centerPos = Global::ScreenSize / 2.0 - cardSize / 2.0;
		UI_Card* cards[] = {new UI_Card(cardSize) ,new UI_Card(cardSize) ,new UI_Card(cardSize)};
		auto closeFunc = [cards]() {
			cards[0]->Destroy();
			cards[1]->Destroy();
			cards[2]->Destroy();
			GameManager::Resume();
		};
		for (auto* card : cards) {
			card->position = centerPos;
			card->OnClick.push_back(closeFunc);
			card->AssignPlayerUpgrade(Rand::PlayerUpgrade());
		}
		cards[0]->position.x -= cardSize.x * 1.5f;
		cards[2]->position.x += cardSize.x * 1.5f;
	}
}

void Player::SetUsingWeapon(int index)
{
	using_weapon = index;
	bulletGenerator->CreateBullet = weapon_CreateBullet[index];
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

	float currCost = weapon_SpPerSec[using_weapon] * Global::DeltaTime;
	bulletGenerator->enable = Sp > currCost && Global::GetKey(MouseButtons::Left);
	if (bulletGenerator->enable) {
		Sp -= currCost;
	}

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
