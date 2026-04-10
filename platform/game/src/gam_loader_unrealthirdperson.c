#include "gam_loader_unrealthirdperson.h"

#include <raymath.h>
#include <raylib.h>
#include "eng_assets.h"
#include "eng_material.h"
#include "eng_skybox.h"
#include "eng_scene.h"

//---------------------------------------------------------
// Local Functions Declaration
//---------------------------------------------------------

Image GetTiledImage(int tilingX, int tilingY, Color col1, Color col2);

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

GameObject *Load_LevelTree(GameObject *tree)
{
    // Player
    char *playerModelPath = "resources/models/Character.m3d";
    tree[LEVEL_PLAYER_MODEL] = (GameObject){
        "Player",
        (Transform2){
            (Vector3){9.0f, 0.0f, 11.0f},
            (Rotation2){ROTATION_YAW, ROTATE_P90},
            (Vector3){1.0f, 1.0f, 1.0f},
        },
        LoadModel_GetAssetPath(playerModelPath),
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_lightmap
        WHITE};
    // TODO: METAL
    // tree[0].model.materials[1].maps[MATERIAL_MAP_METALNESS].texture = LoadTexture_GetAssetPath("resources/models/Character_Metal_0.png");
    tree[LEVEL_PLAYER_MODEL].model.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture_GetAssetPath("resources/models/Character_Diffuse_0.png");
    // tree[0].model.materials[2].maps[MATERIAL_MAP_METALNESS].texture = LoadTexture_GetAssetPath("resources/models/Character_Metal_1.png");
    tree[LEVEL_PLAYER_MODEL].model.materials[2].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture_GetAssetPath("resources/models/Character_Diffuse_1.png");
    // Player Shadow
    tree[LEVEL_PLAYER_SHADOW] = (GameObject){
        "PlayerShadow",
        (Transform2){
            (Vector3){
                9.0f, 0.1f, 11.0f},
            (Rotation2){
                ROTATION_YAW, ROTATE_P90},
            (Vector3){
                1.0f, 0.0f, 1.0f},
        },
        LoadModel_GetAssetPath(playerModelPath),
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_lightmap
        DARKGRAY};
        
    Scene scene = LoadSceneFile("resources/scenes/unrealthirdperson.json");

    for (int i = 0; i < scene.nodesSize; i++) 
    {
        // create GameObject
        Node3D node_1 = scene.nodes[i];
        Model model = LoadModel_GetAssetPath(node_1.model);
        GameObject go_1 = (GameObject){0};
        strcpy(go_1.name, node_1.name);
        go_1.transform = node_1.transform;
        go_1.model = model;
        go_1.color = node_1.color;

        if( IsCheckboard(node_1.material.albedo)) {
            go_1.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = CheckboardTexture2D(
                node_1.transform.scale.x,
                node_1.transform.scale.y,
                node_1.transform.scale.z);
        }

        // load GameObject
        tree[11 + i] = go_1;
    }

    // Block01
    {

        // "SM_Cube10"
        // "SM_QuarterCylinder3"
        // "SM_QuarterCylinder6"
        tree[3] = (GameObject){
            "SM_Ramp2",
            (Transform2){
                (Vector3){18.0f, 0.0f, 15.0f},
                (Rotation2){ROTATION_YAW, ROTATE_M90},
                (Vector3){2.0f, 1.0f, 4.0f}},
            LoadModel_GetAssetPath("resources/models/SM_Ramp.obj"),
            DARKGRAY};

        // tree[3].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 2, GRAY, DARKGRAY));

        tree[4] = (GameObject){
            "SM_Ramp3",
            (Transform2){
                (Vector3){20.0f, 1.0f, 21.0f},
                (Rotation2){ROTATION_YAW, ROTATE_M180},
                (Vector3){2.0f, 1.0f, 4.0f}},
            LoadModel_GetAssetPath("resources/models/SM_Ramp.obj"),
            DARKGRAY};

        tree[4].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 2, GRAY, DARKGRAY));
    }

    // Cylinder
    {
        tree[13] = (GameObject){
            "SM_QuarterCylinder11",
            (Transform2){
                (Vector3){22.0f, 0.0f, 7.0f},
                (Rotation2){Vector3Zero(), ROTATE_ZERO},
                (Vector3){4.0f, 1.0f, 4.0f}},
            LoadModel_GetAssetPath("resources/models/SM_QuarterCylinder.obj"),
            GRAY};

        tree[13].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 2, GRAY, DARKGRAY));

        tree[14] = (GameObject){
            "SM_QuarterCylinder12",
            (Transform2){
                (Vector3){22.0f, 0.0f, 7.0f},
                (Rotation2){ROTATION_YAW, ROTATE_M90},
                (Vector3){4.0f, 1.0f, 4.0f}},
            LoadModel_GetAssetPath("resources/models/SM_QuarterCylinder.obj"),
            GRAY};

        tree[14].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 1, GRAY, DARKGRAY));

        tree[15] = (GameObject){
            "SM_QuarterCylinder13",
            (Transform2){
                (Vector3){22.0f, 0.0f, 7.0f},
                (Rotation2){ROTATION_YAW, ROTATE_M180},
                (Vector3){4.0f, 1.0f, 4.0f}},
            LoadModel_GetAssetPath("resources/models/SM_QuarterCylinder.obj"),
            GRAY};

        tree[15].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 1, GRAY, DARKGRAY));

        tree[16] = (GameObject){
            "SM_QuarterCylinder14",
            (Transform2){
                (Vector3){22.0f, 0.0f, 7.0f},
                (Rotation2){ROTATION_YAW, ROTATE_M270},
                (Vector3){4.0f, 1.0f, 4.0f}},
            LoadModel_GetAssetPath("resources/models/SM_QuarterCylinder.obj"),
            GRAY};

        tree[16].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 1, GRAY, DARKGRAY));
    }

    // Playground
    {
        tree[5] = (GameObject){
            "SM_Cube",
            (Transform2){
                (Vector3){0.0f, -0.5f, 0.1f},
                (Rotation2){Vector3Zero(), ROTATE_ZERO},
                (Vector3){30.0f, 0.5f, 35.0f},
            },
            LoadModel_GetAssetPath("resources/models/SM_Cube.obj"),
            WHITE};

        tree[5].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(30, 30, WHITE, LIGHTGRAY));

        tree[6] = (GameObject){
            "SM_Cube2",
            (Transform2){
                (Vector3){0.0f, 0.0f, 0.0f},
                (Rotation2){Vector3Zero(), ROTATE_ZERO},
                (Vector3){30.0f, 4.0f, 1.0f}},
            LoadModel_GetAssetPath("resources/models/SM_Cube.obj"),
            DARKGRAY};

        tree[6].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 30, GRAY, DARKGRAY));

        tree[7] = (GameObject){
            "SM_Cube3",
            (Transform2){
                (Vector3){0.0f, 0.0f, 34.0f},
                (Rotation2){Vector3Zero(), ROTATE_ZERO},
                (Vector3){30.0f, 4.0f, 1.0f}},
            LoadModel_GetAssetPath("resources/models/SM_Cube.obj"),
            DARKGRAY};

        tree[7].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 30, GRAY, DARKGRAY));

        tree[8] = (GameObject){
            "SM_Cube5",
            (Transform2){
                (Vector3){1.0f, 0.0f, 1.0f},
                (Rotation2){ROTATION_YAW, ROTATE_M90},
                (Vector3){33.0f, 4.0f, 1.0f}},
            LoadModel_GetAssetPath("resources/models/SM_Cube.obj"),
            DARKGRAY};

        tree[8].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 33, GRAY, DARKGRAY));

        tree[9] = (GameObject){
            "SM_Cube6",
            (Transform2){
                (Vector3){30.0f, 0.0f, 1.0f},
                (Rotation2){ROTATION_YAW, ROTATE_M90},
                (Vector3){33.0f, 4.0f, 1.0f}},
            LoadModel_GetAssetPath("resources/models/SM_Cube.obj"),
            DARKGRAY};

        tree[9].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 33, GRAY, DARKGRAY));
    }
    tree[10] = (GameObject){
        "SM_ChamferCube",
        (Transform2){
            (Vector3){11.0f, 0.0f, 20.5f},
            (Rotation2){Vector3Zero(), ROTATE_ZERO},
            (Vector3){1.0f, 1.0f, 1.0f}},
        LoadModel_GetAssetPath("resources/models/SM_Cube.obj"),
        BLUE};

    tree[2] = (GameObject){
        "SM_Ramp",
        (Transform2){
            (Vector3){17.0f, 0.0f, 6.0f},
            (Rotation2){ROTATION_YAW, ROTATE_M90},
            (Vector3){2.0f, 0.3f, 4.0f}},
        LoadModel_GetAssetPath("resources/models/SM_Ramp.obj"),
        GRAY};

    tree[2].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 2, GRAY, DARKGRAY));

    return tree;
}

Model Load_LevelSkybox(Color sunColor, bool postpro)
{
    // SOURCE: https://www.raylib.com/examples/textures/loader.html?name=textures_image_processing
    Image skyboxImg = LoadImage_GetAssetPath("resources/images/skybox.png");
    if (postpro == true)
    {
        ImageColorBrightness(&skyboxImg, -80);
    }
    ImageColorTint(&skyboxImg, sunColor);
    Model skybox = LoadSkyboxFromImage(skyboxImg);
    UnloadImage(skyboxImg);
    return skybox;
}

//---------------------------------------------------------
// Local Functions Definition
//---------------------------------------------------------

Image GetTiledImage(int tilingX, int tilingY, Color col1, Color col2)
{
    const int res = 1024;
    const int check = 256;
    return GenImageChecked(res, res, check / tilingX, check / tilingY, col1, col2);
}
