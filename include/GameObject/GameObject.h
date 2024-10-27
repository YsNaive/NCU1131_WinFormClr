#pragma once;
#include "Core.h"

class GameObject;
struct CollideInfo {
    bool    is_collide;
};

class Collider {
public:
    static vector<int>& GetIgnoreCollideList();
    static unordered_set<GameObject*> FindObject(const Circle& range, function<bool(GameObject*)> filter);
    static unordered_set<GameObject*> FindObject(const Circle& range);
    static void AddIgnore(const int lhs, const int rhs);
    static bool IsIgnore(GameObject* lhs, GameObject* rhs);
    GameObject* gameObject = nullptr;
    Rect boundingBox;
    vector<Polygon2D> hitboxes;
    vector<Polygon2D> hitboxes_world;
    void UpdateBoundingBox();
    void AddRect(const Rect& rect);
    void AddCircle(const Circle& circle);
    void Render();
    CollideInfo CollideWith(Collider& other);
    void Update();
};

class Rigidbody {
public:
    GameObject* gameObject = nullptr;
    float       decelerate = 0.9f;
    float       maxSpeed = 5.0f;
    Vector2     movement = { 0,0 };
    bool        relate_rotation = false;
    bool        enable = true;
    void Update();
    void AddForce(Vector2 force);
    void AddForce(Vector2 direction, float force);
    void AddForce(float rotation, float force);
};

class GameObject {
private:
    using InstancesTable = unordered_map<string, unordered_set<GameObject*>>;
    static       unordered_set<GameObject*>& m_GetInstances();
    //static       InstancesTable& m_GetInstancesTable();
protected:
    bool mark_destory = false;
    GameObject();
public:
    ~GameObject();
    static const unordered_set<GameObject*>& GetInstances();
    //static const InstancesTable& GetInstancesTable();
    inline virtual void Update() {};
    inline virtual void Render() {};
    inline virtual void OnCollide(GameObject* other, CollideInfo collideInfo) {};
    Tag tag;
    int render_layer = 0;
    inline bool is_Destroy() { return mark_destory; }
    void Destroy();

    Vector2 position = { 0,0 };
    float   rotation = 0;
    Matrix2x2 get_rotateMatrix();

    Collider collider;
    Rigidbody rigidbody;
};
