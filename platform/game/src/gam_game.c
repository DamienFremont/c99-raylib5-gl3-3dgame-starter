#include "gam_game.h"

// SOURCE: https://github.com/raysan5/raylib-game-template

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "gam_screens.h"
#include "eng_console.h"
#include "eng_camera.h"
#include "eng_assets.h"

static const SCREEN_START = GAMEPLAY;

//---------------------------------------------------------
// Local Functions Declaration
//---------------------------------------------------------

int UpdateScreen(int screen);
void DrawScreen(int screen, RenderTexture2D *target);

//-----------------------------------------------------------
// Program main entry point
//-----------------------------------------------------------

int main(AppConfiguration cfg)
{
	LogConsole("Hello Console!");

	// Initialization
	//-------------------------------------------------------------
	InitAssets(cfg.res_path, cfg.glsl_version);

	if (cfg.postpro_antialias_msaa == true)
		SetConfigFlags(FLAG_MSAA_4X_HINT); // Enable Multi Sampling Anti Aliasing 4x (if available)

	InitWindow(cfg.screen_width, cfg.screen_height, cfg.appName);
	SetTargetFPS(cfg.fps_limit);

	// Create a RenderTexture2D to be used for render to texture
	RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	SetTextureFilter(target.texture, cfg.postpro_texturefilter);

	// Levels
	int currentScreen = SCREEN_START;
	InitLauncher(cfg);
	Init_UnrealThirdPerson(cfg, &target);
	Init_Menu(cfg);

	//-------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
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
	{
		currentScreen = UpdateLauncher();
	}
	break;
	case GAMEPLAY:
	{
		currentScreen = Update_UnrealThirdPerson();
	}
	break;
	case MENU:
	{
		currentScreen = Update_Menu();
	}
	break;
	default:
		break;
	}
	return currentScreen;
}

void DrawScreen(int currentScreen, RenderTexture2D *postprocessing_target)
{
	switch (currentScreen)
	{
	case LOGO:
	{
		DrawLauncher();
	}
	break;
	case GAMEPLAY:
	{
		Draw_UnrealThirdPerson(postprocessing_target);
	}
	break;
	case MENU:
	{
		Draw_Menu();
	}
	break;
	default:
		break;
	}
}