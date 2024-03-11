#include "mygame.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "console.h"
#include "launcher.h"
#include "level_unrealthirdperson.h"
#include "camera.h"
#include "assets.h"

const char consoleOut[999];

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen
{
	LAUNCHER = 0,
	UNREAL_THIRDPERSON = 1
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
	SetTargetFPS(appConfig.fps_limit);
	if (appConfig.postpro_msaa_enable == true)
		SetConfigFlags(FLAG_MSAA_4X_HINT); // (if available)
	// Create a RenderTexture2D to be used for render to texture
	RenderTexture2D target = LoadRenderTexture(appConfig.screen_width, appConfig.screen_height);

	// Levels
	int currentScreen = UNREAL_THIRDPERSON;
	Launcher_State launcherState = InitLauncher(appConfig);
	UnrealThirdPerson_State unrealThirdPerson_State = Init_UnrealThirdPerson(appConfig, &target, consoleOut);

	// Skybox
	int GLSL_VERSION = appConfig.glsl_version;
	Mesh cube = GenMeshCube(500.0f, 500.0f, 500.0f);
	Model skybox = LoadModelFromMesh(cube);
	// Load skybox shader and set required locations

	// NOTE: Some locations are automatically set at shader loading
	skybox.materials[0].shader = LoadShaderResource2(appConfig.res_path,
													TextFormat("resources/shaders/glsl%i/skybox.vs", GLSL_VERSION),
													TextFormat("resources/shaders/glsl%i/skybox.fs", GLSL_VERSION));
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), (int[1]){MATERIAL_MAP_CUBEMAP}, SHADER_UNIFORM_INT);
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), (int[1]){0}, SHADER_UNIFORM_INT);
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), (int[1]){0}, SHADER_UNIFORM_INT);
	// Load cubemap shader and setup required shader locations
	Shader shdrCubemap = LoadShaderResource2(appConfig.res_path,
											TextFormat("resources/shaders/glsl%i/cubemap.vs", GLSL_VERSION),
											TextFormat("resources/shaders/glsl%i/cubemap.fs", GLSL_VERSION));
	SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), (int[1]){0}, SHADER_UNIFORM_INT);

	// Load img texture
	Image img = LoadImageResource(appConfig.res_path, "resources/skybox.png");
	skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT); // CUBEMAP_LAYOUT_PANORAMA
	UnloadImage(img);

	Camera camera = InitCamera();

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

				// Skybox
				BeginMode3D(camera);
				{
					rlDisableBackfaceCulling();
					rlDisableDepthMask();
					{
						DrawModel(skybox, (Vector3){0, 0, 0}, 1.0f, SKYBLUE);
					}
					rlEnableBackfaceCulling();
					rlEnableDepthMask();
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

	UnloadModel(skybox); // Unload skybox model

	// target
	UnloadRenderTexture(target);

	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
