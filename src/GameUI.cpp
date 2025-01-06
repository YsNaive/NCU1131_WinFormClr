
#include "App.h"

namespace {
    Vector2 lastScreenSize = { 0,0 };
    UI_ProgressBar* UI_Hp;
    UI_ProgressBar* UI_Exp;

    UI_Text* UI_HP_Text;
    UI_Text* UI_Time;


    float           pauseAndShowText_Timer = 0;
    UI_Text*        pauseAndShowText_Text  = nullptr;
    FuncGameObject* pauseAndShowText_Cover = nullptr;
}

void PauseAndShowText(const string& text)
{
    pauseAndShowText_Timer = Global::RealTime;
    GameManager::Pause();
}

auto game_ui_start =
Start::Create([]() {
    UI_Hp  = new UI_ProgressBar(0, 0, { .2,.1,.1 }, { 0.9,.25,.25 });
    UI_Exp = new UI_ProgressBar(0, 0, { .1,.2,.1 }, { 0.25,.9,.25 });
    UI_HP_Text = new UI_Text("", Anchor::UpperRight);
    UI_HP_Text->color = Color(.7, .2, .2);
    UI_Time = new UI_Text("", Anchor::MiddleCenter);
    UI_Time->color = Color(.2, .2, .2);

    // pauseAndShowText
    pauseAndShowText_Text = new UI_Text("none", Anchor::MiddleCenter);
    pauseAndShowText_Text->render_layer = 100;
    pauseAndShowText_Text->tag.Add(Tag::DontDestroyOnReset);
    pauseAndShowText_Cover = new FuncGameObject(nullptr, []() {
        Drawer::AddFillRect(Color(1, 1, 1), { {0,0}, Global::ScreenSize });
        });
    pauseAndShowText_Cover->render_layer = 99;
    pauseAndShowText_Cover->tag.Add(Tag::DontDestroyOnReset | Tag::UI);
    });

auto game_ui_update =
Update::Create([]() {
    if (lastScreenSize != Global::ScreenSize) {
        lastScreenSize  = Global::ScreenSize;

        UI_Hp ->Bound = { 0,Global::ScreenSize.y - 25, Global::ScreenSize.x, 25 };
        UI_Exp->Bound = { 0,Global::ScreenSize.y - 35, Global::ScreenSize.x, 10 };
        UI_HP_Text->position = { Global::ScreenSize.x, 0 };
        UI_Time->position = { Global::ScreenSize.x/2, 10 };

        pauseAndShowText_Text->position = Global::ScreenSize / 2;
    }

    UI_Hp->MaxValue = Global::Player->entityInfo.MaxHp;
    UI_Hp->Value    = Global::Player->Hp;

    UI_Exp->MaxValue = Global::Player->LevelUpExp;
    UI_Exp->Value    = Global::Player->CurrentExp;

    UI_HP_Text->text = FormatString("%.1f", Global::Player->Hp);
    UI_Time->text = FormatString("%d : %d", (int)(Global::Time / 60.0), ((int)Global::Time) % 60);
    });