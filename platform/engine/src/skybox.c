#include "skybox.h"

#include <raylib.h>
#include "config.h"

#include "assets.h"

Model LoadSkyboxResource(AppConfiguration appConfig, char *assetpath)
{
    char tmp[PATH_MAX];
    char tmp2[PATH_MAX];

    Mesh cube = GenMeshCube(500.0f, 500.0f, 500.0f);
    int GLSL_VERSION = appConfig.glsl_version; // move to compile time declaration
    Model skybox = LoadModelFromMesh(cube);
    // Load skybox shader and set required locations
    // NOTE: Some locations are automatically set at shader loading
    skybox.materials[0].shader = LoadShader(
        GetAssetPath(tmp, TextFormat("resources/shaders/glsl%i/skybox.vs", GLSL_VERSION)),
        GetAssetPath(tmp2, TextFormat("resources/shaders/glsl%i/skybox.fs", GLSL_VERSION)));
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), (int[1]){MATERIAL_MAP_CUBEMAP}, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), (int[1]){0}, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), (int[1]){0}, SHADER_UNIFORM_INT);
    // Load cubemap shader and setup required shader locations
    Shader shdrCubemap = LoadShader(
        GetAssetPath(tmp, TextFormat("resources/shaders/glsl%i/cubemap.vs", GLSL_VERSION)),
        GetAssetPath(tmp2, TextFormat("resources/shaders/glsl%i/cubemap.fs", GLSL_VERSION)));
    SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), (int[1]){0}, SHADER_UNIFORM_INT);
    // Load img texture
    Image img = LoadImage(GetAssetPath(tmp, assetpath));
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT); // CUBEMAP_LAYOUT_PANORAMA
    UnloadImage(img);

    return skybox;
}