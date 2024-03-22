#include "config.h"
#include "gameobject.h"

#pragma once

typedef enum
{
    LEVEL_SIZE = 12
    // LEVEL_SIZE = 13,
} UnrealThirdPersonLevelSize;

GameObject *Load_LevelTree();
