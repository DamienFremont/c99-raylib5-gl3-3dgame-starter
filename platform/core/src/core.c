#include "core.h"

#include "common.c"
#include "console.c"
#include "input.c"

#include "level1.c"
#include "prefabCamera.c"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

const char consoleOut[999];

// log(char str)
// {
// 	strcpy(consoleOut, str);
// }

void changeStage(int* stage_p, int newValue){
    *stage_p = newValue;
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(AppProperties props)
{
	strcpy(consoleOut, "Hello Console!");

	// Initialization
	//--------------------------------------------------------------------------------------
	if (props.postpro_msaa_enable == true)
		SetConfigFlags(FLAG_MSAA_4X_HINT); // Enable Multi Sampling Anti Aliasing 4x (if available)

	InitWindow(props.screen_width, props.screen_height, props.appName);

	ShaderResources shaRes = LoadShaders(props);
	LvlResources lvlkRes = LoadLevel(props);

	// Load gltf model animations
	unsigned int animIndex = ANIM_IDLE;
	unsigned int animCurrentFrame = 0;
	ModelAnimation *modelAnimations = lvlkRes.modelAnimations;

	// Start
	//--------------------------------------------------------------------------------------

	// Define the camera to look into our 3d world
	Camera camera = InitCamera();

	// Config Render
	Vector3 playerPosition = PlayerPosition(); // Set model position

	// Create a RenderTexture2D to be used for render to texture
	RenderTexture2D target = LoadRenderTexture(props.screen_width, props.screen_height);

	// Set our game to run at X frames-per-second
	SetTargetFPS(props.fps_limit);
	bool showFPS = props.fps_counter_show;
	int showConsole = 0;

	Shader postproShader = (props.postpro_bloom_enable == true) ? shaRes.shaders[FX_BLOOM] : shaRes.shaders[FX_DEFAULT];
	Model model = lvlkRes.models[CHARACTER];
	// Texture2D texture = res.textures[CHARACTER];

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// UpdateCamera(&camera, CAMERA_CUSTOM);

		// Input
		float char_speed = 0.05f; // TODO: tickCount

		InputOut inout = CheckInputEvent((InputConfig){
			playerPosition,
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
		ModelAnimation anim = modelAnimations[animIndex];
		animCurrentFrame = (animCurrentFrame + 1) % anim.frameCount; // TODO: tickCount
		UpdateModelAnimation(model, anim, animCurrentFrame);

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------

		// 3D
		BeginTextureMode(target); // Enable drawing to texture
		{
			ClearBackground(RAYWHITE); // Clear texture background
			BeginMode3D(camera);
			{
				DrawModel(model, playerPosition, 0.1f, WHITE);
				DrawLevel();
				DrawGrid(10, 1.0f);
			}
			EndMode3D();
		}
		EndTextureMode(); // End drawing to texture (now we have a texture available for next passes)

		// Begin Drawing
		BeginDrawing();
		{
			ClearBackground(RAYWHITE); // Clear screen background

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
				props.fps_counter_show,
				props.screen_width,
				consoleOut};
			strcpy(cfg.consoleOut, consoleOut);
			DrawConsole(cfg);
		}
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadShaders(shaRes);
	UnloadLevel(lvlkRes);
	UnloadRenderTexture(target);

	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
