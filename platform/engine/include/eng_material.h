#pragma once
#include <raylib.h>
#include "eng_config.h"

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Shader TileShader(Vector2 tiling); // TODO: REMOVE ME

bool IsCheckboard(const char *str); // TODO: MOVE ME
Texture2D CheckboardTexture2D(float x, float y, float z);