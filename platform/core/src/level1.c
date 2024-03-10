#include "level.h"

#include <raylib.h>

LvlResources LoadLevel(AppConfiguration appConfig)
{
    // 	models[CHARACTER] = LoadModel(pathJoin(appProps.res_path, "resources\\models\\church.obj"));

    // resolve paths
    char *mod_path = "resources\\models\\character.glb";
    const char mod_str[999];
    snprintf(mod_str, sizeof(mod_str), "%s\\%s", appConfig.res_path, mod_path);

    char *tex_path = "resources\\models\\church_diffuse.png";
    const char texture_str[999];
    snprintf(texture_str, sizeof(texture_str), "%s\\%s", appConfig.res_path, tex_path);

    // load
    LvlResources res;
    res.textures[CHARACTER] = LoadTexture(texture_str);
    res.models[CHARACTER] = LoadModel(mod_str);
    res.models[CHARACTER].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = res.textures[CHARACTER];

    int animsCount = 0;
    res.modelAnimations = LoadModelAnimations(mod_str, &animsCount);

    return res;
}

void DrawLevel()
{
    DrawCubeWiresV((Vector3){0.0f, 0.5f, 1.0f}, (Vector3){1.0f, 1.0f, 1.0f}, RED);
    DrawCubeV((Vector3){0.0f, 0.5f, 1.0f}, (Vector3){1.0f, 1.0f, 1.0f}, PURPLE);
    DrawCubeWiresV((Vector3){0.0f, 0.5f, -1.0f}, (Vector3){1.0f, 1.0f, 1.0f}, DARKGREEN);
    DrawCubeV((Vector3){0.0f, 0.5f, -1.0f}, (Vector3){1.0f, 1.0f, 1.0f}, YELLOW);
}

void UnloadLevel(LvlResources lvlkRes)
{
    // Unload texture
    for (int i = 0; i < MAX_MODELS; i++)
        UnloadTexture(lvlkRes.textures[i]);
    // Unload model
    for (int i = 0; i < MAX_MODELS; i++)
        UnloadModel(lvlkRes.models[i]);
}

Vector3 PlayerPosition()
{
    return (Vector3){0.0f, 0.0f, 0.0f};
}
