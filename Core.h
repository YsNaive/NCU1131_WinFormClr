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
using System::Drawing::Brush;
using System::Drawing::Graphics;

#define PI 3.14159265358979323846
#define DEG2RAD 0.01745329251
#define RAD2DEG 57.2957795457

const string Tag_Entity = "Player";
const string Tag_Player = "Player";

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

class Matrix2x2 {
public:
    inline Matrix2x2()
        :m00(0), m01(0), m10(0), m11(0) {}
    inline Matrix2x2(float m00, float m01, float m10, float m11)
        :m00(m00), m01(m01), m10(m10), m11(m11) {}

    float m00;
    float m01;
    float m10;
    float m11;

    inline friend Vector2 operator* (const Matrix2x2& matrix, const Vector2& vec){
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
    unordered_set<string> tags;
    inline void Add(string value) { tags.insert(value); }
    inline void Remove(string value) { tags.erase(value); }
    inline bool Contains(string value) { return tags.count(value); };
};

class GameObject;
struct CollideInfo {
    bool    is_collide;
    Vector2 collidePoint;
    Line2D  normalLine;
};
class Collider {
public:
    GameObject* gameObject = nullptr;
    vector<Rect> boxes;
    vector<Circle> circles;
    
    void Render();
    void CollideWith(Collider& other);
};

class GameObject {
protected:
    GameObject();
    ~GameObject();

private:
    static       unordered_set<GameObject*>& m_GetInstances();
public:
    static const unordered_set<GameObject*>& GetInstances();
    inline virtual void Update() {};
    inline virtual void Render() {};
    inline virtual void OnCollide(GameObject* other) {};
    Tag tag;
    int render_layer = 0;

    Vector2 position = { 0,0 };
    float   rotation = 0;
    Matrix2x2 get_rotateMatrix();

    Collider collider;
    bool has_rigidbody = false;
    bool CollideWith(GameObject* other);
};

class Drawer {
public:
    static void SetPosition(Vector2 position);
    static void SetRotation(float   rotation);
    static void SetScale   (Vector2 scale);
    static void SetTransform(GameObject* obj);
    static void AddCircle    (Color color, Circle circle, float thickness = 1);
    static void AddFillCircle(Color color, Circle circle);
    static void AddRect    (Color color, Rect rect, float thickness = 1);
    static void AddFillRect(Color color, Rect rect);
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
    Entity();
    float max_hp;
    float min_hp;
    float hp;
};

class Player : public Entity {
public:
    static Player* instance;
    Player();
    float maxSpeed = 4;
    float curSpeed = 0;
    void Update() override;
    void Render() override;
};

class Obstacle : public GameObject {
public:
    void Render() override;
    void OnCollide(GameObject* other);
};

extern Vector2	screenSize;
extern Vector2	mousePosition;
extern set<int> getKey;
extern set<int> getKeyDown;
extern set<int> getKeyUp;
extern bool DebugMode;

bool GetKey    (Keys keyCode);
bool GetKeyDown(Keys keyCode);
bool GetKeyUp  (Keys keyCode);

extern Camera mainCamera;
