#pragma once

#include "Core.h"
#include "Global.h"
#include "GameObject.h"

namespace InputEvt {
    void OnPaint(Object^ sender, PaintEventArgs^ e);
}

class Drawer {
public:
    static void AddPosition(Vector2 position);
    static void AddRotation(float   rotation);
    static void AddScale   (Vector2 scale);
    static void SetRenderTarget(GameObject* obj, Camera* camera);
    static void AddCircle    (Color color, Circle circle, float thickness = 1);
    static void AddFillCircle(Color color, Circle circle);
    static void AddRect    (Color color, Rect rect, float thickness = 1);
    static void AddFillRect(Color color, Rect rect);
    static void AddPoly    (Color color, Polygon2D& poly, float thickness = 1);
    static void AddFillPoly(Color color, Polygon2D& poly);
    static void AddText(Color color,const string& text, Vector2 position, Anchor anchor = Anchor::UpperLeft, float textSize = 14);
};