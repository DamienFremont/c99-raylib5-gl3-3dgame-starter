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

typedef struct NodeTexture
{
    int tilingX;
    int tilingY;
    Color col1;
    Color col2;
} NodeTexture;

typedef struct Node3D
{
    int id;
    char name[256];
    Transform2 transform;
    char model[1024];
    Color color;
    NodeTexture texture;
} Node3D;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

GameObject *Load_LevelTree(GameObject *gos);
Model Load_LevelSkybox(Color sunColor, bool postpro);
