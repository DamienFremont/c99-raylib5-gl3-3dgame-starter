#include "mygame.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "console.h"
#include "launcher.h"
#include "level_unrealthirdperson.h"
#include "camera.h"

#include "rlgl.h"
#include "raymath.h" // Required for: MatrixPerspective(), MatrixLookAt()

// Generate cubemap (6 faces) from equirectangular (panorama) texture
static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format);

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
	int currentScreen = LAUNCHER;
	Launcher_State launcherState = InitLauncher(appConfig);
	UnrealThirdPerson_State unrealThirdPerson_State = Init_UnrealThirdPerson(appConfig, &target, consoleOut);

	// Skybox
	int GLSL_VERSION = appConfig.glsl_version;
	bool useHDR = false;
	Mesh cube = GenMeshCube(500.0f, 500.0f, 500.0f);
	Model skybox = LoadModelFromMesh(cube);
	// Load skybox shader and set required locations
	// NOTE: Some locations are automatically set at shader loading
	skybox.materials[0].shader = LoadShader(TextFormat("resources\\shaders\\glsl%i\\skybox.vs", GLSL_VERSION),
											TextFormat("resources\\shaders\\glsl%i\\skybox.fs", GLSL_VERSION));
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), (int[1]){MATERIAL_MAP_CUBEMAP}, SHADER_UNIFORM_INT);
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), (int[1]){useHDR ? 1 : 0}, SHADER_UNIFORM_INT);
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), (int[1]){useHDR ? 1 : 0}, SHADER_UNIFORM_INT);
	// Load cubemap shader and setup required shader locations
	Shader shdrCubemap = LoadShader(TextFormat("resources\\shaders\\glsl%i\\cubemap.vs", GLSL_VERSION),
									TextFormat("resources\\shaders\\glsl%i\\cubemap.fs", GLSL_VERSION));
	SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), (int[1]){0}, SHADER_UNIFORM_INT);

	if (useHDR)
	{
		// Load HDR panorama (sphere) texture
		Texture2D panorama = LoadTexture("resources\\skybox.png");
		skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = GenTextureCubemap(shdrCubemap, panorama, 1024, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
		UnloadTexture(panorama); // Texture not required anymore, cubemap already generated
	}
	else
	{
		// Load img texture
		Image img = LoadImage("resources\\skybox.png");
		skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT); // CUBEMAP_LAYOUT_PANORAMA
		UnloadImage(img);
	}
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
					DrawModel(skybox, (Vector3){0, 0, 0}, 1.0f, SKYBLUE);
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

// Generate cubemap texture from HDR texture
static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format)
{
	TextureCubemap cubemap = {0};

	rlDisableBackfaceCulling(); // Disable backface culling to render inside the cube

	// STEP 1: Setup framebuffer
	//------------------------------------------------------------------------------------------
	unsigned int rbo = rlLoadTextureDepth(size, size, true);
	cubemap.id = rlLoadTextureCubemap(0, size, format);

	unsigned int fbo = rlLoadFramebuffer(512, 512);
	rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
	rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);

	// Check if framebuffer is complete with attachments (valid)
	if (rlFramebufferComplete(fbo))
		TraceLog(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", fbo);
	//------------------------------------------------------------------------------------------

	// STEP 2: Draw to framebuffer
	//------------------------------------------------------------------------------------------
	// NOTE: Shader is used to convert HDR equirectangular environment map to cubemap equivalent (6 faces)
	rlEnableShader(shader.id);

	// Define projection matrix and send it to shader
	Matrix matFboProjection = MatrixPerspective(90.0 * DEG2RAD, 1.0, RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);
	rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matFboProjection);

	// Define view matrix for every side of the cubemap
	Matrix fboViews[6] = {
		MatrixLookAt((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){1.0f, 0.0f, 0.0f}, (Vector3){0.0f, -1.0f, 0.0f}),
		MatrixLookAt((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){-1.0f, 0.0f, 0.0f}, (Vector3){0.0f, -1.0f, 0.0f}),
		MatrixLookAt((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, 1.0f, 0.0f}, (Vector3){0.0f, 0.0f, 1.0f}),
		MatrixLookAt((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, -1.0f, 0.0f}, (Vector3){0.0f, 0.0f, -1.0f}),
		MatrixLookAt((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, 0.0f, 1.0f}, (Vector3){0.0f, -1.0f, 0.0f}),
		MatrixLookAt((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, 0.0f, -1.0f}, (Vector3){0.0f, -1.0f, 0.0f})};

	rlViewport(0, 0, size, size); // Set viewport to current fbo dimensions

	// Activate and enable texture for drawing to cubemap faces
	rlActiveTextureSlot(0);
	rlEnableTexture(panorama.id);

	for (int i = 0; i < 6; i++)
	{
		// Set the view matrix for the current cube face
		rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], fboViews[i]);

		// Select the current cubemap face attachment for the fbo
		// WARNING: This function by default enables->attach->disables fbo!!!
		rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X + i, 0);
		rlEnableFramebuffer(fbo);

		// Load and draw a cube, it uses the current enabled texture
		rlClearScreenBuffers();
		rlLoadDrawCube();

		// ALTERNATIVE: Try to use internal batch system to draw the cube instead of rlLoadDrawCube
		// for some reason this method does not work, maybe due to cube triangles definition? normals pointing out?
		// TODO: Investigate this issue...
		// rlSetTexture(panorama.id); // WARNING: It must be called after enabling current framebuffer if using internal batch system!
		// rlClearScreenBuffers();
		// DrawCubeV(Vector3Zero(), Vector3One(), WHITE);
		// rlDrawRenderBatchActive();
	}
	//------------------------------------------------------------------------------------------

	// STEP 3: Unload framebuffer and reset state
	//------------------------------------------------------------------------------------------
	rlDisableShader();		  // Unbind shader
	rlDisableTexture();		  // Unbind texture
	rlDisableFramebuffer();	  // Unbind framebuffer
	rlUnloadFramebuffer(fbo); // Unload framebuffer (and automatically attached depth texture/renderbuffer)

	// Reset viewport dimensions to default
	rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());
	rlEnableBackfaceCulling();
	//------------------------------------------------------------------------------------------

	cubemap.width = size;
	cubemap.height = size;
	cubemap.mipmaps = 1;
	cubemap.format = format;

	return cubemap;
}