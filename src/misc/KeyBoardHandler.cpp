#include "pch.h"
#include "KeyBoardHandler.h"

#include <unordered_set>
#include "Global.h"
namespace {
	std::unordered_set<int> s_getKey;
	std::unordered_set<int> s_getKeyUp;
	std::unordered_set<int> s_getKeyDown;
}

void InputEvt::OnKeyDown(Object^ sender, KeyEventArgs^ e)
{
	if (!s_getKey.count(e->KeyValue)) 
		s_getKeyDown.insert(e->KeyValue);
	s_getKey.insert(e->KeyValue);
}

void InputEvt::OnKeyUp(Object^ sender, KeyEventArgs^ e)
{
	s_getKey.erase(e->KeyValue);
	s_getKeyUp.insert(e->KeyValue);
}

void InputEvt::ClearKeyBoardInput()
{
	s_getKeyUp.clear();
	s_getKeyDown.clear();
}

bool Global::GetKey(Keys keyCode)
{
	return s_getKey.count((int)keyCode);
}

bool Global::GetKeyDown(Keys keyCode)
{
	return s_getKeyDown.count((int)keyCode);
}

bool Global::GetKeyUp(Keys keyCode)
{
	return s_getKeyUp.count((int)keyCode);
}