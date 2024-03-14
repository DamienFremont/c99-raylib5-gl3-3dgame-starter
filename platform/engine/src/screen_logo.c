#include "screen_logo.h"

#include <raylib.h>
#include <time.h>
#include "config.h"

Launcher_State InitLauncher(AppConfiguration appConfig)
{
    int clockTriggerSec = 2;
    // load
    char *logo_path = "resources\\launcher.png";
    const char logo_str[999];
    snprintf(logo_str, sizeof(logo_str), "%s\\%s", appConfig.res_path, logo_path);
    Image image = LoadImage(logo_str);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    Launcher_State state = {0};
    state.appConfig = appConfig;
    state.counterInMs = clockTriggerSec * 1000;
    state.startClock = clock();
    state.screenTexture = texture;
    state.screenTextureScale = (float)GetScreenHeight() / GetScreenHeight();
    state.logoPositionX = GetScreenWidth() / 2 - (state.screenTextureScale * texture.width) / 2;
    return state;
}

int UpdateLauncher(Launcher_State *state)
{
    int LOGO = 0;
    int GAMEPLAY = 1;
    // compute
    clock_t difference = clock() - state->startClock;
    int msec = difference * 1000 / CLOCKS_PER_SEC;
    // changer level
    if (msec > state->counterInMs)
    {
        return GAMEPLAY;
    }
    return LOGO;
}

void DrawLauncher(Launcher_State *state)
{
    // DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
    // DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
    DrawRectangle(0, 0, state->appConfig.screen_width, state->appConfig.screen_height, BLACK);
    DrawTextureEx(state->screenTexture,
                  (Vector2){
                      state->logoPositionX, 0},
                  0.0f,
                  state->screenTextureScale,
                  WHITE);
}

void UnloadLauncher(Launcher_State *state)
{
    UnloadTexture(state->screenTexture);
}