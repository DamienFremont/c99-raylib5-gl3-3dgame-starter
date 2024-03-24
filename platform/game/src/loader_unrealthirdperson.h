#include "config.h"
#include "gameobject.h"

#pragma once

typedef enum
{
    LEVEL_SIZE = 17
} UnrealThirdPersonLevelSize;

GameObject *Load_LevelTree(GameObject *gos);
