#include "gam_screen_logo.h"

#include <raylib.h>
#include <time.h>
#include "eng_config.h"

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef struct Launcher_State
{
    Texture2D screenTexture;
    float screenTextureScale;
    int logoPositionX;
    int counterInMs;
    clock_t startClock;
} Launcher_State;

//---------------------------------------------------------
// Local Variables Definition (local to this module)
//---------------------------------------------------------

const int SCREEN_CURR = 0;
const int SCREEN_NEXT = 1;
const char *LOGO_URI = "resources\\launcher.png";

Launcher_State state = {0};

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

void InitLauncher(AppConfiguration appConfig)
{
    int clockTriggerSec = 2;
    // load
    const char logo_str[999];
    snprintf(logo_str, sizeof(logo_str), "%s\\%s", appConfig.res_path, LOGO_URI);
    Image image = LoadImage(logo_str);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    // start
    state.screenTexture = texture;
    state.screenTextureScale = (float)GetScreenHeight() / GetScreenHeight();
    state.logoPositionX = GetScreenWidth() / 2 - (state.screenTextureScale * texture.width) / 2;
    state.counterInMs = clockTriggerSec * 1000;
    state.startClock = clock();
}

int UpdateLauncher()
{
    // compute
    clock_t difference = clock() - state.startClock;
    int msec = difference * 1000 / CLOCKS_PER_SEC;
    // changer level
    if (msec > state.counterInMs)
    {
        return SCREEN_NEXT;
    }
    return SCREEN_CURR;
}

void DrawLauncher()
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE); // Clear texture background

        // DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
        // DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
        DrawTextureEx(state.screenTexture,
                      (Vector2){
                          state.logoPositionX, 0},
                      0.0f,
                      state.screenTextureScale,
                      WHITE);
    }
    EndDrawing();
}

void UnloadLauncher()
{
    UnloadTexture(state.screenTexture);
}
