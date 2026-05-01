#include "gam_loader_unrealthirdperson.h"

#include <raymath.h>
#include <raylib.h>
#include <string.h>
#include "eng_assets.h"
#include "eng_material.h"
#include "eng_skybox.h"
#include "eng_scene.h"

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

bool strstartsWith(const char *str, const char *prefix) {
    // Handle NULL inputs to avoid undefined behavior
    if (str == NULL || prefix == NULL) {
        return false;
    }
    size_t str_len = strlen(str);
    size_t prefix_len = strlen(prefix);
    // If prefix is longer than str, it can't be a prefix
    if (prefix_len > str_len) {
        return false;
    }
    // Compare first 'prefix_len' characters
    return (strncmp(str, prefix, prefix_len) == 0);
}

GameObject *Load_LevelTree(GameObject *tree)
{
    // Player
    char *playerModelPath = "resources/models/Character.m3d";
    tree[LEVEL_PLAYER_MODEL] = (GameObject){
        "Player",
        (Transform2){
            (Vector3){9.0f, 0.0f, 11.0f},
            (Rotation2){AXIS_YAW, ANGLE_P90},
            (Vector3){1.0f, 1.0f, 1.0f},
        },
        LoadModelFile(playerModelPath),
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_lightmap
        WHITE};
    // TODO: METAL
    // tree[0].model.materials[1].maps[MATERIAL_MAP_METALNESS].texture = LoadTextureFile("resources/models/Character_Metal_0.png");
    tree[LEVEL_PLAYER_MODEL].model.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFile("resources/models/Character_Diffuse_0.png");
    // tree[0].model.materials[2].maps[MATERIAL_MAP_METALNESS].texture = LoadTextureFile("resources/models/Character_Metal_1.png");
    tree[LEVEL_PLAYER_MODEL].model.materials[2].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFile("resources/models/Character_Diffuse_1.png");
    // Player Shadow
    tree[LEVEL_PLAYER_SHADOW] = (GameObject){
        "PlayerShadow",
        (Transform2){
            (Vector3){
                9.0f, 0.1f, 11.0f},
            (Rotation2){
                AXIS_YAW, ANGLE_P90},
            (Vector3){
                1.0f, 0.0f, 1.0f},
        },
        LoadModelFile(playerModelPath),
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_lightmap
        DARKGRAY};
        
    Scene scene = LoadSceneFile("resources/scenes/unrealthirdperson.json");
    for (int i = 0; i < scene.nodesSize; i++) 
    {
        // create GameObject
        Node3D node_1 = scene.nodes[i];
        Model model = LoadModelFile(node_1.model);
        GameObject go_1 = (GameObject){0};
        strcpy_s(go_1.name, sizeof(go_1.name), node_1.name);
        go_1.transform = node_1.transform;
        go_1.model = model;
        go_1.color = node_1.color;
        if (strcmp("CHECKBOARD", node_1.texture) == 0)
            go_1.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = CheckboardTexture2D(
                GRAY, DARKGRAY,
                node_1.transform.scale.x, 
                node_1.transform.scale.y, 
                node_1.transform.scale.z);
        else if (strcmp("CHECKBOARD_LIGHT", node_1.texture) == 0)
            go_1.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = CheckboardTexture2D(
                WHITE, LIGHTGRAY,
                node_1.transform.scale.x, 
                node_1.transform.scale.y, 
                node_1.transform.scale.z);
        else if(strstartsWith(node_1.texture, "resources")) {
            go_1.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFile(node_1.texture);
            // go_1.model.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFile(node_1.texture);
            // go_1.model.materials[2].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFile(node_1.texture);
        }
        // load GameObject
        tree[2 + i] = go_1;
    }
    return tree;
}

Model Load_LevelSkybox(Color sunColor, bool postpro)
{
    // SOURCE: https://www.raylib.com/examples/textures/loader.html?name=textures_image_processing
    Image skyboxImg = LoadImageFile("resources/images/skybox.png");
    if (postpro == true)
    {
        ImageColorBrightness(&skyboxImg, -80);
    }
    ImageColorTint(&skyboxImg, sunColor);
    Model skybox = LoadSkyboxFromImage(skyboxImg);
    UnloadImage(skyboxImg);
    return skybox;
}
