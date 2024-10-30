#include "Entity.h"

#include "Global.h"

float& EntityInfo::operator[](int key)
{
	if (key == EntityModifierKey::MaxHp)
		return MaxHp;
	if (key == EntityModifierKey::MaxSp)
		return MaxSp;
	if (key == EntityModifierKey::RegHp)
		return RegHp;
	if (key == EntityModifierKey::RegSp)
		return RegSp;
	if (key == EntityModifierKey::Spd)
		return Spd;
	if (key == EntityModifierKey::Atk)
		return Atk;
	if (key == EntityModifierKey::Def)
		return Def;
	if (key == EntityModifierKey::Atk_M)
		return Atk_M;
	if (key == EntityModifierKey::Res_M)
		return Res_M;
	if (key == EntityModifierKey::Res_E)
		return Res_E;
	if (key == EntityModifierKey::AtkSpd)
		return AtkSpd;
	if (key == EntityModifierKey::DivDeg)
		return DivDeg;
	if (key == EntityModifierKey::CDR)
		return CDR;
	cout << "EntityInfo[] receive invalid key : [" << key << "]\n";
	Environment::Exit(1);
}

void EntityInfo::MakeValid()
{
	if (Spd < 1.0f) Spd = 1.0f;
	if (Atk < 0.0f) Atk = 0.0f;
	if (Def < 0.0f) Def = 0.0f;
	if (Atk_M  < 0.0f) Atk_M  = 0.0f;
	if (AtkSpd < 0.0f) AtkSpd = 0.0f;
	if (DivDeg < 0.0f) DivDeg = 0.0f;
	if (RegHp  < 0.0f) RegHp  = 0.0f;
	if (RegSp  < 0.0f) RegSp  = 0.0f;
	Res_M = clamp(0.0f, 1.0f, Res_M);
	Res_E = clamp(0.0f, 1.0f, Res_E);
	CDR   = clamp(0.0f, 1.0f, CDR  );
}

Entity::Entity()
{
	tag.Add(Tag::Entity);
}

void Entity::Update()
{
	entityInfo = entityInfo_origin;

	vector<EntityModifier*> toDelete;
	// <Type, <Key, value>>
	unordered_map<int, pair<int, float>> uniqueTable;
	for (auto& pair : entityModifiers) {
		auto* modifier = pair.first;
		auto& timer	   = pair.second;

		float effectTarget_origin = entityInfo_origin[modifier->Key];
		float effectValue = modifier->value;
		if (modifier->OP == EntityModifierOP::MUL)
			effectValue *= effectTarget_origin;
		else if (modifier->OP == EntityModifierOP::None)
			effectValue = 0;

		if (modifier->IsUnique()) {
			auto& tableValue = uniqueTable[modifier->GetPureType()];
			tableValue.first = modifier->Key;
			tableValue.second = max(tableValue.second, effectValue);
			continue;
		}
		entityInfo[modifier->Key] += effectValue;

		if (timer == numeric_limits<float>().max())
			continue;
		timer -= Global::DeltaTime;
		if (timer <= 0)
			toDelete.push_back(modifier);
	}
	for (auto& pair : uniqueTable)
		entityInfo[pair.second.first] += pair.second.second;
	for (auto& modifier : toDelete)
		entityModifiers.erase(modifier);

	entityInfo.MakeValid();
	Hp += entityInfo.RegHp * Global::DeltaTime;
	Sp += entityInfo.RegSp * Global::DeltaTime;

	if (Hp > entityInfo.MaxHp)
		Hp = entityInfo.MaxHp;
	if (Sp > entityInfo.MaxSp)
		Sp = entityInfo.MaxSp;
}

void Entity::AddModifier(EntityModifier* modifier, float addTime)
{
	if (entityModifiers.count(modifier))
		entityModifiers[modifier] += addTime;
	else
		entityModifiers[modifier] = addTime;
}

void Entity::SetModifier(EntityModifier* modifier, float setTime)
{
	entityModifiers[modifier] = setTime;
}

void Entity::RemoveModifier(EntityModifier* modifier)
{
	entityModifiers.erase(modifier);
}
