#include "material.h"

#include "config.h"
#include "assets.h"


Shader TileTexture2D(Vector2 tiling)
{
    // Set the texture tiling using a shader
    float tilingArr[2] = {tiling.x, tiling.y};
    char fs_path[PATH_MAX];
    GetAssetPath(fs_path, "resources/shaders/glsl%i/tiling.fs");
    Shader shader = LoadShader(0, fs_path);
    SetShaderValue(shader, GetShaderLocation(shader, "tiling"), tilingArr, SHADER_UNIFORM_VEC2);
    return shader;
}
