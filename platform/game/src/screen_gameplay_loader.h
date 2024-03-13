#include "gameobject.h"
#include "config.h"

typedef enum
{
    LEVEL_SIZE = 13,
} UnrealThirdPersonLevelSize;

typedef enum
{
    MI_Grid_TopDark,
    MI_Grid_Gray,
} LevelTexture;

typedef enum
{
    PlayerStart,
    SM_Cube,
    SM_Ramp,
    SM_Cube4,
} LevelModel;

StaticMeshComponent *Load_LevelTree(AppConfiguration appConfig);
