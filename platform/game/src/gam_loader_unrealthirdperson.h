#include <raylib.h>
#include "eng_config.h"
#include "eng_gameobject.h"

#pragma once

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    LEVEL_SIZE = 17,
    LEVEL_PLAYER_MODEL = 0,
    LEVEL_PLAYER_SHADOW = 12,
    LEVEL_PLAYER_ANIMATIONS = 2
} UnrealThirdPersonLevelSize;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

GameObject *Load_LevelTree(GameObject *gos);
Model Load_LevelSkybox(Color sunColor, bool postprocessing);
