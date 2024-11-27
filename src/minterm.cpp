#include "App.h"

class RandomMonster : public Monster {
	float lastRandTime = 0;
public:
	RandomMonster(Vector2 center, float randRadius, float moveSpd) {
		position = {
			center.x + Rand::Float(-randRadius, randRadius),
			center.y + Rand::Float(-randRadius, randRadius)
		};
		rotation = Rand::Float(-180, 180);
		entityInfo_origin.Spd   = moveSpd;
		entityInfo_origin.MaxHp = 20;
		Hp = 20;

		collider.AddCircle({ {0,0}, 15 });
	}
	virtual void Update() {
		Entity::Update();
		if (Global::Time - lastRandTime > 0.75f) {
			rotation = Rand::Float(-180, 180);
			lastRandTime = Global::Time;
		}

		Vector2 dir = Vector2::FromDegree(rotation);
		dir.set_length(entityInfo.Spd);
		rigidbody.AddForce(dir * Global::DeltaTime);
	}
	//virtual void OnCollide(GameObject* other, CollideInfo collideInfo) override;
	virtual void OnDead() {
		Global::Player->ReciveExp(10);
		this->Destroy();
	}

	void Render() {
		Drawer::AddFillCircle(Color(.8, .25, .25), { {0,0}, 15 });
		Drawer::AddCircle(Color(.6, .2, .2), { {0,0}, 15 });

		float hit_dt = Global::Time - lastHitTime;
		const Rect hp_bar_bound = { -15,-25,30,7.5 };
		if (hit_dt < 3) {
			Drawer::AddRotation(-rotation);
			Color color = { 0.25,0.15,0.15, 1.0f };
			if (hit_dt > 2.5)
				color.a = 1.0f - ((hit_dt - 2.5f) / 0.5f);
			Drawer::AddFillRect(color, hp_bar_bound);
			Color color2 = { 0.8f, 0.4f, 0.4f, color.a };
			Rect hp_bar = hp_bar_bound;
			hp_bar.width *= Hp / entityInfo.MaxHp;
			Drawer::AddFillRect(color2, hp_bar);
			Drawer::AddRect(color, hp_bar_bound);
		}
	}

	float lastHitTime = -999;
	inline virtual void OnHit(const DamageInfo& info) {
		lastHitTime = Global::Time;
	}
};

class TrackingBullet : public Bullet {
	static const Circle range;
	float createdTime;
	float hitResetTime;
public:
	TrackingBullet(const BulletInfo* bulletInfo, const DamageInfo* damageInfo)
		: Bullet(bulletInfo, damageInfo) {
		collider.AddCircle(range);
		createdTime  = Global::Time;
		hitResetTime = Global::Time;
	}

	virtual void Update() override {
		GameObject* target = nullptr;
		float minDistance = numeric_limits<float>().max();
		for (auto* obj : GameObject::GetInstances()) {
			if (!obj->tag.Contains(Tag::Monster))
				continue;
			float distance = (obj->position - position).get_length();
			if (distance < minDistance) {
				target = obj;
				minDistance = distance;
			}
		}
		if (target != nullptr) {
			Vector2 force = target->position - this->position;
			force.set_length(bulletInfo->Speed * Global::DeltaTime);
			rigidbody.AddForce(force);
		}
		if (Global::Time - hitResetTime >= 1.0f) {
			hitResetTime += 1.0f;
			hit_history.clear();
		}
		if (Global::Time - createdTime > 5)
			Destroy();
	}
	virtual void Render() override {
		Drawer::AddFillCircle(Color(0.75f + (sin(Global::Time*5.0f) / 4.0f), .2f, .2f), range);
	}
};
const Circle TrackingBullet::range = { {0,0}, 10 };

#pragma region ChatGPT Generated

class Laser : public GameObject {
public:
	float duration = 3.0f; // Laser lasts for 3 seconds
	float elapsedTime = 0.0f;
	float range = 1000.0f; // Laser range
	bool has_hit = false;
	const DamageInfo* damageInfo; // Damage information for the laser

	Laser(const DamageInfo* damageInfo, Vector2 startPos, Vector2 direction)
		: damageInfo(damageInfo){
		collider.AddRect({ -2.5f, 0, 5, range });
		rotation = (atan2(direction.y, direction.x) * RAD2DEG) - 90.0f;
		position = startPos;
		tag.Add(Tag::Bullet);  // Use Tag::Bullet instead of Tag::Projectile
	}

	void Update() override {
		// Move the laser in the given direction
		if (elapsedTime < duration) {
			elapsedTime += Global::DeltaTime;
		}
		else {
			Destroy(); // Destroy the laser after the duration ends
		}
	}

	void Render() override {
		// Draw the laser as a red filled rectangle
		Drawer::AddFillPoly(Color(1.0f, 0.0f, 0.0f), collider.hitboxes[0]);
	}

	void OnCollide(GameObject* other, CollideInfo collideInfo) override {
		if (other->tag.Contains(Tag::Player) && !has_hit) {
			Entity* player = dynamic_cast<Entity*>(other);
			if (player) {
				damageInfo->Hit(player);
			}
			has_hit = true;
		}
	}
};

class LaserEnemy : public Monster {
private:
	Laser* activeLaser = nullptr; // The laser object
	float lastHitTime = -99;
public:
	static DamageInfo info_LaserDamage;
	LaserEnemy(Vector2 position) {
		this->position = position;
		collider.AddRect({ -20,-20,40,40 });
		entityInfo_origin.MaxHp = 50;
		Hp = 50;
		entityInfo.Spd = 3;
	}

	inline virtual void OnHit(const DamageInfo& info) {
		StartLaserAttack();
		lastHitTime = Global::Time;
	}

	void StartLaserAttack() {
		Vector2 direction = (Global::Player->position - position).normalize(); // Aim at the player
		new Laser(&info_LaserDamage, position, direction); // Create laser with appropriate damage
	}

	void Update() override {
		Entity::Update();
	}

	void Render() override {
		Drawer::AddFillRect(Color(0.45f, 0.2f, 0.2f), { -20,-20,40,40 });
		Drawer::AddRect	   (Color(0.2f, 0.1f, 0.1f) , { -20,-20,40,40 });

		float hit_dt = Global::Time - lastHitTime;
		const Rect hp_bar_bound = { -25,-25,50,7.5 };
		if (hit_dt < 3) {
			Drawer::AddRotation(-rotation);
			Color color = { 0.25,0.15,0.15, 1.0f };
			if (hit_dt > 2.5)
				color.a = 1.0f - ((hit_dt - 2.5f) / 0.5f);
			Drawer::AddFillRect(color, hp_bar_bound);
			Color color2 = { 0.8f, 0.4f, 0.4f, color.a };
			Rect hp_bar = hp_bar_bound;
			hp_bar.width *= Hp / entityInfo.MaxHp;
			Drawer::AddFillRect(color2, hp_bar);
			Drawer::AddRect(color, hp_bar_bound);
		}
	}

	void OnDead() override {
		// Handle death of the enemy (destroy laser if any)
		if (activeLaser != nullptr) {
			activeLaser->Destroy(); // Destroy the laser if it's active
		}
		Destroy(); // Destroy enemy
	}
};
DamageInfo LaserEnemy::info_LaserDamage = DamageInfo();
auto midterm_start_LaserEnemy_setup =
Start::Create([]() {
	LaserEnemy::info_LaserDamage.Damage = 20;
});
#pragma endregion


namespace {
	Vector2 lastScreenSize = {0,0};
	// [1.b] 2 weapon
	UI_Button* Btn_Shotgun;
	BulletInfo info_ShotgunBullet;
	DamageInfo info_ShotgunDamage;

	UI_Button* Btn_Rifle;
	BulletInfo info_RifleBullet;
	DamageInfo info_RifleDamage;

	auto midterm_start_weapon = 
	Start::Create([]() {
		Btn_Shotgun = new UI_Button("Shotgun");
		info_ShotgunBullet = BulletInfo();
		info_ShotgunBullet.Speed		   = 50 * 2;
		info_ShotgunBullet.DestroyDistance = 100;
		info_ShotgunBullet.IgnoreTag = Tag::Player;
		info_ShotgunDamage = DamageInfo();
		info_ShotgunDamage.Damage = 10;
		Btn_Shotgun->OnClick.push_back([&]() {
			if (Global::Player->Sp < 5)
				return;
			Global::Player->Sp -= 5;
			float dir = Global::Player->rotation - 50;
			for (int i = 0; i < 5; i++) {
				auto bullet = new Bullet(&info_ShotgunBullet, &info_ShotgunDamage);
				auto offset = Vector2::FromDegree(dir);
				offset.set_length(20);
				bullet->position = Global::Player->position + offset;
				bullet->rotation = dir;
				bullet->collider.AddRect({ -3,-10,6,20 });
				dir += 25;
			}
		});

		Btn_Rifle = new UI_Button("Rifle");
		info_RifleBullet = BulletInfo();
		info_RifleBullet.Speed			 = 100 * 2;
		info_RifleBullet.DestroyDistance = 400;
		info_RifleBullet.Penetrate	     = 99999;
		info_RifleBullet.IgnoreTag = Tag::Player;
		info_RifleDamage = DamageInfo();
		info_RifleDamage.Damage = 20;
		Btn_Rifle->OnClick.push_back([&]() {
			if (Global::Player->Sp < 20)
				return;
			Global::Player->Sp -= 20;
			float dir = Global::Player->rotation;
			auto bullet = new Bullet(&info_RifleBullet, &info_RifleDamage);
			auto offset = Vector2::FromDegree(dir);
			offset.set_length(20);
			bullet->position = Global::Player->position + offset;
			bullet->rotation = dir;
			bullet->collider.AddRect({ -5,-15,10,30 });
			});
	});

	// [1.c] 2 magic
	UI_Button* Btn_Heal;
	UI_Button* Btn_TrackingFire;
	BulletInfo info_TrackingBullet;
	DamageInfo info_TrackingDamage;
	auto midterm_start_magin = Start::Create([]() {
		Btn_Heal = new UI_Button("Heal");
		Btn_Heal->OnClick.push_back([&](){
			if (Global::Player->Mp < 5)
				return;
			Global::Player->Mp -= 5;
			Global::Player->Hp += 10;
		});

		Btn_TrackingFire = new UI_Button("Fire");
		info_TrackingBullet = BulletInfo();
		info_TrackingBullet.Speed = 10 * 2;
		info_TrackingBullet.DestroyDistance = numeric_limits<int>().max();
		info_TrackingBullet.IgnoreTag = Tag::Player;
		info_TrackingDamage = DamageInfo();
		info_TrackingDamage.Damage = 20;
		Btn_TrackingFire->OnClick.push_back([&]() {
			if (Global::Player->Mp < 10)
				return;
			Global::Player->Mp -= 10;
			TrackingBullet* bullet = new TrackingBullet(&info_TrackingBullet, &info_TrackingDamage);
			bullet->position = Global::Player->position;
			});
	});

	auto midterm_on_player_lv_up =
	OnLevelUp::Create([]() {
		info_ShotgunBullet.Speed  *= 2;
		info_RifleBullet.Speed    *= 2;
		info_TrackingBullet.Speed *= 2;
	});

	auto midterm_reset = 
	GameReset::Create([]() {
		// [1.a] 5 moving obstacles (HP=20) with random positions and velocities.
		for (int i = 0; i < 5; i++) {
			auto monster = new RandomMonster({ 0,0 }, 100, 5);
		}
		auto boss = new LaserEnemy({ 0,-100 });
	});

	auto midterm_update = 
	Update::Create([]() {
		if (lastScreenSize != Global::ScreenSize) {
			const Vector2 btn_size  = { 100, 25 };
			const float btn_padding = 10;
			Btn_Shotgun->bound = {
				Global::ScreenSize.x - btn_size.x - btn_padding,
				70,
				btn_size.x,
				btn_size.y
			};
			Btn_Shotgun->collider.hitboxes.clear();
			Btn_Shotgun->collider.AddRect(Btn_Shotgun->bound);

			Btn_Rifle->bound = Btn_Shotgun->bound;
			Btn_Rifle->bound.x -= btn_size.x + btn_padding;
			Btn_Rifle->collider.hitboxes.clear();
			Btn_Rifle->collider.AddRect(Btn_Rifle->bound);

			Btn_Heal->bound = Btn_Shotgun->bound;
			Btn_Heal->bound.y += btn_size.y + btn_padding;
			Btn_Heal->collider.hitboxes.clear();
			Btn_Heal->collider.AddRect(Btn_Heal->bound);

			Btn_TrackingFire->bound = Btn_Rifle->bound;
			Btn_TrackingFire->bound.y += btn_size.y + btn_padding;
			Btn_TrackingFire->collider.hitboxes.clear();
			Btn_TrackingFire->collider.AddRect(Btn_TrackingFire->bound);
		}
	});
	
	auto midterm_render = 
	Render::Create([]() {
		RefGlobal::CurrentGraphics->ResetTransform();
		Color color = { .15f,.15f,.15f };
		float y = 0;
		Drawer::AddText(color, "Press F5 to restart, WASD to move",				 { 0, y++ * 18 });
		Drawer::AddText(color, "Buttons at top right to use weapon and magic",   { 0, y++ * 18 });
		Drawer::AddText(color, "If Screen is stop, means you dead. pls restart", { 0, y++ * 18 });
	});
}