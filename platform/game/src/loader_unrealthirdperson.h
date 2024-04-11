#include "config.h"
#include "gameobject.h"

#pragma once

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    LEVEL_SIZE = 17
} UnrealThirdPersonLevelSize;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

GameObject *Load_LevelTree(GameObject *gos);
Model Load_LevelSkybox(Color sunColor, bool postprocessing);
