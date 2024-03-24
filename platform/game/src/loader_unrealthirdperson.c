#include "loader_unrealthirdperson.h"

#include "assets.h"
#include "material.h"
#include <raymath.h>
#include <raylib.h>

#pragma once

Image GetTiledImage(int tilingX, int tilingY, Color col1, Color col2)
{
    const int res = 1024;
    const int check = 256;
    return GenImageChecked(res, res, check / tilingX, check / tilingY, col1, col2);
}

GameObject *Load_LevelTree(GameObject *tree)
{
    const Vector3 VECTOR__Y_ = (Vector3){0, 1, 0};
    const float SCALE_1024 = 4.0f;
    char tmp[PATH_MAX];
    // Texture2D
    Texture2D diff1 = LoadTexture(GetAssetPath(tmp, "resources/models/X_Bot_Beta_Surface_diffuse.png"));
    Texture2D diff2 = LoadTexture(GetAssetPath(tmp, "resources/models/X_Bot_Beta_Joints_diffuse.png"));

    tree[0] = (GameObject){
        "Player",
        (Transform2){
            (Vector3){
                9.0f, 0.0f, 11.0f},
            (Rotation2){
                VECTOR__Y_, ROTATE_P90},
            (Vector3){
                1.0f, 1.0f, 1.0f},
        },
        LoadModel(GetAssetPath(tmp, "resources/models/X_Bot.m3d")),
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_lightmap
        WHITE};

    tree[0].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = diff1;
    tree[0].model.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = diff1;
    tree[0].model.materials[2].maps[MATERIAL_MAP_DIFFUSE].texture = diff2;

    // Block01
    {
        tree[1] = (GameObject){
            "SM_Cube4",
            (Transform2){
                (Vector3){12.0f, 0.0f, 17.0f},
                (Rotation2){Vector3Zero(), ROTATE_ZERO},
                (Vector3){6.0f, 2.0f, 5.0f}},
            LoadModel(GetAssetPath(tmp, "resources/models/SM_Cube.obj")),
            DARKGRAY};

        tree[1].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(2, 5, GRAY, DARKGRAY));

        // "SM_Cube7"
        tree[2] = (GameObject){
            "SM_Cube9",
            (Transform2){
                (Vector3){18.0f, 0.0f, 15.0f},
                (Rotation2){Vector3Zero(), ROTATE_ZERO},
                (Vector3){2.0f, 1.0f, 7.0f}},
            LoadModel(GetAssetPath(tmp, "resources/models/SM_Cube.obj")),
            DARKGRAY};

        tree[2].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(7, 2, GRAY, DARKGRAY));

        // "SM_Cube10"
        // "SM_QuarterCylinder3"
        // "SM_QuarterCylinder6"
        tree[3] = (GameObject){
            "SM_Ramp2",
            (Transform2){
                (Vector3){18.0f, 0.0f, 15.0f},
                (Rotation2){VECTOR__Y_, ROTATE_M90},
                (Vector3){2.0f, 1.0f, 4.0f}},
            LoadModel(GetAssetPath(tmp, "resources/models/SM_Ramp.obj")),
            DARKGRAY};

        tree[3].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 2, GRAY, DARKGRAY));

        tree[4] = (GameObject){
            "SM_Ramp3",
            (Transform2){
                (Vector3){20.0f, 1.0f, 21.0f},
                (Rotation2){VECTOR__Y_, ROTATE_M180},
                (Vector3){2.0f, 1.0f, 4.0f}},
            LoadModel(GetAssetPath(tmp, "resources/models/SM_Ramp.obj")),
            DARKGRAY};

        tree[4].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 2, GRAY, DARKGRAY));
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
            LoadModel(GetAssetPath(tmp, "resources/models/SM_Cube.obj")),
            WHITE};

        tree[5].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(30, 30, WHITE, LIGHTGRAY));

        tree[6] = (GameObject){
            "SM_Cube2",
            (Transform2){
                (Vector3){0.0f, 0.0f, 0.0f},
                (Rotation2){Vector3Zero(), ROTATE_ZERO},
                (Vector3){30.0f, 4.0f, 1.0f}},
            LoadModel(GetAssetPath(tmp, "resources/models/SM_Cube.obj")),
            DARKGRAY};

        tree[6].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 30, GRAY, DARKGRAY));

        tree[7] = (GameObject){
            "SM_Cube3",
            (Transform2){
                (Vector3){0.0f, 0.0f, 34.0f},
                (Rotation2){Vector3Zero(), ROTATE_ZERO},
                (Vector3){30.0f, 4.0f, 1.0f}},
            LoadModel(GetAssetPath(tmp, "resources/models/SM_Cube.obj")),
            DARKGRAY};

        tree[7].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 30, GRAY, DARKGRAY));

        tree[8] = (GameObject){
            "SM_Cube5",
            (Transform2){
                (Vector3){1.0f, 0.0f, 1.0f},
                (Rotation2){VECTOR__Y_, ROTATE_M90},
                (Vector3){33.0f, 4.0f, 1.0f}},
            LoadModel(GetAssetPath(tmp, "resources/models/SM_Cube.obj")),
            DARKGRAY};

        tree[8].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 33, GRAY, DARKGRAY));

        tree[9] = (GameObject){
            "SM_Cube6",
            (Transform2){
                (Vector3){30.0f, 0.0f, 1.0f},
                (Rotation2){VECTOR__Y_, ROTATE_M90},
                (Vector3){33.0f, 4.0f, 1.0f}},
            LoadModel(GetAssetPath(tmp, "resources/models/SM_Cube.obj")),
            DARKGRAY};

        tree[9].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 33, GRAY, DARKGRAY));
    }
    tree[10] = (GameObject){
        "SM_ChamferCube",
        (Transform2){
            (Vector3){11.5f, 0.0f, 20.5f},
            (Rotation2){Vector3Zero(), ROTATE_ZERO},
            (Vector3){1.0f, 1.0f, 1.0f}},
        LoadModel(GetAssetPath(tmp, "resources/models/SM_Cube.obj")),
        BLUE};

    tree[11] = (GameObject){
        "SM_Ramp",
        (Transform2){
            (Vector3){17.0f, 0.0f, 6.0f},
            (Rotation2){VECTOR__Y_, ROTATE_M90},
            (Vector3){2.0f, 0.3f, 4.0f}},
        LoadModel(GetAssetPath(tmp, "resources/models/SM_Ramp.obj")),
        GRAY};

    tree[11].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GetTiledImage(4, 2, GRAY, DARKGRAY));

    tree[12] = (GameObject){
        "PlayerShadow",
        (Transform2){
            (Vector3){
                9.0f, 0.1f, 11.0f},
            (Rotation2){
                VECTOR__Y_, ROTATE_P90},
            (Vector3){
                1.0f, 0.0f, 1.0f},
        },
        LoadModel(GetAssetPath(tmp, "resources/models/X_Bot.m3d")),
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_lightmap
        BLACK};

    return tree;
}