#pragma once

#include "pch.h"
#include "Core.h"

namespace InputEvt{
    void OnMouseDown (Object^ sender, MouseEventArgs^ e);
    void OnMouseUp   (Object^ sender, MouseEventArgs^ e);
    void OnMouseWheel(Object^ sender, MouseEventArgs^ e);

    void ClearMouseInput();
}