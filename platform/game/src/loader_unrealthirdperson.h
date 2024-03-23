#include "config.h"
#include "gameobject.h"

#pragma once

typedef enum
{
    LEVEL_SIZE = 12
} UnrealThirdPersonLevelSize;

GameObject *Load_LevelTree(GameObject *gos);
