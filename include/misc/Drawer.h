#pragma once

#include "Core.h"
#include "Global.h"
#include "GameObject.h"

namespace InputEvt {
    void OnPaint(Object^ sender, PaintEventArgs^ e);
}

ref class RefDrawer {
public:
    static Drawing::Font^       Font;
    static Pen^                 Pen;
    static SolidBrush^          SolidBrush;
    static LinearGradientBrush^ GradientBrush;

    static System::Drawing::Brush^ UsingBrush;

    static GraphicsPath^ PathBuffer;
    static void SetPathBuffer(Polygon2D& poly);
    static void SetStringBuffer(const string& str);

    static String^ StringBuffer;
    static Vector2 MeasureStringSize();
};

class Drawer {
public:
    static void AddPosition(Vector2 position);
    static void AddRotation(float   rotation);
    static void AddScale   (Vector2 scale);
    static void SetRenderTarget(GameObject* obj, Camera* camera);

    static void SetPen(Color color, float thickness);
    static void SetBrush(Color color);
    static void SetBrush(Color begColor, Color endColor, Vector2 begPos, Vector2 endPos);

    static void AddCircle    (Circle circle);
    static void AddCircle    (Color color, Circle circle, float thickness = 1);
    static void AddFillCircle(Circle circle);
    static void AddFillCircle(Color color, Circle circle);
    
    static void AddRect    (Rect rect);
    static void AddRect    (Color color, Rect rect, float thickness = 1);
    static void AddFillRect(Rect rect);
    static void AddFillRect(Color color, Rect rect);

    static void AddPoly    (Polygon2D& poly);
    static void AddPoly    (Color color, Polygon2D& poly, float thickness = 1);
    static void AddFillPoly(Polygon2D& poly);
    static void AddFillPoly(Color color, Polygon2D& poly);

    static void AddText(const string& text, Vector2 position, Anchor anchor = Anchor::UpperLeft);
    static void AddText(Color color, const string& text, Vector2 position, Anchor anchor = Anchor::UpperLeft);
    static void AddText(Color color, const string& text, Vector2 position, float textSize, Anchor anchor = Anchor::UpperLeft);
    
    static void AddImage(Image^ image, Rect position);
};