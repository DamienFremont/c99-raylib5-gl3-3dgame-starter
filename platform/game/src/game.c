#include "game.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "console.h"
#include "assets.h"

#include "scenes.h"
#include "scene_launcher.h"
#include "scene_unrealthirdperson.h"

const char consoleOut[999];

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(AppConfiguration appConfig)
{
	strcpy(consoleOut, "Hello Console!");

	// Initialization
	//--------------------------------------------------------------------------------------
	InitWindow(appConfig.screen_width, appConfig.screen_height, appConfig.appName);
	SetTargetFPS(appConfig.fps_limit);

	// Create a RenderTexture2D to be used for render to texture
	RenderTexture2D target = LoadRenderTexture(appConfig.screen_width, appConfig.screen_height);
	if (appConfig.postpro_msaa_enable == true)
		SetConfigFlags(FLAG_MSAA_4X_HINT); // (if available)
	SetTextureFilter(target.texture, appConfig.postpro_texturefilter);

	// Levels
	int currentScreen = UNREAL_THIRDPERSON;
	Launcher_State launcherState = InitLauncher(appConfig);
	UnrealThirdPerson_State unrealThirdPerson_State = Init_UnrealThirdPerson(appConfig, &target, consoleOut);

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		switch (currentScreen)
		{
		case LAUNCHER:
		{
			currentScreen = UpdateLauncher(&launcherState);
		}
		break;
		case UNREAL_THIRDPERSON:
		{
			Update_UnrealThirdPerson(&unrealThirdPerson_State);
		}
		break;
		default:
			break;
		}
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginTextureMode(target); // Enable drawing to texture
		{
			ClearBackground(RAYWHITE); // Clear texture background

			switch (currentScreen)
			{
			case UNREAL_THIRDPERSON:
			{
				Texture_UnrealThirdPerson(&unrealThirdPerson_State);
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
			case LAUNCHER:
			{
				DrawLauncher(&launcherState);
			}
			break;
			case UNREAL_THIRDPERSON:
			{
				Draw_UnrealThirdPerson(&unrealThirdPerson_State, target);
			}
			default:
				break;
			}
		}
		EndDrawing();
		//----------------------------------------------------------------------------------
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
