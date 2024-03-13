#include "skybox.h"

#include <raylib.h>
#include "config.h"
#include "assets.h"

Model LoadSkyboxResource(AppConfiguration appConfig, char *assetpath)
{
    Mesh cube = GenMeshCube(500.0f, 500.0f, 500.0f);
    int GLSL_VERSION = appConfig.glsl_version; // move to compile time declaration
    Model skybox = LoadModelFromMesh(cube);
    // Load skybox shader and set required locations
    // NOTE: Some locations are automatically set at shader loading
    skybox.materials[0].shader = LoadShaderResource2(appConfig.res_path,
                                                     TextFormat("resources/shaders/glsl%i/skybox.vs", GLSL_VERSION),
                                                     TextFormat("resources/shaders/glsl%i/skybox.fs", GLSL_VERSION));
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), (int[1]){MATERIAL_MAP_CUBEMAP}, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), (int[1]){0}, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), (int[1]){0}, SHADER_UNIFORM_INT);
    // Load cubemap shader and setup required shader locations
    Shader shdrCubemap = LoadShaderResource2(appConfig.res_path,
                                             TextFormat("resources/shaders/glsl%i/cubemap.vs", GLSL_VERSION),
                                             TextFormat("resources/shaders/glsl%i/cubemap.fs", GLSL_VERSION));
    SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), (int[1]){0}, SHADER_UNIFORM_INT);
    // Load img texture
    Image img = LoadImageResource(appConfig.res_path, assetpath);
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT); // CUBEMAP_LAYOUT_PANORAMA
    UnloadImage(img);

    return skybox;
}