#pragma once;
#include <iostream>
#include <math.h>
#include <map>
#include <set>
#include <string>
#include <functional>
#include <unordered_set>
#include <msclr/marshal_cppstd.h>
#include <unordered_map>
#include "callib.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;

using namespace std;
using namespace callib;

using System::Drawing::Graphics;
using System::Drawing::Pen;
using System::Drawing::Brush;
using System::Drawing::SolidBrush;
using System::Drawing::Drawing2D::LinearGradientBrush;
using System::Drawing::Drawing2D::GraphicsPath;

#define PI 3.14159265358979323846
#define DEG2RAD 0.01745329251
#define RAD2DEG 57.2957795457

#undef min;
#undef max;

template<class Inherit_T, class Ret_T, class... Args_T>
class GlobalEvent {
    using func_T = function<Ret_T(Args_T...)>;
protected:
    static unordered_set<Inherit_T*>& _get_invoke_set() {
        static unordered_set<Inherit_T*> set;
        return set;
    }

    func_T m_func = nullptr;
    GlobalEvent() {}
public:
    static Inherit_T* Create(func_T func) {
        if (func == nullptr) {
            cout << "Given func can't be null.\n";
            abort();
        }
        Inherit_T* obj = new Inherit_T();
        obj->m_func = func;
        return obj;
    }
    static Ret_T Invoke(Args_T... args) {
        for (Inherit_T* func : _get_invoke_set())
            func->m_func(args...);
    }
    void* operator new(size_t scale) {
        void* ptr = ::operator new(scale);
        _get_invoke_set().insert((Inherit_T*)ptr);
        return ptr;
    }
    void  operator delete(void* ptr) {
        _get_invoke_set().erase((Inherit_T*)ptr);
        ::operator delete(ptr);
    }
};
class Start      : public GlobalEvent<Start     , void> {};
class PreUpdate  : public GlobalEvent<PreUpdate , void> {};
class Update     : public GlobalEvent<Update    , void> {};
class LateUpdate : public GlobalEvent<LateUpdate, void> {};
class Render     : public GlobalEvent<Render    , void> {};
class GameReset  : public GlobalEvent<GameReset , void> {};

class Matrix2x2 {
public:
    inline Matrix2x2()
        :m00(0), m01(0), m10(0), m11(0) {}
    inline Matrix2x2(float m00, float m01, float m10, float m11)
        : m00(m00), m01(m01), m10(m10), m11(m11) {}

    static Matrix2x2 FromRotation(float degree);

    float m00;
    float m01;
    float m10;
    float m11;

    inline friend Vector2 operator* (const Matrix2x2& matrix, const Vector2& vec) {
        return Vector2(matrix.m00 * vec.x + matrix.m01 * vec.y, matrix.m10 * vec.x + matrix.m11 * vec.y);
    }
};

class Circle {
public:
    inline Circle(Vector2 center, float radius)
        : center(center), radius(radius) {}
    Vector2 center;
    float radius;
};

class Tag {
public:
    static const int None      = 0;
    static const int Entity    = 1;
    static const int Player    = 2;
    static const int Monster   = 4;
    static const int Bullet    = 8;
    static const int Exp       = 16;
    static const int UI        = 32;
    static const int Clickable = 64;

    int flag = 0;
    inline void Add(const int value) { flag |= value; }
    inline void Remove(const int value) { flag &= ~value; }
    inline bool Contains(const int value) { return (flag & value) == value; };
    inline bool Any(const int value) { return (flag & value) != 0; };
};

class Rarity {
public:
    static constexpr int None   =  0;
    static constexpr int Common =  1;
    static constexpr int Rare   =  2;
    static constexpr int Epic   =  3;
    static constexpr int Legend =  4;

    static float GetColorHue(int rarity);
};

class Layer {
public:
    static constexpr int UI      =  99;
    static constexpr int Player  =  0;
    static constexpr int Exp     =  1;
    static constexpr int Monster = -1;
};

class Anchor {
private:
    inline Anchor(int value)
        : value(value) {}
    int value;

public:
    static const Anchor UpperLeft   ;
    static const Anchor UpperCenter ;
    static const Anchor UpperRight  ;
    static const Anchor MiddleLeft  ;
    static const Anchor MiddleCenter;
    static const Anchor MiddleRight ;
    static const Anchor LowerLeft   ;
    static const Anchor LowerCenter ;
    static const Anchor LowerRight  ;

    Vector2 GetPosition(const Rect& rect);

    inline bool operator== (Anchor other) { return value == other.value; }
    inline bool operator!= (Anchor other) { return value != other.value; }
};

