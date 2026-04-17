#include <raylib.h>
#include <time.h>
#include <stdbool.h>
#include "eng_config.h"
#include "eng_input.h"
#include "eng_gameobject.h"

#pragma once

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef struct
{
    Camera3D camera;
    InputActions actions;
} UnrealThirdPersonGameState;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

void Init_UnrealThirdPerson(RenderTexture2D *target, AppConfiguration appConfig);
int Update_UnrealThirdPerson();
void Draw_UnrealThirdPerson(const RenderTexture2D *target);
void Unload_UnrealThirdPerson();
