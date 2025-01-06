#include "Bullet.h"

#include "Drawer.h"
#include "Entity.h"

const BulletInfo BulletInfo::DefaultPlayer  = BulletInfo(500, 0, 1000, Tag::Player);
const BulletInfo BulletInfo::DefaultMonster = BulletInfo(500, 0, 1000, Tag::Monster);

void Bullet::Update()
{
	auto offset = Vector2::FromDegree(rotation);
	offset.set_length(bulletInfo->Speed * Global::DeltaTime);
	position += offset;
	movedDistance += bulletInfo->Speed * Global::DeltaTime;
	aliveTime += Global::DeltaTime;
	if (movedDistance > bulletInfo->DestroyDistance)
		this->Destroy();
	if (aliveTime	  > bulletInfo->DestroyTimeSec)
		this->Destroy();
}

void Bullet::Render()
{
	for (auto& poly : collider.hitboxes) {
		Drawer::AddFillPoly(Color::FromHSV(color_h, 0.5, 0.75), poly);
		Drawer::AddPoly(Color::FromHSV(color_h, 0.5, 0.5), poly);
	}
}

void Bullet::OnCollide(GameObject* other, CollideInfo collideInfo)
{
	if (other->tag.Any(bulletInfo->IgnoreTag))
		return;
	if (find(hit_history.begin(), hit_history.end(), other) != hit_history.end())
		return;
	auto entity = dynamic_cast<Entity*>(other);
	if (!entity)
		return;

	damageInfo->Hit(entity);
	hit_history.push_back(entity);
	if (hit_history.size() > bulletInfo->Penetrate)
		this->Destroy();
}
