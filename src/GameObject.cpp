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
	Matrix2x2 ret;
	auto theta = rotation * DEG2RAD;
	ret.m00 = cos(theta);
	ret.m10 = sin(theta);
	ret.m01 = -ret.m10;
	ret.m11 = ret.m00;
	return ret;
}

GameObject::GameObject()
{
	m_GetInstances().insert(this);
	collider.gameObject = this;
}

GameObject::~GameObject()
{
	m_GetInstances().erase(this);
}