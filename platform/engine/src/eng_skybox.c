#include "eng_skybox.h"

#include <raylib.h>
#include "eng_assets.h"

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

Model LoadSkyboxFromImage(Image img)
{
    char tmp[PATH_MAX];
    char tmp2[PATH_MAX];

    Mesh cube = GenMeshCube(500.0f, 500.0f, 500.0f);
    Model skybox = LoadModelFromMesh(cube);
    // Load skybox shader and set required locations
    // NOTE: Some locations are automatically set at shader loading
    skybox.materials[0].shader = LoadShader(
        GetAssetPath(tmp, "resources/shaders/glsl%i/skybox.vs"),
        GetAssetPath(tmp2, "resources/shaders/glsl%i/skybox.fs"));
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), (int[1]){MATERIAL_MAP_CUBEMAP}, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), (int[1]){0}, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), (int[1]){0}, SHADER_UNIFORM_INT);
    // Load cubemap shader and setup required shader locations
    Shader shdrCubemap = LoadShader(
        GetAssetPath(tmp, "resources/shaders/glsl%i/cubemap.vs"),
        GetAssetPath(tmp2, "resources/shaders/glsl%i/cubemap.fs"));
    SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), (int[1]){0}, SHADER_UNIFORM_INT);
    // Load img texture
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT); // CUBEMAP_LAYOUT_PANORAMA
    return skybox;
}

void Draw_3D_Skybox(Model skybox, Camera camera)
{
    BeginMode3D(camera);
    {
        rlDisableBackfaceCulling();
        rlDisableDepthMask();
        {
            DrawModel(skybox, (Vector3){0, 0, 0}, 1.0f, SKYBLUE);
        }
        rlEnableBackfaceCulling();
        rlEnableDepthMask();
    }
    EndMode3D();
}

Model UnloadSkybox(Model skybox)
{
    UnloadShader(skybox.materials[0].shader);
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(skybox); // Unload skybox model
}