#include "gam_game.h"

// SOURCE: https://github.com/raysan5/raylib-game-template

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "gam_screens.h"
#include "eng_screen.h"
#include "eng_console.h"
#include "eng_camera.h"
#include "eng_assets.h"

static const SCREEN_START = GAMEPLAY;
static const WINDOWED = 1;

//---------------------------------------------------------
// Local Functions Declaration
//---------------------------------------------------------

int UpdateScreen(int screen);
void DrawScreen(int screen, RenderTexture2D *target);

void UpdateVideoMode(int w, int h);

RenderTexture2D InitRenderTarget(int w, int h, int txt);
void InitRenderResolution(int w, int h, int txt);

//-----------------------------------------------------------
// Program main entry point
//-----------------------------------------------------------

RenderTexture2D target;

int main(AppConfiguration cfg)
{
	LogConsole("Hello Console!");

	// Initialization
	//-------------------------------------------------------------
	InitAssets(cfg.res_path, cfg.glsl_version);

	// Set configuration flags for window creation
	if (cfg.postpro_antialias_msaa == true)
		SetConfigFlags(FLAG_MSAA_4X_HINT); // Enable Multi Sampling Anti Aliasing 4x (if available)
	SetTargetFPS(cfg.fps_limit);
	// window / fullscreen
	cfg.screen_height = ENG_GetScreenHeight(cfg.screen_resolution);
	cfg.screen_width = ENG_GetScreenWidth(cfg.screen_resolution);
	InitWindow(cfg.screen_width, cfg.screen_height, cfg.appName);
	InitRenderResolution(cfg.screen_width, cfg.screen_height, cfg.postpro_texturefilter);
	if (cfg.screen_windowed == false && IsWindowFullscreen() == false)
		ToggleFullscreen();

	// Levels
	int currentScreen = SCREEN_START;
	InitLauncher();
	Init_UnrealThirdPerson(&target, cfg);
	Init_Menu();

	//-------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// common
		UpdateVideoMode(cfg.screen_width, cfg.screen_height);
		// game
		currentScreen = UpdateScreen(currentScreen);
		DrawScreen(currentScreen, &target);
	}

	// De-Initialization
	//-------------------------------------------------------------
	// Levels
	Unload_UnrealThirdPerson();
	UnloadLauncher();
	Unload_Menu();
	// target
	UnloadRenderTexture(target);

	CloseWindow(); // Close window and OpenGL context
	//-------------------------------------------------------------

	return 0;
}

int UpdateScreen(int currentScreen)
{
	switch (currentScreen)
	{
	case LOGO:
		currentScreen = UpdateLauncher();
		break;
	case GAMEPLAY:
		currentScreen = Update_UnrealThirdPerson();
		break;
	case MENU:
		currentScreen = Update_Menu();
		break;
	default:
		break;
	}
	return currentScreen;
}

void DrawScreen(int currentScreen, RenderTexture2D *target)
{
	switch (currentScreen)
	{
	case LOGO:
		DrawLauncher();
		break;
	case GAMEPLAY:
		Draw_UnrealThirdPerson(target);
		break;
	case MENU:
		Draw_Menu();
		break;
	default:
		break;
	}
}

RenderTexture2D InitRenderTarget(int screen_width, int screen_height, int postpro_texturefilter)
{
	RenderTexture2D target = LoadRenderTexture(screen_width, screen_height);
	SetTextureFilter(target.texture, postpro_texturefilter);
	return target;
}

// source: https://gist.github.com/JeffM2501/6e4630a0e34c0c7dddf066f7192e342d
void UpdateVideoMode(int screen_width, int screen_height, int postpro_texturefilter)
{
	if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
	{
		InitRenderResolution(screen_width, screen_height, postpro_texturefilter);
		ToggleFullscreen();
	}
}

void InitRenderResolution(int screen_width, int screen_height, int postpro_texturefilter)
{
	SetWindowSize(screen_width, screen_height);
	target = InitRenderTarget(screen_width, screen_height, postpro_texturefilter);
}
