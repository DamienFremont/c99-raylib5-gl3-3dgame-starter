#include "game.h"

// SOURCE: https://github.com/raysan5/raylib-game-template

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "console.h"
#include "screens.h"
#include "camera.h"
#include "assets.h"

static const SCREEN = GAMEPLAY;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------

int UpdateScreen(int currentScreen, Launcher_State *launcherState, UnrealThirdPerson_State *unrealThirdPerson_State);
void DrawScreen(int currentScreen, Launcher_State *launcherState, UnrealThirdPerson_State *unrealThirdPerson_State, RenderTexture2D *target);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main(AppConfiguration appConfig)
{
	LogConsole("Hello Console!");

	// Initialization
	//--------------------------------------------------------------------------------------
	InitAssets(appConfig.res_path, appConfig.glsl_version);

	if (appConfig.postpro_msaa_enable == true)
		SetConfigFlags(FLAG_MSAA_4X_HINT); // Enable Multi Sampling Anti Aliasing 4x (if available)

	InitWindow(appConfig.screen_width, appConfig.screen_height, appConfig.appName);
	SetTargetFPS(appConfig.fps_limit);

	// Create a RenderTexture2D to be used for render to texture
	RenderTexture2D postprocessing_target = LoadRenderTexture(appConfig.screen_width, appConfig.screen_height);
	SetTextureFilter(postprocessing_target.texture, appConfig.postpro_texturefilter);

	// Levels
	int currentScreen = SCREEN;
	Launcher_State launcherState = InitLauncher(appConfig);
	UnrealThirdPerson_State unrealThirdPerson_State = Init_UnrealThirdPerson(appConfig, &postprocessing_target);
	Init_Menu(appConfig);

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		currentScreen = UpdateScreen(currentScreen, &launcherState, &unrealThirdPerson_State);
		DrawScreen(currentScreen, &launcherState, &unrealThirdPerson_State, &postprocessing_target);
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	// Levels
	Unload_UnrealThirdPerson(&unrealThirdPerson_State);
	UnloadLauncher(&launcherState);
	Unload_Menu();
	// target
	UnloadRenderTexture(postprocessing_target);

	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

int UpdateScreen(int currentScreen, Launcher_State *launcherState, UnrealThirdPerson_State *unrealThirdPerson_State)
{
	switch (currentScreen)
	{
	case LOGO:
	{
		currentScreen = UpdateLauncher(launcherState);
	}
	break;
	case GAMEPLAY:
	{
		currentScreen = Update_UnrealThirdPerson(unrealThirdPerson_State);
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

void DrawScreen(int currentScreen, Launcher_State *launcherState, UnrealThirdPerson_State *unrealThirdPerson_State, RenderTexture2D *postprocessing_target)
{
	switch (currentScreen)
	{
	case LOGO:
	{
		DrawLauncher(launcherState);
	}
	break;
	case GAMEPLAY:
	{
		Draw_UnrealThirdPerson(unrealThirdPerson_State, postprocessing_target);
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