#include "pch.h"
#include "Entity.h"

Entity::Entity()
{
}

void Entity::ReciveDamage(float value, GameObject* sender)
{
	if (hp == numeric_limits<float>().min())
		return;
	value -= defence;
	if (value < 0)
		return;

	hp -= value;
	OnHurt();
	if (hp < 0) {
		OnDead();
		hp = numeric_limits<float>().min();
	}
}