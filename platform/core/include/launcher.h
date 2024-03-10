#include "config.h"

#include <raylib.h>
#include <time.h>

typedef struct Launcher_State Launcher_State;
struct Launcher_State
{
    AppConfiguration appConfig;
    Texture2D texture;
    int durationInMs;
    clock_t clockAtStartup;
};

Launcher_State InitLauncher(AppConfiguration appConfig);

int UpdateLauncher(Launcher_State *state);

void DrawLauncher(Launcher_State *state);

void UnloadLauncher(Launcher_State *state);
