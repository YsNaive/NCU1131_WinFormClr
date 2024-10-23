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

const string Tag_Entity  = "Entity";
const string Tag_Player  = "Player";
const string Tag_Monster = "Monster";
const string Tag_Bullet  = "Bullet";

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

class Input {
public:
    static unordered_set<int> s_getKey;
    static unordered_set<int> s_getKeyDown;
    static unordered_set<int> s_getKeyUp;
    static bool s_mouseState[2][3]; // [left/right] [down/hold/up]

    static bool GetKey    (Keys keyCode);
    static bool GetKeyDown(Keys keyCode);
    static bool GetKeyUp  (Keys keyCode);
    static bool GetKey    (MouseButtons keyCode);
    static bool GetKeyDown(MouseButtons keyCode);
    static bool GetKeyUp  (MouseButtons keyCode);

    static float Time;
    static Vector2	ScreenSize;
    static Vector2	MousePosition;
    static float MouseScroller;
};

class GameObject;
struct CollideInfo {
    bool    is_collide;
};
class Collider {
public:
    GameObject* gameObject = nullptr;
    vector<Rect> boxes;
    vector<Circle> circles;
    
    void Render();
    CollideInfo CollideWith(Collider& other);
    vector<Polygon2D> GetWorldPositionBoxes();
    vector<Circle> GetWorldPositionCircles();
};
class Rigidbody {
public:
    GameObject* gameObject      = nullptr;
    float       decelerate      = 0.9f;
    float       maxSpeed        = 5.0f;
    Vector2     movement        = { 0,0 };
    bool        relate_rotation = false;
    bool        enable          = true;
    void Update();
    void AddForce(Vector2 force);
    void AddForce(Vector2 direction, float force);
    void AddForce(float rotation, float force);
};

class GameObject {
private:
    static       unordered_set<GameObject*>& m_GetInstances();
protected:
    bool mark_destory = false;
    GameObject();
public:
    ~GameObject();
    static const unordered_set<GameObject*>& GetInstances();
    inline virtual void Update() {};
    inline virtual void Render() {};
    inline virtual void OnCollide(GameObject* other,CollideInfo collideInfo) {};
    Tag tag;
    int render_layer = 0;
    inline bool is_Destory() { return mark_destory; }
    void Destory();

    Vector2 position = { 0,0 };
    float   rotation = 0;
    Matrix2x2 get_rotateMatrix();

    Collider collider;
    Rigidbody rigidbody;
};

class Camera : public GameObject{
public:
    float scale = 1;
    float targetScale = 1;

    Vector2 WorldToScreen(const Vector2& pos);
    Vector2 ScreenToWorld(const Vector2& pos);
    Rect    WorldToScreen(const Rect& rect);
    Rect    ScreenToWorld(const Rect& rect);

    void Update() override;
    void Render() override;
};

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
    static void AddText(Color color,const string& text, Vector2 position, float textSize = 14);
};

class Entity : public GameObject {
public:
    Entity();
    float max_hp;
    float min_hp;
    float hp;
    float defence;
    float speed;

    void ReciveDamage(float value, GameObject* sender);
    inline void SendDamage(float value, Entity* reciver) { reciver->ReciveDamage(value, this); };
    inline virtual void OnDead() {}
    inline virtual void OnHurt() {}
};

class Bullet : public GameObject {
public:
    Bullet(float direction, float speed, float destoryDistance = 200);
    float direction;
    float speed;
    float destoryDistance;
    float movedDistance = 0;

    void Update() override;
    void Render() override;
};

class Player : public Entity {
public:
    static Player* instance;
    Player();
    void Update() override;
    void Render() override;
};

class Monster : public Entity {
public:
    Monster();
    virtual void Update() override;
    virtual void OnCollide(GameObject* other, CollideInfo collideInfo) override;
};

class NormalMonster : public Monster {
public:
    NormalMonster();
    void Render() override;
};

class Obstacle : public GameObject {
public:
    void Render() override;
    void OnCollide(GameObject* other, CollideInfo info);
};

extern bool DebugMode;

extern Camera mainCamera;
