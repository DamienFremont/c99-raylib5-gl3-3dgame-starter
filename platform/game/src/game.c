#include "game.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "console.h"
#include "assets.h"

#include "screens.h"

//-----------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//-----------------------------------------------------------------------------
GameScreen currentScreen = LOGO;

//-----------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//-----------------------------------------------------------------------------
static int screenWidth = 800;
static int screenHeight = 450;

char consoleOut[999];	 // TODO: REMOVEME
char *resources;		 // TODO: REMOVEME
AppConfiguration config; // TODO: REMOVEME
RenderTexture2D target;

//-----------------------------------------------------------------------------
// Local Functions Declaration
//-----------------------------------------------------------------------------
static void ChangeToScreen(int screen); // Change to screen, no transition effect

// static void TransitionToScreen(int screen); // Request transition to next screen
// static void UpdateTransition(void);			// Update transition effect
// static void DrawTransition(void);			// Draw transition effect (full-screen rectangle)

static void UpdateDrawFrame(void); // Update and draw one frame

static void UnloadCurrentScreen(void);
static void InitScreen(void);

//-----------------------------------------------------------------------------
// Main entry point
//-----------------------------------------------------------------------------
int main(AppConfiguration appConfig)
{
	strcpy(consoleOut, "Hello Console!");
	config = appConfig;
	screenWidth = appConfig.screenWidth;
	screenHeight = appConfig.screen_height;
	char *title = appConfig.appName;
	resources = &appConfig.res_path;

	// Initialization
	//-------------------------------------------------------------------------
	InitWindow(screenWidth, screenHeight, title);

	// InitAudioDevice();      // Initialize audio device

	// Load global data (assets that must be available in all screens, i.e. font)
	// font = LoadFont("resources/mecha.png");
	// music = LoadMusicStream("resources/ambient.ogg");
	// fxCoin = LoadSound("resources/coin.wav");

	// SetMusicVolume(music, 1.0f);
	// PlayMusicStream(music);

	// Create a RenderTexture2D to be used for render to texture
	target = LoadRenderTexture(screenWidth, screenHeight);
	if (appConfig.postpro_msaa_enable == true)
	{
		SetConfigFlags(FLAG_MSAA_4X_HINT); // (if available)
										   // SetTextureFilter(target.texture, TEXTURE_FILTER_ANISOTROPIC_4X);
	}

	// Setup and init first screen
	currentScreen = LOGO;
	InitScreen(currentScreen);

	SetTargetFPS(appConfig.fps_limit);
	//  DisableCursor();

	//-------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		UpdateDrawFrame();
	}

	// De-Initialization
	//-------------------------------------------------------------------------
	UnloadCurrentScreen();

	// Unload global data loaded
	// UnloadFont(font);
	// UnloadMusicStream(music);
	// UnloadSound(fxCoin);

	// CloseAudioDevice();     // Close audio context

	CloseWindow(); // Close window and OpenGL context
	//-------------------------------------------------------------------------

	return 0;
}

//-----------------------------------------------------------------------------
// Module specific Functions Definition
//-----------------------------------------------------------------------------

static void UnloadCurrentScreen(void)
{
	// Unload current screen data before closing
	switch (currentScreen)
	{
	case LOGO:
		UnloadLogoScreen();
		break;
	case GAMEPLAY:
		UnloadGameplayScreen();
		break;
	default:
		break;
	}
}

static void InitScreen(GameScreen screen) {
	switch (screen)
	{
	case LOGO:
		InitLogoScreen(resources);
		break;
	case GAMEPLAY:
		InitGameplayScreen(config, &target, consoleOut);
		break;
	default:
		break;
	}
}

// Change to next screen, no transition
static void ChangeToScreen(GameScreen screen)
{
	UnloadCurrentScreen();
	InitScreen(screen);
	currentScreen = screen;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
	// Update
	//-------------------------------------------------------------------------
	// UpdateMusicStream(music); // NOTE: Music keeps playing between screens

	switch (currentScreen)
	{
	case LOGO:
	{
		UpdateLogoScreen();
		if (FinishLogoScreen())
			ChangeToScreen(GAMEPLAY);
	}
	break;
	case GAMEPLAY:
	{
		UpdateGameplayScreen();
		// if (FinishGameplayScreen() == 1)
		// 	ChangeToScreen(ENDING);
	}
	break;
	default:
		break;
	}
	//-------------------------------------------------------------------------

	// Draw
	//-------------------------------------------------------------------------
	BeginDrawing();
	{
		ClearBackground(RAYWHITE);

		switch (currentScreen)
		{
		case LOGO:
			DrawLogoScreen();
			break;
		case GAMEPLAY:
			DrawGameplayScreen();
			break;
		default:
			break;
		}

		// DrawFPS(10, 10);
	}
	EndDrawing();

	//----------------------------------------------------------------------------------
}
