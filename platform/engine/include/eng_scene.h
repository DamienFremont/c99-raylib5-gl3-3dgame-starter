#include <raylib.h>
#include "eng_gameobject.h"

#pragma once

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    SCENE_NODES_SIZE = 128,
    ALBEDO_STR_LENGTH = 256,
    NAME_STR_LENGTH = 256,
    MODEL_STR_LENGTH = 1024, // TODO: CHANGE TO PATH_STR_LENGTH
} SceneConst;

typedef struct NodeMaterial
{
    char albedo[ALBEDO_STR_LENGTH];
} NodeMaterial;

typedef struct Node3D
{
    int id;
    char name[NAME_STR_LENGTH];
    Transform2 transform;
    char model[MODEL_STR_LENGTH];
    Color color;
    NodeMaterial material;
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
