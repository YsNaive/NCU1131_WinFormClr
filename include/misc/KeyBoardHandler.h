#pragma once
#include "Core.h"

namespace InputEvt {
    void OnKeyDown(Object^ sender, KeyEventArgs^ e);
    void OnKeyUp(Object^ sender, KeyEventArgs^ e);

    void ClearKeyBoardInput();
}