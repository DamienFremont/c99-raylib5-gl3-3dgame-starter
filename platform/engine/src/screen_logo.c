#include "screen_logo.h"

#include <raylib.h>
#include <time.h>
#include "config.h"
#include "assets.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0;
// const
static int DELAY_IN_SEC = 2;
static char *ASSET_PATH = "resources/launcher.png";
static const Color BACKGROUND_COLOR = {0, 0, 0, 255}; // black
// vars
static int counterInMs = 0;
static clock_t startClock = {0};
static Texture2D screenTexture = {0};
static float screenTextureScale = 1.0f;
static int logoPositionX = 0;
static int screenHeight = 450;
static int screenWidth = 800;

//----------------------------------------------------------------------------------
// Logo Screen Functions Definition
//----------------------------------------------------------------------------------

void InitLogoScreen(char *RESOURCES)
{
    counterInMs = DELAY_IN_SEC * 1000;
    startClock = clock();
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    screenTexture = LoadTextureResource(RESOURCES, ASSET_PATH);
    screenTextureScale = (float)GetScreenHeight() / screenTexture.height;
    logoPositionX = screenWidth / 2 - (screenTextureScale * screenTexture.width) / 2;
}

void UpdateLogoScreen(void)
{
    int LAUNCHER = 0;
    int LEVEL1 = 1;
    // compute
    clock_t difference = clock() - startClock;
    int msec = difference * 1000 / CLOCKS_PER_SEC;
    // changer level
    if (msec > counterInMs)
    {
        finishScreen = 1;
    }
}

void DrawLogoScreen(void)
{
    DrawRectangle(0, 0, screenWidth, screenHeight, BACKGROUND_COLOR);
    // DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
    // DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
    DrawTextureEx(screenTexture,
                  (Vector2){
                      logoPositionX, 0},
                  0.0f,
                  screenTextureScale,
                  WHITE);
}

void UnloadLogoScreen(void)
{
    UnloadTexture(screenTexture);
}

int FinishLogoScreen(void)
{
    return finishScreen;
}
