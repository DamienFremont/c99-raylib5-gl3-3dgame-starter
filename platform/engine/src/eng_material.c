#include "eng_material.h"

#include "eng_config.h"
#include "eng_assets.h"

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

// FIXME:
Shader TileTexture2D(Vector2 tiling)
{
    // Set the texture tiling using a shader
    float tilingArr[2] = {tiling.x, tiling.y};
    Shader shader = LoadShader_GetShaderPath(
        0,
        "resources/shaders/glsl%i/tiling.fs");
    SetShaderValue(shader, GetShaderLocation(shader, "tiling"), tilingArr, SHADER_UNIFORM_VEC2);
    return shader;
}
