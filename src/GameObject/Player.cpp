
#include "Player.h"

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

		player->damageInfo = DamageInfo::FromEntity(player);
		player->bulletInfo = BulletInfo::DefaultPlayer;

		});
}

Player::Player()
{
	render_layer = Layer::Player;
	tag.Add(Tag::Player);
	position = { 200,200 };
	rotation = 45;
	auto scale = 35.0f;
	collider.AddRect({ -scale / 2.0f, -scale / 2.0f , scale, scale });


	bulletGenerator = new BulletGenerator(this, [&]() {
		auto bullet = new Bullet(&bulletInfo, &damageInfo);
		bullet->collider.AddRect({ -3,-5,6,10 });
		return bullet;
		});
	bulletGenerator->BulletWave.push_back(0);
	bulletGenerator->BulletWave.push_back(180);
	bulletGenerator->WavePerShoot = 5;
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

void Player::Update()
{
	Entity::Update();
	damageInfo = DamageInfo::FromEntity(this);
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

	// attract exp
	for (auto exp : Collider::FindObject({ position, attractExpRange }, [](GameObject* m) {return m->tag.Contains(Tag::Exp); })) {
		auto attractForce = position - exp->position;
		attractForce.set_length(entityInfo.Spd * Global::DeltaTime * 2.0f);
		exp->rigidbody.AddForce(attractForce);
	}
}

void Player::Render()
{
	const float bodySize = 40;
	const float wheelExtend = 5;
	const Rect  body = { -bodySize / 2.0f, -bodySize / 2.0f , bodySize,bodySize };
	const Rect  leftWheel = { body.x - wheelExtend, body.y - wheelExtend , wheelExtend * 2,bodySize + wheelExtend * 2 };
	const Rect  rightWheel = { body.get_xMax() - wheelExtend, body.y - wheelExtend , leftWheel.width, leftWheel.height };
	const Rect  gun = { -wheelExtend / 2.0, -bodySize , wheelExtend, bodySize };

	Drawer::AddFillRect(Color(0, 0, 0), leftWheel);
	Drawer::AddFillRect(Color(0, 0, 0), rightWheel);
	Drawer::AddFillRect(Color(.2, .2, .2), body);
	Drawer::AddFillRect(Color(.4, .4, .4), gun);
}

void Player::OnDead()
{
	GameManager::Pause();
	auto msg = new UI_Text("Game Over !", Anchor::MiddleCenter);
	msg->position = Global::ScreenSize / 4.0;
}
