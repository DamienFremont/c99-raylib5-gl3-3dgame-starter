#include "game.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "console.h"
#include "screens.h"
#include "camera_thirdperson.h"
#include "assets.h"


const char consoleOut[999];



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
	strcpy(consoleOut, "Hello Console!");

	// Initialization
	//--------------------------------------------------------------------------------------
	InitAssets(appConfig.res_path);

	InitWindow(appConfig.screen_width, appConfig.screen_height, appConfig.appName);
	SetTargetFPS(appConfig.fps_limit);

	// Create a RenderTexture2D to be used for render to texture
	RenderTexture2D target = LoadRenderTexture(appConfig.screen_width, appConfig.screen_height);
	if (appConfig.postpro_msaa_enable == true)
		SetConfigFlags(FLAG_MSAA_4X_HINT); // (if available)
	SetTextureFilter(target.texture, appConfig.postpro_texturefilter);

	// Levels
	int currentScreen = GAMEPLAY;
	Launcher_State launcherState = InitLauncher(appConfig);
	UnrealThirdPerson_State unrealThirdPerson_State = Init_UnrealThirdPerson(appConfig, &target, consoleOut);

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		currentScreen = UpdateScreen(currentScreen, &launcherState, &unrealThirdPerson_State);
		DrawScreen(currentScreen, &launcherState, &unrealThirdPerson_State, &target);
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	// Levels
	Unload_UnrealThirdPerson(&unrealThirdPerson_State);
	UnloadLauncher(&launcherState);
	// target
	UnloadRenderTexture(target);

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
		Update_UnrealThirdPerson(unrealThirdPerson_State);
	}
	break;
	default:
		break;
	}
	return currentScreen;
}

void DrawScreen(int currentScreen, Launcher_State *launcherState, UnrealThirdPerson_State *unrealThirdPerson_State, RenderTexture2D *target)
{
	BeginTextureMode(*target); // Enable drawing to texture
	{
		ClearBackground(RAYWHITE); // Clear texture background

		switch (currentScreen)
		{
		case GAMEPLAY:
		{
			Texture_UnrealThirdPerson(unrealThirdPerson_State);
		}
		default:
			break;
		}
	}
	EndTextureMode();

	BeginDrawing();
	{
		ClearBackground(RAYWHITE); // Clear screen background

		switch (currentScreen)
		{
		case LOGO:
		{
			DrawLauncher(launcherState);
		}
		break;
		case GAMEPLAY:
		{
			Draw_UnrealThirdPerson(unrealThirdPerson_State, target);
		}
		default:
			break;
		}
	}
	EndDrawing();
}	