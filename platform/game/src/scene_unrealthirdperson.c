#include "scene_unrealthirdperson.h"

#include <raylib.h>
#include <time.h>

#include "config.h"
#include "console.h"
#include "camera_thirdperson.h"
#include "input.h"
#include <stdio.h>
#include <string.h>
#include "assets.h"
#include "skybox.h"
#include "material.h"
#include "light.h"
#include <raymath.h>

typedef enum
{
    FX_DEFAULT,
    FX_BLOOM,
} PostproShader;

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

StaticMeshComponent *Load_LevelTree(AppConfiguration appConfig)
{
    int GLSL_VERSION = appConfig.glsl_version;
    char *RESOURCES = appConfig.res_path;
    const Vector3 VECTOR__Y_ = (Vector3){0, 1, 0};
    const float SCALE_1024 = 4.0f;
    // Texture2D
    Texture2D texture = LoadTextureResource(RESOURCES, "resources/models/character_diffuse.png");
    Texture2D MI_Grid_Gray = LoadTextureResource(RESOURCES, "resources/models/MI_Grid_Gray-1024.png");
    Texture2D MI_Grid_TopDark = LoadTextureResource(RESOURCES, "resources/models/MI_Grid_TopDark-1024.png");

    StaticMeshComponent tree[LEVEL_SIZE];
    tree[0] = (StaticMeshComponent){
        "Player",
        (Transform2){
            (Vector3){
                9.0f, 0.0f, 11.0f},
            (Rotation2){
                VECTOR__Y_, ROTATE_P90},
            (Vector3){
                0.45f, 0.45f, 0.45f},
        },
        LoadModelResource(RESOURCES, "resources/models/character.glb"),
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_lightmap
        WHITE,
        (Material2){
            MATERIAL2_COLOR,
            texture}};

    // Block01
    {
        tree[1] = (StaticMeshComponent){
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
        tree[2] = (StaticMeshComponent){
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
        tree[3] = (StaticMeshComponent){
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
        tree[4] = (StaticMeshComponent){
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
        tree[5] = (StaticMeshComponent){
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
        tree[6] = (StaticMeshComponent){
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
        tree[7] = (StaticMeshComponent){
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
        tree[8] = (StaticMeshComponent){
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
        tree[9] = (StaticMeshComponent){
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
    tree[10] = (StaticMeshComponent){
        "SM_ChamferCube",
        (Transform2){
            (Vector3){11.5f, 0.0f, 20.5f},
            (Rotation2){Vector3Zero(), ROTATE_ZERO},
            (Vector3){1.0f, 1.0f, 1.0f}},
        LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
        BLUE,
        (Material2){MATERIAL2_COLOR}};
    tree[11] = (StaticMeshComponent){
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

    tree[12] = (StaticMeshComponent){
        "Test Ligth",
        (Transform2){
            (Vector3){12.0f, 0.0f, 13.0f},
            (Rotation2){Vector3Zero(), ROTATE_ZERO},
            (Vector3){2.0f, 2.0f, 2.0f}},
        LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
        WHITE,
        (Material2){
            MATERIAL2_TEXTURESHADER,
            MI_Grid_TopDark,
            LoadLightShader(RESOURCES, GLSL_VERSION)}};
            // TODO: second material with alpha (texture)
            // TODO: third material with netal
    return tree;
}

UnrealThirdPerson_State Init_UnrealThirdPerson(AppConfiguration appConfig, RenderTexture2D *target, char consoleOut)
{
    int GLSL_VERSION = appConfig.glsl_version;
    char *RESOURCES = appConfig.res_path;
    // Shader
    Shader shaderDefault = LoadShaderResource(RESOURCES, TextFormat("resources/shaders/glsl%i/default.fs", GLSL_VERSION));
    Shader shaderPostpro = LoadShaderResource(RESOURCES, TextFormat("resources/shaders/glsl%i/blur.fs", GLSL_VERSION));
    // init
    UnrealThirdPerson_State state = {0};
    state.consoleOut = consoleOut;
    state.showConsole = 0;
    state.appConfig = appConfig;
    state.camera = InitCamera();
    state.postproShader = (appConfig.postpro_blur_enable == true) ? shaderPostpro : shaderDefault;
    state.playerPosition = (Vector3){9.0f, 0.0f, 11.0f};
    memcpy(state.components, Load_LevelTree(appConfig), sizeof(state.components));
    state.skybox = LoadSkyboxResource(appConfig, "resources/images/skybox.png");
    state.modelAnimations = LoadAnimationsResource(RESOURCES, "resources/models/character.glb");
    // TODO: state.shaders = shaders;
    state.target = target;
    state.input_State = InitInputEvent();
    state.animCurrentFrame = 0;
    return state;
}

int Update_UnrealThirdPerson(UnrealThirdPerson_State *state)
{
    int animationEnable = 1;
    // Input
    float char_speed = 0.05f; // TODO: tickCount
    InputOut inout = ExecuteInputEvent(state->input_State, (InputConfig){
                                                               state->playerPosition,
                                                               state->showConsole,
                                                               char_speed});
    state->playerPosition.x = inout.playerPosition.x;
    state->playerPosition.y = inout.playerPosition.y;
    state->playerPosition.z = inout.playerPosition.z;
    state->showConsole = inout.showConsole;
    state->animIndex = inout.animIndex;
    // Action
    state->camera.position = (Vector3){
        -6.0f + state->playerPosition.x,
        1.5f + state->playerPosition.y,
        0.0f + state->playerPosition.z};
    state->camera.target = (Vector3){
        0.0f + state->playerPosition.x,
        1.5f + state->playerPosition.y,
        0.0f + state->playerPosition.z};
    state->components[0].transform.translation = (Vector3){
        state->playerPosition.x,
        state->playerPosition.y,
        state->playerPosition.z};
    // Animation
    if (animationEnable == 1)
    {
        ModelAnimation anim = state->modelAnimations[state->animIndex];
        state->animCurrentFrame = (state->animCurrentFrame + 1) % anim.frameCount; // TODO: tickCount
        UpdateModelAnimation(state->components[0].staticMesh, anim, state->animCurrentFrame);
    }
    // TODO: https://www.raylib.com/examples/models/loader.html?name=models_box_collisions
}

void DrawConsole3D(UnrealThirdPerson_State *state)
{
    // grid
    DrawGrid(50, 1.0f);
    // player hitbox
    StaticMeshComponent player = state->components[0];
    DrawCubeWiresV((Vector3){
                       player.transform.translation.x,
                       player.transform.translation.y + 1.0f,
                       player.transform.translation.z},
                   (Vector3){1.0f, 2.0f, 1.0f}, RED);
}

void Texture_UnrealThirdPerson(UnrealThirdPerson_State *state)
{
    // 3D
    BeginMode3D(state->camera);
    {
        for (size_t i = 0; i < LEVEL_SIZE; i++)
            Draw_Component(state->components[i]);
        if (state->showConsole == 1)
            DrawConsole3D(state);
    }
    EndMode3D();
    // Skybox
    BeginMode3D(state->camera);
    {
        rlDisableBackfaceCulling();
        rlDisableDepthMask();
        {
            DrawModel(state->skybox, (Vector3){0, 0, 0}, 1.0f, SKYBLUE);
        }
        rlEnableBackfaceCulling();
        rlEnableDepthMask();
    }
    EndMode3D();
}

void Draw_UnrealThirdPerson(UnrealThirdPerson_State *state, RenderTexture2D target)
{
    // postprocessing
    // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_hybrid_render
    // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_basic_lighting
    // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_fog
    BeginShaderMode(state->postproShader);
    {
        DrawTextureRec(                              //
            target.texture,                          //
            (Rectangle){                             //
                        0,                           //
                        0,                           //
                        (float)target.texture.width, //
                        (float)-target.texture.height},
            (Vector2){0, 0}, //
            WHITE);
    }
    EndShaderMode();

    // 2D
    if (state->showConsole == 1)
    {
        ConsoleConfig cfg = (ConsoleConfig){
            state->showConsole,
            state->appConfig.fps_counter_show,
            state->appConfig.screen_width,
            state->consoleOut};
        DrawConsole(cfg);
    }
}

void Unload_UnrealThirdPerson(UnrealThirdPerson_State *state)
{
    UnloadModel(state->skybox); // Unload skybox model
    // UnloadShadersAll(state->shaders);
}