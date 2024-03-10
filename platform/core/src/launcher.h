#include "config.h"

#include <raylib.h>

// #pragma once

typedef struct Launcher_State Launcher_State;
struct Launcher_State
{
    Texture2D texture;
    AppConfiguration appConfig;
    int launcherDelay; // sec
    int framesCounter; // FPS
};

Launcher_State InitLauncher(AppConfiguration appConfig);

int UpdateLauncher(Launcher_State *state);

void DrawLauncher(Launcher_State *state);

void UnloadLauncher(Launcher_State *state);
