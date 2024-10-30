
#include "App.h"

namespace {
    Vector2 lastScreenSize = { 0,0 };
    UI_ProgressBar* UI_Hp;
    UI_ProgressBar* UI_Sp;
    UI_ProgressBar* UI_Exp;
}

auto game_ui_start =
Start::Create([]() {
    UI_Hp  = new UI_ProgressBar(0, 0, { .2,.1,.1 }, { 0.9,.25,.25 });
    UI_Sp  = new UI_ProgressBar(0, 0, { .1,.1,.2 }, { 0.25,.25,.9 });
    UI_Exp = new UI_ProgressBar(0, 0, { .1,.2,.1 }, { 0.25,.9,.25 });
    });

auto game_ui_update =
Update::Create([]() {
    if (lastScreenSize != Global::ScreenSize) {
        lastScreenSize  = Global::ScreenSize;

        UI_Hp ->Bound = { 0,Global::ScreenSize.y - 25, Global::ScreenSize.x, 25 };
        UI_Sp ->Bound = { 0,Global::ScreenSize.y - 35, Global::ScreenSize.x, 10 };
        UI_Exp->Bound = { 0,Global::ScreenSize.y - 45, Global::ScreenSize.x, 10 };
    }

    UI_Hp->MaxValue = Global::Player->entityInfo.MaxHp;
    UI_Hp->Value    = Global::Player->Hp;

    UI_Sp->MaxValue = Global::Player->entityInfo.MaxSp;
    UI_Sp->Value    = Global::Player->Sp;

    UI_Exp->MaxValue = Global::Player->LevelUpExp;
    UI_Exp->Value    = Global::Player->CurrentExp;
    });