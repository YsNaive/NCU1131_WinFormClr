#pragma once;

#include "Core.h"
#include "Window.h"

ref class RefGlobal {
public:
    static Timer^    MainTimer;
    static Window^   MainWindow;
    static Graphics^ CurrentGraphics;
};

extern class Player;
extern class Camera;
class Global {
public:

    static bool GetKey    (Keys keyCode);
    static bool GetKeyDown(Keys keyCode);
    static bool GetKeyUp  (Keys keyCode);
    // in MouseHandler
    static bool GetKey    (MouseButtons keyCode);
    static bool GetKeyDown(MouseButtons keyCode);
    static bool GetKeyUp  (MouseButtons keyCode);

    static float    Time;
    static float    DeltaTime;
    static float    TimeScale;
    static Vector2	ScreenSize;
    static Vector2	MousePosition;
    static float    MouseScroller;
    static int      UpdatePerSecond;
    static long long int UpdateCount;

    static Camera*  MainCamera;
    static Player*  Player;
};