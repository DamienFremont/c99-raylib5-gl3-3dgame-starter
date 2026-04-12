#include "eng_material.h"

#include "eng_config.h"
#include "eng_assets.h"

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

// FIXME:
Shader TileShader(Vector2 tiling) // TODO: REMOVEME
{
    // Set the texture tiling using a shader
    const float tilingArr[2] = {tiling.x, tiling.y};
    Shader shader = LoadShaderFile(
        0,
        "resources/shaders/glsl%i/tiling.fs");
    SetShaderValue(shader, GetShaderLocation(shader, "tiling"), tilingArr, SHADER_UNIFORM_VEC2);
    return shader;
}

Texture2D CheckboardTexture2D(float x, float y, float z) 
{
    int checkByMeter = 1;
    int textureResolutionByMeter = 64;
    int checksX= y * 32;
    int checksY= x * 1;

    int width = x * checkByMeter * textureResolutionByMeter;
    int height = y * checkByMeter * textureResolutionByMeter;

    Texture2D texture = LoadTextureFromImage(GenImageChecked(
        width, height,
        checksX, checksY,
        GRAY, DARKGRAY));

    return texture;
}

