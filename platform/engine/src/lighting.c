#include "lighting.h"

#include <raylib.h>
#include "assets.h" // PATH_MAX

Shader LoadLighting()
{
    char tmp[PATH_MAX];
    char tmp2[PATH_MAX];

    // SOURCE: https://www.raylib.com/examples/shaders/loader.html?name=shaders_fog
    // Load shader and set up some uniforms
    Shader shader = LoadShader(
        GetShaderPath(tmp, "resources/shaders/glsl%i/lighting.vs"),
        GetShaderPath(tmp2, "resources/shaders/glsl%i/fog.fs"));
    shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    // Ambient light level
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){0.2f, 0.2f, 0.2f, 1.0f}, SHADER_UNIFORM_VEC4);
    // Fog
    float fogDensity = 0.05f;
    int fogDensityLoc = GetShaderLocation(shader, "fogDensity");
    SetShaderValue(shader, fogDensityLoc, &fogDensity, SHADER_UNIFORM_FLOAT);
    return shader;
}

void UpdateLighting(Shader shader, Camera camera)
{
    // Update the light shader with the camera view position
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position.x, SHADER_UNIFORM_VEC3);
}

void UnloadLighting(Shader shader)
{
    UnloadShader(shader);
}

void SetModelLighting(Model model, Shader shader)
{
    model.materials[0].shader = shader;
}
