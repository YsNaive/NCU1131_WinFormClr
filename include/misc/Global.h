#pragma once;

#include "Core.h"
#include "Window.h"
extern class Player;
extern class Camera;

ref class RefGlobal {
public:
    static Timer^    MainTimer;
    static Window^   MainWindow;
    static Graphics^ CurrentGraphics;
};

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
    static float    RealTime;
    static float    DeltaTime;
    static float    RealDeltaTime;
    static float    TimeScale;
    static Vector2	ScreenSize;
    static float	ScreenDiagonal;
    static Vector2	MousePosition;
    static float    MouseScroller;
    static int      UpdatePerSecond;
    static long long int UpdateCount;

    static Camera*  MainCamera;
    static Player*  Player;
};