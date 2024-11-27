
#include "App.h"

namespace {
    Vector2 lastScreenSize = { 0,0 };
    UI_ProgressBar* UI_Hp;
    UI_ProgressBar* UI_Sp;
    UI_ProgressBar* UI_Mp;
    UI_ProgressBar* UI_Exp;

    UI_Text* UI_HP_Text;
    UI_Text* UI_SP_Text;
    UI_Text* UI_MP_Text;
}

auto game_ui_start =
Start::Create([]() {
    UI_Hp  = new UI_ProgressBar(0, 0, { .2,.1,.1 }, { 0.9,.25,.25 });
    UI_Sp  = new UI_ProgressBar(0, 0, { .1,.1,.2 }, { 0.25,.25,.9 });
    UI_Mp  = new UI_ProgressBar(0, 0, { .1,.2,.2 }, { 0.25,.9 ,.9 });
    UI_Exp = new UI_ProgressBar(0, 0, { .1,.2,.1 }, { 0.25,.9 ,.25});
    UI_HP_Text = new UI_Text("", Anchor::UpperRight);
    UI_HP_Text->color = Color(.7, .2, .2);
    UI_SP_Text = new UI_Text("", Anchor::UpperRight);
    UI_SP_Text->color = Color(.2, .2, .7);
    UI_MP_Text = new UI_Text("", Anchor::UpperRight);
    UI_MP_Text->color = Color(.2, .7, .7);
    });

auto game_ui_update =
Update::Create([]() {
    if (lastScreenSize != Global::ScreenSize) {
        lastScreenSize  = Global::ScreenSize;

        UI_Hp ->Bound = { 0,Global::ScreenSize.y - 25, Global::ScreenSize.x, 25 };
        UI_Sp ->Bound = { 0,Global::ScreenSize.y - 35, Global::ScreenSize.x, 10 };
        UI_Mp ->Bound = { 0,Global::ScreenSize.y - 45, Global::ScreenSize.x, 10 };
        UI_Exp->Bound = { 0,Global::ScreenSize.y - 55, Global::ScreenSize.x, 10 };

        UI_HP_Text->position = { Global::ScreenSize.x, 0 };
        UI_SP_Text->position = { Global::ScreenSize.x, 18 };
        UI_MP_Text->position = { Global::ScreenSize.x, 36 };
    }

    UI_Hp->MaxValue = Global::Player->entityInfo.MaxHp;
    UI_Hp->Value    = Global::Player->Hp;

    UI_Sp->MaxValue = Global::Player->entityInfo.MaxSp;
    UI_Sp->Value    = Global::Player->Sp;

    UI_Mp->MaxValue = Global::Player->entityInfo.MaxSp;
    UI_Mp->Value    = Global::Player->Mp;

    UI_Exp->MaxValue = Global::Player->LevelUpExp;
    UI_Exp->Value    = Global::Player->CurrentExp;

    UI_HP_Text->text = FormatString("HP %.1f", Global::Player->Hp);
    UI_SP_Text->text = FormatString("SP %.1f", Global::Player->Sp);
    UI_MP_Text->text = FormatString("MP %.1f", Global::Player->Mp);
    });