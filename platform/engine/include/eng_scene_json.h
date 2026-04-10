#include <cjson.h>
#include "eng_scene.h"

#pragma once

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    SCENE_NODES_SIZE = 128,
} SceneConst;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Scene Parse_SceneJson(const cJSON* json);
