#pragma once
#include <raylib.h>
#include "eng_gameobject.h"
#include "eng_config.h"

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    SCENE_NODES_SIZE = 128,
    NAME_STR_LENGTH = 256
} SceneConst;

typedef struct Node3D
{
    int id;
    char name[NAME_STR_LENGTH];
    Transform2 transform;
    char model[PATH_STR_LENGTH];
    Color color;
    char texture[NAME_STR_LENGTH];
} Node3D;

typedef struct Scene
{
    int name;
    Node3D* nodes;
    int nodesSize;
} Scene;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Scene LoadSceneFile(const char *fileName);
