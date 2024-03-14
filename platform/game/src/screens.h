#include <raylib.h>
#include "config.h"

//-----------------------------------------------------------------------------
// Types and Structures Definition
typedef enum GameScreen
{
    LOGO = 0,
    GAMEPLAY = 1
} GameScreen;

//-----------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//-----------------------------------------------------------------------------
extern GameScreen currentScreen;

//----------------------------------------------------------------------------------
// Logo Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen(AppConfiguration appConfig, RenderTexture2D target, char consoleOut);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);
