#include "pch.h"
#include "Core.h"

unordered_set<int> Input::s_getKey;
unordered_set<int> Input::s_getKeyDown;
unordered_set<int> Input::s_getKeyUp;
bool Input::s_mouseState[2][3] = { false };

float Input::Time = 0;
Vector2	Input::ScreenSize;
Vector2	Input::MousePosition;
float	Input::MouseScroller;

bool Input::GetKey(Keys keyCode)
{
	return s_getKey.count((int)keyCode);
}

bool Input::GetKeyDown(Keys keyCode)
{
	return s_getKeyDown.count((int)keyCode);
}

bool Input::GetKeyUp(Keys keyCode)
{
	return s_getKeyUp.count((int)keyCode);
}

bool Input::GetKey(MouseButtons keyCode)
{
	return s_mouseState[(keyCode == MouseButtons::Left) ? 0 : 1][1];
}

bool Input::GetKeyDown(MouseButtons keyCode)
{
	return s_mouseState[(keyCode == MouseButtons::Left) ? 0 : 1][0];
}

bool Input::GetKeyUp(MouseButtons keyCode)
{
	return s_mouseState[(keyCode == MouseButtons::Left) ? 0 : 1][2];
}
