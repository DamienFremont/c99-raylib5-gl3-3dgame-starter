#pragma once
#include <raylib.h>
#include "eng_config.h"

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Shader TileShader(Vector2 tiling); // TODO: REMOVE ME
Texture2D CheckboardTexture2D(Color col1, Color col2, float x, float y, float z);
