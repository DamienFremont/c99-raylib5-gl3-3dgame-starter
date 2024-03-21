#include "loader_unrealthirdperson.h"

#include "assets.h"
#include "material.h"
#include "light.h"
#include <raymath.h>

#pragma once

typedef enum
{
    MI_Grid_TopDark,
    MI_Grid_Gray,
} LevelTexture;

typedef enum
{
    PlayerStart,
    SM_Cube,
    SM_Ramp,
    SM_Cube4,
} LevelModel;

GameObject *Load_LevelTree(AppConfiguration appConfig)
{
    int GLSL_VERSION = appConfig.glsl_version;
    char *RESOURCES = appConfig.res_path;
    const Vector3 VECTOR__Y_ = (Vector3){0, 1, 0};
    const float SCALE_1024 = 4.0f;
    // Texture2D
    Texture2D diff1 = LoadTextureResource(RESOURCES, "resources/models/X_Bot_Beta_Surface_diffuse.png");
    Texture2D diff2 = LoadTextureResource(RESOURCES, "resources/models/X_Bot_Beta_Joints_diffuse.png");
    Texture2D MI_Grid_Gray = LoadTextureResource(RESOURCES, "resources/models/MI_Grid_Gray-1024.png");
    Texture2D MI_Grid_TopDark = LoadTextureResource(RESOURCES, "resources/models/MI_Grid_TopDark-1024.png");

    GameObject tree[LEVEL_SIZE];
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
        LoadModelResource(RESOURCES, "resources/models/X_Bot.m3d"),
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_lightmap
        WHITE,
        (Material2){
            MATERIAL2_TEXTURE,
            diff1}};

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
            LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
            GRAY,
            (Material2){
                MATERIAL2_TEXTURESHADER,
                MI_Grid_TopDark,
                TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){2 * SCALE_1024, 5 * SCALE_1024})}};
        // "SM_Cube7"
        tree[2] = (GameObject){
            "SM_Cube9",
            (Transform2){
                (Vector3){18.0f, 0.0f, 15.0f},
                (Rotation2){Vector3Zero(), ROTATE_ZERO},
                (Vector3){2.0f, 1.0f, 7.0f}},
            LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
            GRAY,
            (Material2){
                MATERIAL2_TEXTURESHADER,
                MI_Grid_TopDark,
                TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){7 * SCALE_1024, 2 * SCALE_1024})}};
        // "SM_Cube10"
        // "SM_QuarterCylinder3"
        // "SM_QuarterCylinder6"
        tree[3] = (GameObject){
            "SM_Ramp2",
            (Transform2){
                (Vector3){18.0f, 0.0f, 15.0f},
                (Rotation2){VECTOR__Y_, ROTATE_M90},
                (Vector3){2.0f, 1.0f, 4.0f}},
            LoadModelResource(RESOURCES, "resources/models/SM_Ramp.obj"),
            GRAY,
            (Material2){
                MATERIAL2_TEXTURESHADER,
                MI_Grid_TopDark,
                TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){4 * SCALE_1024, 2 * SCALE_1024})}};
        tree[4] = (GameObject){
            "SM_Ramp3",
            (Transform2){
                (Vector3){20.0f, 1.0f, 21.0f},
                (Rotation2){VECTOR__Y_, ROTATE_M180},
                (Vector3){2.0f, 1.0f, 4.0f}},
            LoadModelResource(RESOURCES, "resources/models/SM_Ramp.obj"),
            GRAY,
            (Material2){
                MATERIAL2_TEXTURESHADER,
                MI_Grid_TopDark,
                TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){4 * SCALE_1024, 2 * SCALE_1024})}};
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
            LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
            LIGHTGRAY,
            (Material2){
                MATERIAL2_TEXTURESHADER,
                MI_Grid_Gray,
                TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){30 * SCALE_1024, 35 * SCALE_1024})}};
        tree[6] = (GameObject){
            "SM_Cube2",
            (Transform2){
                (Vector3){0.0f, 0.0f, 0.0f},
                (Rotation2){Vector3Zero(), ROTATE_ZERO},
                (Vector3){30.0f, 4.0f, 1.0f}},
            LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
            GRAY,
            (Material2){
                MATERIAL2_TEXTURESHADER,
                MI_Grid_TopDark,
                TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){4 * SCALE_1024, 30 * SCALE_1024})}};
        tree[7] = (GameObject){
            "SM_Cube3",
            (Transform2){
                (Vector3){0.0f, 0.0f, 34.0f},
                (Rotation2){Vector3Zero(), ROTATE_ZERO},
                (Vector3){30.0f, 4.0f, 1.0f}},
            LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
            GRAY,
            (Material2){
                MATERIAL2_TEXTURESHADER,
                MI_Grid_TopDark,
                TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){4 * SCALE_1024, 30 * SCALE_1024})}};
        tree[8] = (GameObject){
            "SM_Cube5",
            (Transform2){
                (Vector3){1.0f, 0.0f, 1.0f},
                (Rotation2){VECTOR__Y_, ROTATE_M90},
                (Vector3){33.0f, 4.0f, 1.0f}},
            LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
            GRAY,
            (Material2){
                MATERIAL2_TEXTURESHADER,
                MI_Grid_TopDark,
                TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){4 * SCALE_1024, 33 * SCALE_1024})}};
        tree[9] = (GameObject){
            "SM_Cube6",
            (Transform2){
                (Vector3){30.0f, 0.0f, 1.0f},
                (Rotation2){VECTOR__Y_, ROTATE_M90},
                (Vector3){33.0f, 4.0f, 1.0f}},
            LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
            GRAY,
            (Material2){
                MATERIAL2_TEXTURESHADER,
                MI_Grid_TopDark,
                TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){4 * SCALE_1024, 33 * SCALE_1024})}};
    }
    tree[10] = (GameObject){
        "SM_ChamferCube",
        (Transform2){
            (Vector3){11.5f, 0.0f, 20.5f},
            (Rotation2){Vector3Zero(), ROTATE_ZERO},
            (Vector3){1.0f, 1.0f, 1.0f}},
        LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
        BLUE,
        (Material2){MATERIAL2_COLOR}};
    tree[11] = (GameObject){
        "SM_Ramp",
        (Transform2){
            (Vector3){17.0f, 0.0f, 6.0f},
            (Rotation2){VECTOR__Y_, ROTATE_M90},
            (Vector3){2.0f, 0.3f, 4.0f}},
        LoadModelResource(RESOURCES, "resources/models/SM_Ramp.obj"),
        GRAY,
        (Material2){
            MATERIAL2_TEXTURESHADER,
            MI_Grid_TopDark,
            TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){4 * SCALE_1024, 2 * SCALE_1024})}};

    // tree[12] = (GameObject){
    //     "Test Ligth",
    //     (Transform2){
    //         (Vector3){12.0f, 0.0f, 13.0f},
    //         (Rotation2){Vector3Zero(), ROTATE_ZERO},
    //         (Vector3){2.0f, 2.0f, 2.0f}},
    //     LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
    //     WHITE,
    //     (Material2){
    //         MATERIAL2_TEXTURESHADER,
    //         MI_Grid_TopDark,
    //         LoadLightShader(RESOURCES, GLSL_VERSION)}};
    //  TODO: second material with alpha (texture)
    //  TODO: third material with netal
    return tree;
}