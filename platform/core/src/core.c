#include "core.h"

#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_POSTPRO_SHADERS 2
#define MAX_MODELS 1

const char consoleOut[999];

// log(char str)
// {
// 	strcpy(consoleOut, str);
// }

typedef enum
{
	FX_DEFAULT = 0,
	FX_BLOOM,
} PostproShader;

typedef enum
{
	CHARACTER = 0,
} Models;

char *pathJoin(char *path1, char *path2)
{
	const char path[999];
	snprintf(path, sizeof(path), "%s\\%s", path1, path2);
	return path;
}

loadModels(Model *models, AppProperties appProps)
{
	models[CHARACTER] = LoadModel(pathJoin(appProps.res_path, "resources\\models\\character.obj"));
}

// loadShaders(Shader *shaders, AppProperties appProps)
// {
// 	shaders[FX_DEFAULT] = LoadShader(0, pathJoin(appProps.res_path, TextFormat("resources\\shaders\\glsl%i\\default.fs", appProps.glsl_version)));
// 	shaders[FX_BLOOM] = LoadShader(0, pathJoin(appProps.res_path, TextFormat("resources\\shaders\\glsl%i\\bloom.fs", appProps.glsl_version)));
// }

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(AppProperties appProps)
{

	// Vars
	//--------------------------------------------------------------------------------------

	char *tex_path = "resources\\models\\character_diffuse.png";
	const char texture_str[999];
	snprintf(texture_str, sizeof(texture_str), "%s\\%s", appProps.res_path, tex_path);

	char *shadef_path = TextFormat("resources\\shaders\\glsl%i\\default.fs", appProps.glsl_version);
	const char shadef_str[999];
	snprintf(shadef_str, sizeof(shadef_str), "%s\\%s", appProps.res_path, shadef_path);

	char *sha_path = TextFormat("resources\\shaders\\glsl%i\\bloom.fs", appProps.glsl_version);
	const char sha_str[999];
	snprintf(sha_str, sizeof(sha_str), "%s\\%s", appProps.res_path, sha_path);

	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 800;
	const int screenHeight = 450;

	if (appProps.msaa_enable == true)
		SetConfigFlags(FLAG_MSAA_4X_HINT); // Enable Multi Sampling Anti Aliasing 4x (if available)

	InitWindow(screenWidth, screenHeight, "cpp20-raylib-starter");

	// Loading
	//--------------------------------------------------------------------------------------

	// Load resources
	Model models[MAX_MODELS] = {0};

	Texture2D texture = LoadTexture(texture_str);

	loadModels(models, appProps);
	models[CHARACTER].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

	Shader shaders[MAX_POSTPRO_SHADERS] = {0};
	shaders[FX_DEFAULT] = LoadShader(0, shadef_str);
	shaders[FX_BLOOM] = LoadShader(0, sha_str);

	// Start
	//--------------------------------------------------------------------------------------

	// Define the camera to look into our 3d world
	Camera camera = {0};
	camera.position = (Vector3){0.5f, 0.5f, 1.0f}; // Camera position
	camera.target = (Vector3){0.0f, 0.25f, 0.0f};   // Camera looking at point
	camera.up = (Vector3){0.0f, 0.75f, 0.0f};	   // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;						   // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;		   // Camera projection type

	// Config Render
	Shader postproShader = (appProps.bloom_enable == true) ? shaders[FX_BLOOM] : shaders[FX_DEFAULT];
	Vector3 position = {0.0f, 0.0f, 0.0f}; // Set model position

	// Create a RenderTexture2D to be used for render to texture
	RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

	// Set our game to run at X frames-per-second
	SetTargetFPS(appProps.fps_cap);
	bool showFPS = appProps.showFPS;
	bool showConsole = false;

	//--------------------------------------------------------------------------------------

	strcpy(consoleOut, "Hello Console!");

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		UpdateCamera(&camera, CAMERA_ORBITAL);

		if (IsKeyPressed(KEY_F1))
		{
			showConsole = !showConsole;
		}

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------

		// 3D
		BeginTextureMode(target);  // Enable drawing to texture
		ClearBackground(RAYWHITE); // Clear texture background
		BeginMode3D(camera);
		DrawModel(models[CHARACTER], position, 0.1f, WHITE);
                    DrawCubeWiresV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, RED);
                    DrawCubeV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, PURPLE);
                    DrawCubeWiresV((Vector3){ 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, DARKGREEN);
                    DrawCubeV((Vector3) { 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, YELLOW);
                    DrawGrid(10, 1.0f);
		EndMode3D();
		EndTextureMode(); // End drawing to texture (now we have a texture available for next passes)

		// Begin Drawing
		BeginDrawing();
		ClearBackground(RAYWHITE); // Clear screen background

		// postprocessing
		BeginShaderMode(postproShader);
		DrawTextureRec(target.texture, (Rectangle){0, 0, (float)target.texture.width, (float)-target.texture.height}, (Vector2){0, 0}, WHITE);
		EndShaderMode();

		// 2D
		if (showConsole == true)
		{
			DrawRectangle(0, 9, screenWidth, 30 * 3, Fade(BLACK, 0.7f));
			DrawText(consoleOut, 10, 20, 10, WHITE);
		}
		if (showFPS == true)
		{
			DrawFPS(700, 15);
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	// Unload all postpro shaders
	for (int i = 0; i < MAX_POSTPRO_SHADERS; i++)
		UnloadShader(shaders[i]);
	// Unload texture
	UnloadTexture(texture);
	// Unload model
	for (int i = 0; i < MAX_MODELS; i++)
		UnloadModel(models[i]);
	UnloadRenderTexture(target); // Unload render texture

	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
