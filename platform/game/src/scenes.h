#include "scene_unrealthirdperson.h"

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen
{
    LAUNCHER = 0,
    UNREAL_THIRDPERSON = 1
} GameScreen;

//----------------------------------------------------------------------------------
// Screen Functions Declaration
//----------------------------------------------------------------------------------
UnrealThirdPerson_State Init_UnrealThirdPerson(AppConfiguration appConfig, RenderTexture2D *target, char *consoleOut);

int Update_UnrealThirdPerson(UnrealThirdPerson_State *state);

void Texture_UnrealThirdPerson(UnrealThirdPerson_State *state);

void Draw_UnrealThirdPerson(UnrealThirdPerson_State *state, RenderTexture2D target);

void Unload_UnrealThirdPerson(UnrealThirdPerson_State *state);
