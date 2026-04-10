#include <raylib.h>
#include "eng_gameobject.h"

#pragma once

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef struct NodeMaterial
{
    char albedo[256];
} NodeMaterial;

typedef struct Node3D
{
    int id;
    char name[256];
    Transform2 transform;
    char model[1024];
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
