#include "level_unrealthirdperson.h"

#include <raylib.h>
#include <time.h>

#include "config.h"
#include "console.h"
#include "camera.h"
#include "input.h"

#include <stdio.h>
#include <string.h>
#include "assets.h"
#include "skybox.h"
#include "tiling.h"

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

    const Vector3 VECTOR_ZERO = (Vector3){0, 0, 0};
    const Vector3 VECTOR__Y_ = (Vector3){0, 1, 0};

    const float SCALE_1024 = 4.0f;

    // Texture2D
    Texture2D texture = LoadTextureResource(appConfig.res_path, "resources/models/character_diffuse.png");
    Texture2D floorTexture = LoadTextureResource(appConfig.res_path, "resources/models/MI_Grid_Gray-1024.png");
    Texture2D wallTexture = LoadTextureResource(appConfig.res_path, "resources/models/MI_Grid_TopDark-1024.png");

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
        LoadModelResource(appConfig.res_path, "resources/models/character.glb"),
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_lightmap
        WHITE,
        (Material2){
            MATERIAL2_COLOR,
            texture}};
    tree[1] = (StaticMeshComponent){
        "Floor",
        (Transform2){
            (Vector3){0.0f, -0.5f, 0.1f},
            (Rotation2){VECTOR_ZERO, ROTATE_ZERO},
            (Vector3){30.0f, 0.5f, 35.0f},
        },
        LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
        LIGHTGRAY,
        (Material2){
            MATERIAL2_TEXTURESHADER,
            floorTexture,
            TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){140.0f, 140.0f})}};
    tree[2] = (StaticMeshComponent){
        "WallFront",
        (Transform2){
            (Vector3){30.0f, 0.0f, 1.0f},
            (Rotation2){VECTOR__Y_, ROTATE_M90}, 
            (Vector3){33.0f, 4.0f, 1.0f}
        },
        LoadModelResource(RESOURCES, "resources/models/SM_Cube.obj"),
        GRAY,
        (Material2){
            MATERIAL2_TEXTURESHADER,
            wallTexture,
            TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){4 * SCALE_1024, 33 * SCALE_1024})}};
    return tree;
}

UnrealThirdPerson_State Init_UnrealThirdPerson(AppConfiguration appConfig, RenderTexture2D *target, char consoleOut)
{
    int GLSL_VERSION = appConfig.glsl_version;
    char *RESOURCES = appConfig.res_path;

    // Texture2D
    // Texture2D texture = LoadTextureResource(appConfig.res_path, "resources/models/character_diffuse.png");
    Texture2D wallTexture = LoadTextureResource(appConfig.res_path, "resources/models/MI_Grid_TopDark-1024.png");
    // Model
    Model skybox = LoadSkyboxResource(appConfig, "resources/skybox.png");

    Model cubeModel = LoadModelResource(appConfig.res_path, "resources/models/SM_Cube.obj");
    cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = wallTexture;

    Model rampModel = LoadModelResource(appConfig.res_path, "resources/models/SM_Ramp.obj");
    rampModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = wallTexture;
    // Texture2DTiling
    Shader floorTextureTiler2 = TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){4 * 4.0f, 2 * 4.0f});
    rampModel.materials[0].shader = floorTextureTiler2;

    Model chamferCubeModel = LoadModelResource(appConfig.res_path, "resources/models/SM_Cube.obj");

    // ModelAnimation
    ModelAnimation *modelAnimations = LoadAnimationsResource(RESOURCES, "resources/models/character.glb");
    // Shader
    Shader shaderDefault = LoadShaderResource(RESOURCES, TextFormat("resources/shaders/glsl%i/default.fs", GLSL_VERSION));
    Shader shaderPostpro = LoadShaderResource(RESOURCES, TextFormat("resources/shaders/glsl%i/blur.fs", GLSL_VERSION));
    // other
    Camera camera = InitCamera();
    InputEvent_State input_State = InitInputEvent();
    // init
    UnrealThirdPerson_State state = {0};
    state.consoleOut = consoleOut;
    state.showConsole = 0;
    state.appConfig = appConfig;
    state.camera = camera;
    state.postproShader = (appConfig.postpro_blur_enable == true) ? shaderPostpro : shaderDefault;
    state.playerPosition = (Vector3){9.0f, 0.0f, 11.0f};
    memcpy(state.components, Load_LevelTree(appConfig), sizeof(state.components));
    state.rampModel = rampModel;
    state.cubeModel = cubeModel;
    state.chamferCubeModel = chamferCubeModel;
    state.skybox = skybox;
    state.modelAnimations = modelAnimations;
    // state.shaders = shaders;
    state.target = target;
    state.input_State = input_State;
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

void Texture_UnrealThirdPerson(UnrealThirdPerson_State *state)
{

    const Vector3 VY = (Vector3){0, 1, 0};

    // 3D
    BeginMode3D(state->camera);
    {
        const Vector3 V0 = (Vector3){0, 0, 0};
        const float F0 = 0.0f;

        // DrawGrid(50, 1.0f);
        //  DrawCubeWiresV((Vector3){9.0f, 1.0f, 11.0f}, (Vector3){1.0f, 2.0f, 1.0f}, RED);

        for (size_t i = 0; i < LEVEL_SIZE; i++)
        {
            Draw_Component(state->components[i]);
        }
        // Draw_Component(state->modelComp);
        // Draw_Component(state->floorComp);

        // ramp 1
        DrawModelEx(state->rampModel, (Vector3){17.0f, 0.0f, 6.0f}, VY, -90, (Vector3){2.0f, 0.3f, 4.0f}, GRAY);
        // ramp 2
        DrawModelEx(state->rampModel, (Vector3){18.0f, 0.0f, 15.0f}, VY, -90, (Vector3){2.0f, 1.0f, 4.0f}, GRAY);
        DrawModelEx(state->cubeModel, (Vector3){18.0f, 0.0f, 15.0f}, V0, F0, (Vector3){2.0f, 1.0f, 7.0f}, GRAY);
        DrawModelEx(state->rampModel, (Vector3){20.0f, 1.0f, 21.0f}, VY, -180, (Vector3){2.0f, 1.0f, 4.0f}, GRAY);
        // big cube
        DrawModelEx(state->cubeModel, (Vector3){12.0f, 0.0f, 17.0f}, V0, F0, (Vector3){6.0f, 2.0f, 5.0f}, GRAY);
        // walls
        DrawModelEx(state->cubeModel, (Vector3){0.0f, 0.0f, 0.0f}, V0, F0, (Vector3){30.0f, 4.0f, 1.0f}, GRAY);
        DrawModelEx(state->cubeModel, (Vector3){0.0f, 0.0f, 34.0f}, V0, F0, (Vector3){30.0f, 4.0f, 1.0f}, GRAY);
        DrawModelEx(state->cubeModel, (Vector3){1.0f, 0.0f, 1.0f}, VY, -90, (Vector3){33.0f, 4.0f, 1.0f}, GRAY);
        // DrawModelEx(state->cubeModel, (Vector3){30.0f, 0.0f, 1.0f}, VY, -90, (Vector3){33.0f, 4.0f, 1.0f}, GRAY);
        // floor
        // ChamferCube
        DrawModelEx(state->chamferCubeModel, (Vector3){11.5f, 0.0f, 20.5f}, V0, F0, (Vector3){1.0f, 1.0f, 1.0f}, BLUE);
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
    ConsoleConfig cfg = (ConsoleConfig){
        state->showConsole,
        state->appConfig.fps_counter_show,
        state->appConfig.screen_width,
        state->consoleOut};
    DrawConsole(cfg);
}

void Unload_UnrealThirdPerson(UnrealThirdPerson_State *state)
{
    UnloadModel(state->skybox); // Unload skybox model
    // UnloadShadersAll(state->shaders);
}