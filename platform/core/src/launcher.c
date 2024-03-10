#include "launcher.h"

#include <raylib.h>
#include "config.h"

Launcher_State InitLauncher(AppConfiguration appConfig)
{
    // load
    char *logo_path = "resources\\launcher.png";
    const char logo_str[999];
    snprintf(logo_str, sizeof(logo_str), "%s\\%s", appConfig.res_path, logo_path);
    Image image = LoadImage(logo_str);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    Launcher_State state = {0};
    state.appConfig = appConfig;
    state.launcherDelay = 2;
    state.framesCounter = 0;
    state.texture = texture;
    return state;
}

int UpdateLauncher(Launcher_State *state)
{
    int LAUNCHER = 0;
    int GAMEPLAY = 1;
    int delay = (state->appConfig.fps_limit * state->launcherDelay);
    // TODO: Update LOGO screen variables here!
    state->framesCounter++; // Count frames
    // Wait for 2 seconds (120 frames) before jumping to TITLE screen
    if (state->framesCounter > delay)
    {
        return GAMEPLAY;
    }
    return LAUNCHER;
}

void DrawLauncher(Launcher_State *state)
{
    DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
    DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
    DrawTexture(state->texture,
                state->appConfig.screen_width / 2 - state->texture.width / 2,
                state->appConfig.screen_height / 2 - state->texture.height / 2,
                WHITE);
}

void UnloadLauncher(Launcher_State *state)
{
    UnloadTexture(state->texture);
}