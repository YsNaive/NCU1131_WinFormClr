#include "pch.h"
#include "Core.h"

unordered_set<GameObject*>& GameObject::m_GetInstances()
{
	static unordered_set<GameObject*> instances;
	return instances;
}
const unordered_set<GameObject*>& GameObject::GetInstances()
{
	return m_GetInstances();
}

void GameObject::Destory()
{
	mark_destory = true;
}

Matrix2x2 GameObject::get_rotateMatrix()
{
	return Matrix2x2::FromRotation(rotation);
}

GameObject::GameObject()
{
	m_GetInstances().insert(this);
	collider .gameObject = this;
	rigidbody.gameObject = this;
}

GameObject::~GameObject()
{
	m_GetInstances().erase(this);
}