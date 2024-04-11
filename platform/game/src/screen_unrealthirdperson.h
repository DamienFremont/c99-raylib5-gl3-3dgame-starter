#include "config.h"

#pragma once

#include <raylib.h>
#include <time.h>
#include <stdbool.h>
#include "input.h"
#include "gameobject.h"

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

void Init_UnrealThirdPerson(AppConfiguration appConfig, RenderTexture2D *target);
int Update_UnrealThirdPerson();
void Draw_UnrealThirdPerson(RenderTexture2D *target);
void Unload_UnrealThirdPerson();
