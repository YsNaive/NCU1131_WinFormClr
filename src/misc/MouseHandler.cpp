#include "pch.h"
#include "MouseHandler.h"

#include "Global.h"
namespace {
	bool s_mouseState[2][3] = { false };
}

void InputEvt::OnMouseDown(Object^ sender, MouseEventArgs^ e)
{
	s_mouseState[(e->Button == MouseButtons::Left) ? 0 : 1][0] = true;
	s_mouseState[(e->Button == MouseButtons::Left) ? 0 : 1][1] = true;
}

void InputEvt::OnMouseUp(Object^ sender, MouseEventArgs^ e)
{
	s_mouseState[(e->Button == MouseButtons::Left) ? 0 : 1][1] = false;
	s_mouseState[(e->Button == MouseButtons::Left) ? 0 : 1][2] = true;
}

void InputEvt::OnMouseWheel(Object^ sender, MouseEventArgs^ e)
{
	Global::MouseScroller = e->Delta;
}

void InputEvt::ClearMouseInput()
{
	s_mouseState[0][0] = false;
	s_mouseState[0][2] = false;
	s_mouseState[1][0] = false;
	s_mouseState[1][2] = false;
}

bool Global::GetKey(MouseButtons keyCode)
{
	return s_mouseState[(keyCode == MouseButtons::Left) ? 0 : 1][1];
}

bool Global::GetKeyDown(MouseButtons keyCode)
{
	return s_mouseState[(keyCode == MouseButtons::Left) ? 0 : 1][0];
}

bool Global::GetKeyUp(MouseButtons keyCode)
{
	return s_mouseState[(keyCode == MouseButtons::Left) ? 0 : 1][2];
}