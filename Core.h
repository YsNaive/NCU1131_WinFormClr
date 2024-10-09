#pragma once
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

using namespace std;
using namespace callib;

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using System::Drawing::Pen;
using System::Drawing::Graphics;

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
    void* operator new(size_t size) {
        void* ptr = ::operator new(size);
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
class OnPaint    : public GlobalEvent<OnPaint   , void> {};

class GameObject {
protected:
    GameObject();
    ~GameObject();

private:
    static       unordered_set<GameObject*>& m_GetInstances();
public:
    static const unordered_set<GameObject*>& GetInstances();
    virtual void Update() = 0;
    virtual void Render() = 0;

    Vector2 position = { 0,0 };
    float   rotation = 0;
    Vector2 scale    = { 1,1 };
};

class Drawer {
public:
    static void SetPosition(Vector2 position);
    static void SetRotation(float   rotation);
    static void SetScale   (Vector2 scale);
    static void SetTransform(GameObject* obj);
    static void AddCircle(Color color, Vector2 center, float radius, float thickness = 1);
    static void AddRect(Color color, Rect rect, float thickness = 1);
};

class Camera : GameObject{
public:
    float size;

    Vector2 WorldToScreen(const Vector2& pos);
    Vector2 ScreenToWorld(const Vector2& pos);
    Rect    WorldToScreen(const Rect& rect);
    Rect    ScreenToWorld(const Rect& rect);

    void Update() override;
    void Render() override;
};

class Entity : public GameObject {
public:
    float max_hp;
    float min_hp;
    float hp;
};

class Player : public Entity {
public:
    void Update() override;
    void Render() override;
};

extern Vector2	  screenSize;
extern Vector2	  mousePosition;
extern set<char>  getKey;

extern Camera mainCamera;


//
//class Entity : public GameObject {
//public:
//    //Point position = Point(0, 0);
//    float rotate = 0;
//};
//
//class Player : public Entity {
//public:
//    void Update() override {
//
//    }
//};