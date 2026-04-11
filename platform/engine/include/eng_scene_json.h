#pragma once
#include <cjson.h>
#include "eng_scene.h"

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Scene ParseSceneJson(const cJSON* json);
