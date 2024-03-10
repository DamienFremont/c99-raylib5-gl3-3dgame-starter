#include "core.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "console.h"
#include "input.h"

#include "level.h"
#include "camera.h"
#include "launcher.h"
#include "level_unrealthirdperson.h"

const char consoleOut[999];

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen
{
	LAUNCHER = 0,
	UNREAL_THIRDPERSON = 1,
	GAMEPLAY2 = 2
} GameScreen;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(AppConfiguration appConfig)
{
	strcpy(consoleOut, "Hello Console!");

	// Initialization
	//--------------------------------------------------------------------------------------
	InitWindow(appConfig.screen_width, appConfig.screen_height, appConfig.appName);
	if (appConfig.postpro_msaa_enable == true)
		SetConfigFlags(FLAG_MSAA_4X_HINT); // (if available)
	SetTargetFPS(appConfig.fps_limit);

	int currentScreen = LAUNCHER;

	// LAUNCHER
	Launcher_State launcherState = InitLauncher(appConfig);

	// GAMEPLAY
	ShaderResources shaRes = LoadShaders(appConfig);
	LvlResources lvlkRes = LoadLevel(appConfig);
	// Load gltf model animations
	unsigned int animIndex = ANIM_IDLE;
	unsigned int animCurrentFrame = 0;
	ModelAnimation *modelAnimations = lvlkRes.modelAnimations;
	// Define the camera to look into our 3d world
	Camera camera = InitCamera();
	// Config Render
	Vector3 playerPosition = PlayerPosition(); // Set model position
	// Create a RenderTexture2D to be used for render to texture
	RenderTexture2D target = LoadRenderTexture(appConfig.screen_width, appConfig.screen_height);
	bool showFPS = appConfig.fps_counter_show;
	int showConsole = 0;
	int animationEnable = 1;
	Shader postproShader = (appConfig.postpro_bloom_enable == true) ? shaRes.shaders[FX_BLOOM] : shaRes.shaders[FX_DEFAULT];
	Model model = lvlkRes.models[CHARACTER];
	InputEvent_State state = InitInputEvent();

	// GAMEPLAY2
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
		case GAMEPLAY2:
		{
			// Input
			float char_speed = 0.05f; // TODO: tickCount
			InputOut inout = ExecuteInputEvent(state, (InputConfig){
														  playerPosition,
														  showConsole,
														  char_speed});
			playerPosition.x = inout.playerPosition.x;
			playerPosition.y = inout.playerPosition.y;
			playerPosition.z = inout.playerPosition.z;
			showConsole = inout.showConsole;
			animIndex = inout.animIndex;
			// Action
			camera.position = (Vector3){
				0.0f + playerPosition.x,
				1.0f + playerPosition.y,
				-1.0f + playerPosition.z}; // Camera position
			camera.target = (Vector3){
				0.0f + playerPosition.x,
				0.25f + playerPosition.y,
				0.0f + playerPosition.z}; // Camera looking at point
			// Animation
			if (animationEnable == 1)
			{
				ModelAnimation anim = modelAnimations[animIndex];
				animCurrentFrame = (animCurrentFrame + 1) % anim.frameCount; // TODO: tickCount
				UpdateModelAnimation(model, anim, animCurrentFrame);
			}
		}
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
			break;
			case GAMEPLAY2:
			{
				// 3D
				BeginMode3D(camera);
				{
					DrawModel(model, playerPosition, 0.1f, WHITE);
					DrawLevel();
					DrawGrid(10, 1.0f);
				}
				EndMode3D();
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
			break;
			case GAMEPLAY2:
			{
				// postprocessing
				BeginShaderMode(postproShader);
				{
					DrawTextureRec(target.texture, (Rectangle){0, 0, (float)target.texture.width, (float)-target.texture.height},
								   (Vector2){0, 0}, WHITE);
				}
				EndShaderMode();
				// 2D
				ConsoleConfig cfg = (ConsoleConfig){
					showConsole,
					appConfig.fps_counter_show,
					appConfig.screen_width,
					consoleOut};
				strcpy(cfg.consoleOut, consoleOut);
				DrawConsole(cfg);
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
	// GAMEPLAY
	UnloadShaders(shaRes);
	UnloadLevel(lvlkRes);
	// GAMEPLAY2
	Unload_UnrealThirdPerson(&unrealThirdPerson_State);
	// LAUNCHER
	UnloadLauncher(&launcherState);

	UnloadRenderTexture(target);

	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
