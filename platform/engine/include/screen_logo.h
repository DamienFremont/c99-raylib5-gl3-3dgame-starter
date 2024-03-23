#include "config.h"
#include <raylib.h>
#include <time.h>

typedef struct Launcher_State
{
    AppConfiguration appConfig;
    Texture2D screenTexture;
    int counterInMs;
    clock_t startClock;
    float screenTextureScale;
    int logoPositionX;
} Launcher_State;

Launcher_State InitLauncher(AppConfiguration appConfig);

int UpdateLauncher(Launcher_State *state);

void DrawLauncher(Launcher_State *state);

void UnloadLauncher(Launcher_State *state);
