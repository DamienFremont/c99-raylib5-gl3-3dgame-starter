#include "tiling.h"

#include "config.h"
#include "assets.h"

Shader TileTexture2D(AppConfiguration appConfig, Texture2DTiling tiling)
{
    int GLSL_VERSION = appConfig.glsl_version;
    // Set the texture tiling using a shader
    float tilingArr[2] = { tiling.x, tiling.y };
    Shader shader = LoadShaderResource(appConfig.res_path, TextFormat("resources/shaders/glsl%i/tiling.fs", GLSL_VERSION));
    SetShaderValue(shader, GetShaderLocation(shader, "tiling"), tilingArr, SHADER_UNIFORM_VEC2);
    return shader;
}
