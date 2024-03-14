#include "screens.h"

#include <raylib.h>
#include <time.h>

#include "config.h"
#include "console.h"
#include "camera_thirdperson.h"
#include "gameobject.h"
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

typedef enum
{
    LEVEL_SIZE = 13,
} UnrealThirdPersonLevelSize;

StaticMeshComponent *Load_LevelTree(AppConfiguration appConfig);
void DrawConsole3D();

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static RenderTexture2D target;
static char consoleOut;

static int showConsole;
static Shader postproShader;
static Shader tilingShader;
static Camera camera;

static Vector3 playerPosition;

static StaticMeshComponent components[LEVEL_SIZE];
static Model cubeModel;
static Model rampModel;
static Model chamferCubeModel;

static Model skybox;

static Shader *shaders;
static InputEvent_State input_State;
static int animIndex;
static ModelAnimation *modelAnimations;
static unsigned int animCurrentFrame;
int screenWidth;
bool fps_counter_show;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

void InitGameplayScreen(AppConfiguration appConfig, RenderTexture2D t, char consoleOut)
{
    int GLSL_VERSION = appConfig.glsl_version;
    char *RESOURCES = appConfig.res_path;
    // Shader
    Shader shaderDefault = LoadShaderResource(RESOURCES, TextFormat("resources/shaders/glsl%i/default.fs", GLSL_VERSION));
    Shader shaderPostpro = LoadShaderResource(RESOURCES, TextFormat("resources/shaders/glsl%i/blur.fs", GLSL_VERSION));
    // init
    consoleOut = consoleOut;
    showConsole = 0;
    camera = InitCamera();
    postproShader = shaderDefault;
    playerPosition = (Vector3){9.0f, 0.0f, 11.0f};
    memcpy(components, Load_LevelTree(appConfig), sizeof(components));
    skybox = LoadSkyboxResource(appConfig, "resources/images/skybox.png");
    modelAnimations = LoadAnimationsResource(RESOURCES, "resources/models/character.glb");
    // TODO: shaders = shaders;
    target = t;
    input_State = InitInputEvent();
    animCurrentFrame = 0;
    screenWidth = GetScreenWidth();
    fps_counter_show = appConfig.fps_counter_show;
}

void UpdateGameplayScreen()
{
    int animationEnable = 1;
    // Input
    float char_speed = 0.05f; // TODO: tickCount
    InputOut inout = ExecuteInputEvent(input_State, (InputConfig){
                                                        playerPosition,
                                                        showConsole,
                                                        char_speed});
    playerPosition.x = inout.playerPosition.x;
    playerPosition.y = inout.playerPosition.y;
    playerPosition.z = inout.playerPosition.z;
    showConsole = inout.showConsole;
    animIndex = inout.animIndex;
    // Action
    camera.position = (Vector3){
        -6.0f + playerPosition.x,
        1.5f + playerPosition.y,
        0.0f + playerPosition.z};
    camera.target = (Vector3){
        0.0f + playerPosition.x,
        1.5f + playerPosition.y,
        0.0f + playerPosition.z};
    components[0].transform.translation = (Vector3){
        playerPosition.x,
        playerPosition.y,
        playerPosition.z};
    // Animation
    if (animationEnable == 1)
    {
        ModelAnimation anim = modelAnimations[animIndex];
        animCurrentFrame = (animCurrentFrame + 1) % anim.frameCount; // TODO: tickCount
        UpdateModelAnimation(components[0].staticMesh, anim, animCurrentFrame);
    }
    // TODO: https://www.raylib.com/examples/models/loader.html?name=models_box_collisions
}

void DrawGameplayScreen()
{
    BeginTextureMode(target); // Enable drawing to texture
    {
        ClearBackground(RAYWHITE); // Clear texture background

        // 3D
        BeginMode3D(camera);
        {
            for (size_t i = 0; i < LEVEL_SIZE; i++)
                Draw_Component(components[i]);
            if (showConsole == 1)
                DrawConsole3D();
        }
        EndMode3D();
        // Skybox
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
    EndTextureMode();

    BeginDrawing();
    {
        ClearBackground(RAYWHITE); // Clear screen background

        // postprocessing
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_hybrid_render
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_basic_lighting
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_fog
        BeginShaderMode(postproShader);
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
        if (showConsole == 1)
        {
            ConsoleConfig cfg = (ConsoleConfig){
                showConsole,
                fps_counter_show,
                screenWidth,
                consoleOut};
            DrawConsole(cfg);
        }
    }
    EndDrawing();
}

void UnloadGameplayScreen()
{
    UnloadModel(skybox); // Unload skybox model
    // TODO: UnloadShadersAll(shaders);
}

//----------------------------------------------------------------------------------

// TODO: MOVEME
void DrawConsole3D()
{
    // grid
    DrawGrid(50, 1.0f);
    // player hitbox
    StaticMeshComponent player = components[0];
    DrawCubeWiresV((Vector3){
                       player.transform.translation.x,
                       player.transform.translation.y + 1.0f,
                       player.transform.translation.z},
                   (Vector3){1.0f, 2.0f, 1.0f}, RED);
}

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