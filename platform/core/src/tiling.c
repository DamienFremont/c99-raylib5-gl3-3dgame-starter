#include "tiling.h"

#include "config.h"
#include "assets.h"

Shader TileTexture2D(char *RESOURCES, int GLSL_VERSION, Vector2 tiling)
{
    // Set the texture tiling using a shader
    float tilingArr[2] = { tiling.x, tiling.y };
    Shader shader = LoadShaderResource(RESOURCES, TextFormat("resources/shaders/glsl%i/tiling.fs", GLSL_VERSION));
    SetShaderValue(shader, GetShaderLocation(shader, "tiling"), tilingArr, SHADER_UNIFORM_VEC2);
    return shader;
}
