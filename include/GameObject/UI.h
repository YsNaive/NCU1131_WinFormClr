#pragma once;
#include "Core.h";
#include "GameObject.h";
#include "Camera.h"

extern class PlayerUpgrade;

void PauseAndShowText(const string& text);

class UI : public GameObject
{
public:
    inline UI() { tag.Add(Tag::UI | Tag::DontDestroyOnReset); render_layer = Layer::UI; rigidbody.enable = false; }
};

class UI_Text : public UI {
public:
    inline UI_Text(const string& text, Anchor anchor = Anchor::UpperLeft)
        : text(text), anchor(anchor) {}

    Anchor anchor;
    string text;
    Color color = Color(.1f, .1f, .1f);

    virtual void Render() override;
};

class Fade_Text : public UI_Text {
    bool is_init = false;
    float start_time = -1;
public:
    inline Fade_Text(const string& text, Anchor anchor = Anchor::UpperLeft)
        : UI_Text(text, anchor) {}

    float beginFadeTime = 0.35f;
    float fadeTime = 0.15f;

    void InitInWorld(Vector2 pos);
    virtual void Render() override;
};

class UI_Clickable : public UI {
public:
    inline UI_Clickable() { tag.Add(Tag::Clickable); }
    bool IsHovering = false;
    vector<function<void()>> OnClick;
};

class UI_ProgressBar : public UI {
private:
    float currentValue;
public:
    inline UI_ProgressBar(float min, float max, Color empty, Color fill)
        : MinValue(min), MaxValue(max), currentValue(min), Value(min), EmptyColor(empty), FillColor(fill) {}

    bool  IsHorizontal = true;
    bool  IsReverse    = false;
    float MinValue;
    float MaxValue;
    float Value;

    Color EmptyColor;
    Color FillColor;
    Rect  Bound;

    void Update() override;
    void Render() override;
};

class UI_Card : public UI_Clickable {
public:
    inline UI_Card(Vector2 size = { 180,300 })
        :size(size) { collider.AddRect({ {0,0}, size }); }

    string label;
    string description;

    const Vector2 size;
    float padding = 10;
    float hue = 0;

    float labelHeightPercent = 0.15f;

    void Render()  override;
    void AssignPlayerUpgrade(PlayerUpgrade& info);
};

class UI_UpgradeChoices : public UI {
public:

};