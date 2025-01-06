
#include "App.h"

namespace {
    Vector2 lastScreenSize = { 0,0 };
    UI_ProgressBar* UI_Hp;
    UI_ProgressBar* UI_Exp;

    UI_Text* UI_HP_Text;
    UI_Text* UI_Time;


    float           pauseAndShowText_Timer   = 0;
    UI_Text*        pauseAndShowText_Text    = nullptr;
    UI_Button*      pauseAndShowText_ExitBtn = nullptr;
    FuncGameObject* pauseAndShowText_Cover   = nullptr;
}

void PauseAndShowText(const string& text, bool show_exitBtn)
{
    pauseAndShowText_Timer = Global::RealTime;
    GameManager::Pause();
    pauseAndShowText_Text   ->text   = text;
    pauseAndShowText_Text   ->enable = true;
    pauseAndShowText_ExitBtn->enable = show_exitBtn;
    pauseAndShowText_Cover  ->enable = true;
}

auto game_ui_start =
Start::Create([]() {
    UI_Hp  = new UI_ProgressBar(0, 0, { .2,.1,.1 }, { 0.9,.25,.25 });
    UI_Exp = new UI_ProgressBar(0, 0, { .1,.2,.1 }, { 0.25,.9,.25 });
    UI_HP_Text = new UI_Text("", Anchor::UpperRight);
    UI_HP_Text->color = Color(.7, .2, .2);
    UI_Time = new UI_Text("", Anchor::MiddleCenter);
    UI_Time->size = Drawer::DefaultFontSize * 1.5f;
    UI_Time->color = Color(.2, .2, .2);

    // pauseAndShowText
    pauseAndShowText_Text = new UI_Text("none", Anchor::MiddleCenter);
    pauseAndShowText_Text->size = Drawer::DefaultFontSize * 2;
    pauseAndShowText_Text->render_layer = 100;
    pauseAndShowText_Text->tag.Add(Tag::DontDestroyOnReset);
    pauseAndShowText_ExitBtn = new UI_Button("Back");
    pauseAndShowText_ExitBtn->render_layer = 100;
    pauseAndShowText_ExitBtn->tag.Add(Tag::DontDestroyOnReset);
    pauseAndShowText_ExitBtn->OnClick.push_back([]() {
        pauseAndShowText_Text   ->enable = false;
        pauseAndShowText_ExitBtn->enable = false;
        pauseAndShowText_Cover  ->enable = false;
        });
    pauseAndShowText_Cover = new FuncGameObject(nullptr, []() {
        float a = Global::RealTime - pauseAndShowText_Timer;
        a /= 1.5f;
        a = clamp(0.0f, 0.75f, a);
        Drawer::AddFillRect(Color(0, 0, 0, a), { {0,0}, Global::ScreenSize });
        pauseAndShowText_Text->color = Color(.95, .95, .95, a);
        });
    pauseAndShowText_Cover->render_layer = 99;
    pauseAndShowText_Cover->tag.Add(Tag::DontDestroyOnReset | Tag::UI);
    pauseAndShowText_Text   ->enable = false;
    pauseAndShowText_ExitBtn->enable = false;
    pauseAndShowText_Cover  ->enable = false;
    });

auto game_ui_reset =
GameReset::Create([]() {
    pauseAndShowText_Text   ->enable = false;
    pauseAndShowText_ExitBtn->enable = false;
    pauseAndShowText_Cover  ->enable = false;
});

auto game_ui_update =
Update::Create([]() {
    if (lastScreenSize != Global::ScreenSize) {
        lastScreenSize  = Global::ScreenSize;
        Vector2 screenCenter = Global::ScreenSize / 2;
        UI_Hp ->Bound = { 0,Global::ScreenSize.y - 25, Global::ScreenSize.x, 25 };
        UI_Exp->Bound = { 0,Global::ScreenSize.y - 35, Global::ScreenSize.x, 10 };
        UI_HP_Text->position = { Global::ScreenSize.x, 0 };
        UI_Time->position = { screenCenter.x, 25 };

        pauseAndShowText_Text->position = screenCenter;
        pauseAndShowText_ExitBtn->SetBound({ screenCenter.x - 50,Global::ScreenSize.y - 60,100,40 });
    }

    UI_Hp->MaxValue = Global::Player->entityInfo.MaxHp;
    UI_Hp->Value    = Global::Player->Hp;

    UI_Exp->MaxValue = Global::Player->LevelUpExp;
    UI_Exp->Value    = Global::Player->CurrentExp;

    UI_HP_Text->text = FormatString("%.1f", Global::Player->Hp);
    UI_Time->text = FormatString("%d : %d", (int)(Global::Time / 60.0), ((int)Global::Time) % 60);
    });