#include "GameManager.h"

#include "Global.h"
#include "Player.h"
#include "Camera.h"

namespace {
    float timeScaleCache = 0;
}

void GameManager::Pause()
{
    if (Global::TimeScale == 0)
        return;
    timeScaleCache = Global::TimeScale;
    Global::TimeScale = 0;
}

void GameManager::Resume()
{
    Global::TimeScale = timeScaleCache;
}

void GameManager::Reset()
{
    Pause();
    GameReset::Invoke();
    Global::MainCamera->position = Global::Player->position;

    for (auto& obj : GameObject::GetInstances()) {
        if (!obj->tag.Contains(Tag::DontDestroyOnReset))
            obj->Destroy();
    }
}
