#pragma once
#include <raylib.h>
#include "eng_gameobject.h"

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    SCENE_NODES_SIZE = 128,
    NAME_STR_LENGTH = 256,
    MODEL_STR_LENGTH = 1024, // TODO: CHANGE TO PATH_STR_LENGTH
} SceneConst;

typedef struct Node3D
{
    int id;
    char name[NAME_STR_LENGTH];
    Transform2 transform;
    char model[MODEL_STR_LENGTH];
    Color color;
    Texture2D texture;
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
