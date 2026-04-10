#pragma once
#include <raylib.h>
#include "eng_config.h"

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Shader TileShader(Vector2 tiling); // TODO: REMOVEME

bool IsCheckboard(const char *str);
Texture2D CheckboardTexture2D(float x, float y, float z);