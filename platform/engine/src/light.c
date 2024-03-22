#include "light.h"

#pragma once

#include <raylib.h>
#include "config.h"
#include "assets.h"


Shader LoadLightShader(char *RESOURCES, int GLSL_VERSION)
{
    char tmp[PATH_MAX];
    // Load basic lighting shader
    Shader shader = LoadShader(
        GetAssetPath(tmp, TextFormat("resources/shaders/glsl%i/lightbasic-vs.glsl", GLSL_VERSION)),
        GetAssetPath(tmp, TextFormat("resources/shaders/glsl%i/lightbasic-fs.glsl", GLSL_VERSION)));
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambientColor");
    Vector3 ambientColor_loc = (Vector3){1.0f, 1.0f, 0.8f};
    SetShaderValue(shader, ambientLoc, &ambientColor_loc, SHADER_UNIFORM_VEC3);
    return shader;
}
