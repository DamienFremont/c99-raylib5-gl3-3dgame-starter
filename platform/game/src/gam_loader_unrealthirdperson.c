#include "gam_loader_unrealthirdperson.h"

#include <raymath.h>
#include <raylib.h>
#include "eng_assets.h"
#include "eng_material.h"
#include "eng_skybox.h"

#include "cJSON.h"

//---------------------------------------------------------
// Local Functions Declaration
//---------------------------------------------------------

Image GetTiledImage(int tilingX, int tilingY, Color col1, Color col2);

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

GameObject *Load_LevelTree(GameObject *tree)
{
    char *playerModelPath = "resources/models/Character.m3d";

    tree[0] = (GameObject){
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
    tree[0].model.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture_GetAssetPath("resources/models/Character_Diffuse_0.png");
    // tree[0].model.materials[2].maps[MATERIAL_MAP_METALNESS].texture = LoadTexture_GetAssetPath("resources/models/Character_Metal_1.png");
    tree[0].model.materials[2].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture_GetAssetPath("resources/models/Character_Diffuse_1.png");

    // Block01
    {
        // TODO: parse the JSON data
        cJSON *json = cJSON_CreateObject();
            cJSON_AddNumberToObject(json, "id", 1);
            cJSON_AddStringToObject(json, "name", "SM_Cube4");
            cJSON_AddStringToObject(json, "model", "resources/models/SM_Cube.obj");
            cJSON_AddStringToObject(json, "color", "DARKGRAY");
            cJSON* json_transform = cJSON_AddObjectToObject(json, "transform");
                cJSON* json_tra2_trans = cJSON_AddObjectToObject(json_transform, "translation");
                    cJSON_AddNumberToObject(json_tra2_trans, "x", 12);
                    cJSON_AddNumberToObject(json_tra2_trans, "y", 0);
                    cJSON_AddNumberToObject(json_tra2_trans, "z", 17);
                cJSON* json_tra2_scale = cJSON_AddObjectToObject(json_transform, "scale");
                    cJSON_AddNumberToObject(json_tra2_scale, "x", 6);
                    cJSON_AddNumberToObject(json_tra2_scale, "y", 2);
                    cJSON_AddNumberToObject(json_tra2_scale, "z", 5);
            cJSON* json_color = cJSON_AddObjectToObject(json, "color");
                cJSON_AddNumberToObject(json_color, "r", 80);
                cJSON_AddNumberToObject(json_color, "g", 80);
                cJSON_AddNumberToObject(json_color, "b", 80);
                cJSON_AddNumberToObject(json_color, "a", 255);
            cJSON* json_texture = cJSON_AddObjectToObject(json, "texture");
                cJSON_AddNumberToObject(json_texture, "tilingX", 2);
                cJSON_AddNumberToObject(json_texture, "tilingY", 5);
                cJSON* json_texture_col1 = cJSON_AddObjectToObject(json_texture, "col1");
                    cJSON_AddNumberToObject(json_texture_col1, "r", 80);
                    cJSON_AddNumberToObject(json_texture_col1, "g", 80);
                    cJSON_AddNumberToObject(json_texture_col1, "b", 80);
                    cJSON_AddNumberToObject(json_texture_col1, "a", 255);
                cJSON* json_texture_col2 = cJSON_AddObjectToObject(json_texture, "col2");
                    cJSON_AddNumberToObject(json_texture_col2, "r", 130);
                    cJSON_AddNumberToObject(json_texture_col2, "g", 130);
                    cJSON_AddNumberToObject(json_texture_col2, "b", 130);
                    cJSON_AddNumberToObject(json_texture_col2, "a", 255);
        // access the JSON data
        Node3D node_1 = (Node3D) {0};
        strcpy(node_1.name, cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring);
        strcpy(node_1.model, cJSON_GetObjectItemCaseSensitive(json, "model")->valuestring);
        node_1.transform = (Transform2){
                (Vector3) {
                    cJSON_GetObjectItemCaseSensitive(json_tra2_trans, "x")->valueint,
                    cJSON_GetObjectItemCaseSensitive(json_tra2_trans, "y")->valueint,
                    cJSON_GetObjectItemCaseSensitive(json_tra2_trans, "z")->valueint
                },
                (Rotation2){
                    Vector3Zero(), 
                    ROTATE_ZERO},
                (Vector3){
                    cJSON_GetObjectItemCaseSensitive(json_tra2_scale, "x")->valueint,
                    cJSON_GetObjectItemCaseSensitive(json_tra2_scale, "y")->valueint,
                    cJSON_GetObjectItemCaseSensitive(json_tra2_scale, "z")->valueint
                }
        };
        node_1.color = (Color){
                cJSON_GetObjectItemCaseSensitive(json_color, "r")->valueint,
                cJSON_GetObjectItemCaseSensitive(json_color, "g")->valueint,
                cJSON_GetObjectItemCaseSensitive(json_color, "b")->valueint,
                cJSON_GetObjectItemCaseSensitive(json_color, "a")->valueint
        };
        node_1.texture = (NodeTexture){0};
        node_1.texture.tilingX = cJSON_GetObjectItemCaseSensitive(json_texture, "tilingX")->valueint;
        node_1.texture.tilingY = cJSON_GetObjectItemCaseSensitive(json_texture, "tilingY")->valueint;
        node_1.texture.col1 = (Color){
                cJSON_GetObjectItemCaseSensitive(json_texture_col1, "r")->valueint,
                cJSON_GetObjectItemCaseSensitive(json_texture_col1, "g")->valueint,
                cJSON_GetObjectItemCaseSensitive(json_texture_col1, "b")->valueint,
                cJSON_GetObjectItemCaseSensitive(json_texture_col1, "a")->valueint
        };
        node_1.texture.col2 = (Color){
                cJSON_GetObjectItemCaseSensitive(json_texture_col2, "r")->valueint,
                cJSON_GetObjectItemCaseSensitive(json_texture_col2, "g")->valueint,
                cJSON_GetObjectItemCaseSensitive(json_texture_col2, "b")->valueint,
                cJSON_GetObjectItemCaseSensitive(json_texture_col2, "a")->valueint
        };
        // free memory
        cJSON_Delete(json);
        // create GameObject
        Texture2D texture = LoadTextureFromImage(GetTiledImage(
            node_1.texture.tilingX,
            node_1.texture.tilingY,
            node_1.texture.col1,
            node_1.texture.col2
        ));
        Model model = LoadModel_GetAssetPath(node_1.model);
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
        GameObject go_1 = (GameObject){0};
        strcpy(go_1.name, node_1.name);
        go_1.transform = node_1.transform;
        go_1.model = model;
        go_1.color = node_1.color;
        // load GameObject
        tree[1] = go_1;

        // "SM_Cube7"
        tree[2] = (GameObject){
            "SM_Cube9",
            (Transform2){
                (Vector3){18.0f, 0.0f, 15.0f},
                (Rotation2){Vector3Zero(), ROTATE_ZERO},
                (Vector3){2.0f, 1.0f, 7.0f}},
            LoadModel_GetAssetPath("resources/models/SM_Cube.obj"),
            DARKGRAY};

        tree[2].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(7, 2, GRAY, DARKGRAY));

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

        tree[3].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 2, GRAY, DARKGRAY));

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

    tree[11] = (GameObject){
        "SM_Ramp",
        (Transform2){
            (Vector3){17.0f, 0.0f, 6.0f},
            (Rotation2){ROTATION_YAW, ROTATE_M90},
            (Vector3){2.0f, 0.3f, 4.0f}},
        LoadModel_GetAssetPath("resources/models/SM_Ramp.obj"),
        GRAY};

    tree[11].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 2, GRAY, DARKGRAY));

    tree[12] = (GameObject){
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
