#include <cjson.h>
#include <raylib.h>
#include "eng_gameobject.h"

#pragma once

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

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

cJSON* Read_SceneJsonFile(const char *fileName);
Node3D* Parse_SceneJson(const cJSON *json);
