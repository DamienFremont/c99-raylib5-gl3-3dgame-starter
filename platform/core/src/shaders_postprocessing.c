#include "shaders_postprocessing.h"

#include "raylib.h"

#include<stdio.h>
#include<stdlib.h>


#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

#define MAX_POSTPRO_SHADERS 12

typedef enum
{
	FX_GRAYSCALE = 0,
	FX_POSTERIZATION,
	FX_DREAM_VISION,
	FX_PIXELIZER,
	FX_CROSS_HATCHING,
	FX_CROSS_STITCHING,
	FX_PREDATOR_VIEW,
	FX_SCANLINES,
	FX_FISHEYE,
	FX_SOBEL,
	FX_BLOOM,
	FX_BLUR,
	// FX_FXAA
} PostproShader;

static const char *postproShaderText[] = {
	"GRAYSCALE",
	"POSTERIZATION",
	"DREAM_VISION",
	"PIXELIZER",
	"CROSS_HATCHING",
	"CROSS_STITCHING",
	"PREDATOR_VIEW",
	"SCANLINES",
	"FISHEYE",
	"SOBEL",
	"BLOOM",
	"BLUR",
	//"FXAA"
};

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(const char *respath)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 800;
	const int screenHeight = 450;

	SetConfigFlags(FLAG_MSAA_4X_HINT); // Enable Multi Sampling Anti Aliasing 4x (if available)

	InitWindow(screenWidth, screenHeight, "cpp20-raylib-starter");

	// Define the camera to look into our 3d world
	Camera camera = {0};
	camera.position = (Vector3){2.0f, 3.0f, 2.0f}; // Camera position
	camera.target = (Vector3){0.0f, 1.0f, 0.0f};   // Camera looking at point
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};	   // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;						   // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;		   // Camera projection type

	char *mod_path = "resources\\models\\church.obj";
	const char model_str[999];
	snprintf(model_str, sizeof(model_str), "%s%s", respath, mod_path);

	char *tex_path = "resources\\models\\church_diffuse.png";
	const char texture_str[999];
	snprintf(texture_str, sizeof(model_str), "%s%s", respath, tex_path);

	char *sha_path = TextFormat("resources\\shaders\\glsl%i\\bloom.fs", GLSL_VERSION);
	const char sha_str[999];
	snprintf(sha_str, sizeof(model_str), "%s%s", respath, sha_path);

	// Model model = LoadModel("resources/models/church.obj");					// Load OBJ model
	Model model = LoadModel(model_str);					// Load OBJ model
	Texture2D texture = LoadTexture(texture_str); // Load model texture (diffuse map)
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;		// Set model diffuse texture

	Vector3 position = {0.0f, 0.0f, 0.0f}; // Set model position

	// Load all postpro shaders
	// NOTE 1: All postpro shader use the base vertex shader (DEFAULT_VERTEX_SHADER)
	// NOTE 2: We load the correct shader depending on GLSL version
	Shader shaders[MAX_POSTPRO_SHADERS] = {0};

	// NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
	shaders[FX_BLOOM] = LoadShader(0, sha_str);

	int currentShader = FX_BLOOM;

	// Create a RenderTexture2D to be used for render to texture
	RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

	SetTargetFPS(0); // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// SetCameraMode(camera, CAMERA_ORBITAL);
		UpdateCamera(&camera);
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginTextureMode(target);  // Enable drawing to texture
		ClearBackground(RAYWHITE); // Clear texture background

		BeginMode3D(camera);					 // Begin 3d mode drawing
		DrawModel(model, position, 0.1f, WHITE); // Draw 3d model with texture
		DrawGrid(10, 1.0f);						 // Draw a grid
		EndMode3D();							 // End 3d mode drawing, returns to orthographic 2d mode
		EndTextureMode();						 // End drawing to texture (now we have a texture available for next passes)

		BeginDrawing();
		ClearBackground(RAYWHITE); // Clear screen background

		// Render generated texture using selected postprocessing shader
		BeginShaderMode(shaders[currentShader]);
		// NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
		DrawTextureRec(target.texture, (Rectangle){0, 0, (float)target.texture.width, (float)-target.texture.height}, (Vector2){0, 0}, WHITE);
		EndShaderMode();

		// Draw 2d shapes and text over drawn texture
		DrawRectangle(0, 9, 580, 30, Fade(LIGHTGRAY, 0.7f));

		DrawText(model_str, screenWidth - 700, screenHeight - 20, 10, GRAY);
		DrawText("CURRENT POSTPRO SHADER:", 10, 15, 20, BLACK);
		DrawText(postproShaderText[currentShader], 330, 15, 20, RED);
		DrawFPS(700, 15);
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	// Unload all postpro shaders
	for (int i = 0; i < MAX_POSTPRO_SHADERS; i++)
		UnloadShader(shaders[i]);

	UnloadTexture(texture);		 // Unload texture
	UnloadModel(model);			 // Unload model
	UnloadRenderTexture(target); // Unload render texture

	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
