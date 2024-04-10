#include "screen_unrealthirdperson.h"

#include <raylib.h>
#include <time.h>
#include "loader_unrealthirdperson.h"
#include "config.h"
#include "console.h"
#include "camera.h"
#include "input.h"
#include "control.h"
#include <stdio.h>
#include <string.h>
#include "assets.h"
#include "material.h"
#include "screens.h"
#include "text.h"
#include "tick.h"
#include "console.h"
#define RLIGHTS_IMPLEMENTATION
#include <rlights.h>
#include <raymath.h>

//----------------------------------------------------------------------------------
// Const
//----------------------------------------------------------------------------------

const int TICK_ANIMAT = 25;
const int TICK_INPUT = 120;
const int TICK_RENDER = 30;

const float MAX_WALK_SPEED = 0.08f;
const float MAX_WALK_ROTAT = 0.08f * 15;

const int CAM_DIST = 4;
const int CAM_HEIGHT = 1;

const Vector3 SCENE_FORWARD = {1, 0, 0};

// TODO: move to Load_LevelTree()
const Vector3 LIGHT_TRANSFORM = {0.0f, 9.0f, 30.0f};
const Color LIGHTYELLOW = {255, 255, 230, 255};

//----------------------------------------------------------------------------------
// Vars
//----------------------------------------------------------------------------------

// TODO: move to Load_LevelTree()
static ModelAnimation anim0 = {0};
static ModelAnimation anim1 = {0};

static Shader shader_fog = {0};
static Light light = {0};

static GameObject gos[LEVEL_SIZE];
static bool postprocessing = false;

static TickState animationTick = {0};
static TickState inputTick = {0};
static TickState renderTick = {0};

static InputActions actions;
static Controller playerController;

UnrealThirdPerson_State Init_UnrealThirdPerson(AppConfiguration appConfig, RenderTexture2D *target)
{
    char tmp[PATH_MAX];
    char tmp2[PATH_MAX];

    // init
    UnrealThirdPerson_State state = {0};
    state.showConsole = 0;
    state.appConfig = appConfig;
    state.camera = InitCamera();

    // TODO: move to Load_LevelTree()
    // SOURCE: https://www.raylib.com/examples/shaders/loader.html?name=shaders_postprocessing
    // shaders_postprocessing
    Shader shaderDefault = LoadShader(0, GetAssetPath(tmp, "resources/shaders/glsl%i/default.fs"));
    Shader shaderPostpro = LoadShader(0, GetAssetPath(tmp, "resources/shaders/glsl%i/bloom.fs"));
    postprocessing = appConfig.postpro_bloom_enable;
    state.postproShader = (postprocessing == true) ? shaderPostpro : shaderDefault;

    playerController = (Controller){
        (Vector3){9.0f, 0.0f, 11.0f},
        SCENE_FORWARD,
    };

    Load_LevelTree(gos);
    state.skybox = Load_LevelSkybox(LIGHTYELLOW, postprocessing);

    // TODO: move to Load_LevelTree()
    int animCount = 0;
    anim0 = LoadModelAnimations(GetAssetPath(tmp, "resources/animations/Idle.m3d"), &animCount)[0];
    anim1 = LoadModelAnimations(GetAssetPath(tmp, "resources/animations/Running.m3d"), &animCount)[0];

    state.target = target;
    state.animCurrentFrame = 0;

    Model model = gos[0].model;

    // SOURCE: https://www.raylib.com/examples/shaders/loader.html?name=shaders_fog
    // Load shader and set up some uniforms
    Shader shader = LoadShader(
        GetShaderPath(tmp, "resources/shaders/glsl%i/lighting.vs"),
        GetShaderPath(tmp2, "resources/shaders/glsl%i/fog.fs"));
    shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    // Ambient light level
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){0.2f, 0.2f, 0.2f, 1.0f}, SHADER_UNIFORM_VEC4);
    // Fog
    float fogDensity = 0.05f;
    int fogDensityLoc = GetShaderLocation(shader, "fogDensity");
    SetShaderValue(shader, fogDensityLoc, &fogDensity, SHADER_UNIFORM_FLOAT);
    // NOTE: All models share the same shader
    for (int i = 0; i < LEVEL_SIZE; i++)
        gos[i].model.materials[0].shader = shader;
    // Using just 1 point lights
    light = CreateLight(LIGHT_POINT, LIGHT_TRANSFORM, Vector3Zero(), LIGHTYELLOW, shader);
    shader_fog = shader;

    animationTick = InitTick(TICK_ANIMAT);
    inputTick = InitTick(TICK_INPUT);
    renderTick = InitTick(TICK_RENDER);

    StartTick(&animationTick);
    StartTick(&inputTick);
    StartTick(&renderTick);

    InitInputActions(&actions);

    return state;
}

void UpdateRender(UnrealThirdPerson_State *state)
{
    Shader shader = shader_fog;
    Camera camera = state->camera;
    // Update the light shader with the camera view position
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position.x, SHADER_UNIFORM_VEC3);
}

void UpdatePlayerAnimation(UnrealThirdPerson_State *state)
{
    if (!IsTickUpdate(&animationTick))
        return;
    else
        UpdateTick(&animationTick);
    ModelAnimation anim = state->animIndex == 0 ? anim0 : anim1;
    int animationFPS = 25; // Blender export
    int HACK = 3;          // FIXME
    int frameInMs = TIME_1_SECOND / animationFPS;
    int tickInMs = TIME_1_SECOND / animationTick.rateInHz;
    int frames = tickInMs / frameInMs;
    state->animCurrentFrame = (state->animCurrentFrame + frames * HACK) % anim.frameCount;
    UpdateModelAnimation(gos[0].model, anim, state->animCurrentFrame);
    UpdateModelAnimation(gos[12].model, anim, state->animCurrentFrame);
}

void UpdatePlayerCamera(UnrealThirdPerson_State *state)
{
    FixedCamera_Look(&state->camera, playerController, CAM_DIST, CAM_HEIGHT);
}

void UpdatePlayerPosition(UnrealThirdPerson_State *state)
{
    // player
    gos[0].transform.translation = (Vector3){
        playerController.position.x,
        playerController.position.y,
        playerController.position.z};
    gos[0].transform.rotation = (Rotation2){
        ROTATION_YAW,
        TankControl_ModelRotationAngle(playerController.direction)};
    // shadow
    gos[12].transform.translation = (Vector3){
        playerController.position.x,
        playerController.position.y + 0.01f,
        playerController.position.z};
    gos[12].transform.rotation = gos[0].transform.rotation;
}

void SetupPlayerInputComponent(UnrealThirdPerson_State *state, InputActions *actions)
{
    // console
    if (actions->ConsoleAction.State.Completed == true)
    {
        state->showConsole = !state->showConsole;
        actions->ConsoleAction.State.Completed = false;
    }
    // TODO: Jumping
    // Moving
    if (actions->MoveAction.State.Triggered == true)
        TankControl_Move(&playerController.position, actions->MoveAction.Value, MAX_WALK_SPEED, MAX_WALK_ROTAT);
    // TODO: Looking
}

void SetupPlayerAnimation(UnrealThirdPerson_State *state, InputActions *actions)
{
    state->animIndex = 0;
    if (actions->MoveAction.State.Triggered == true)
        state->animIndex = 1;
}

void UpdatePlayerInput(UnrealThirdPerson_State *state)
{
    if (!IsTickUpdate(&inputTick))
        return;
    else
        UpdateTick(&inputTick);
    ExecuteInputActions(&actions);
    SetupPlayerInputComponent(state, &actions);
    SetupPlayerAnimation(state, &actions);
}

void UpdatePhysics(UnrealThirdPerson_State *state)
{
    // TODO: https://www.raylib.com/examples/models/loader.html?name=models_box_collisions
}

int Update_UnrealThirdPerson(UnrealThirdPerson_State *state)
{
    // tick
    UpdatePlayerInput(state);
    UpdatePlayerPosition(state);
    UpdatePlayerAnimation(state);
    UpdatePlayerCamera(state);
    // TODO: UpdatePhysics(state);
    // no tick
    UpdateRender(state);
    if (IsKeyPressed(KEY_TAB))
    {
        return MENU;
    }
    return GAMEPLAY;
}

void Draw_3D_Console(UnrealThirdPerson_State *state)
{
    // grid
    DrawGrid(50, 1.0f);
    // player hitbox
    GameObject player = gos[0];
    DrawCubeWiresV((Vector3){
                       player.transform.translation.x,
                       player.transform.translation.y + 1.0f,
                       player.transform.translation.z},
                   (Vector3){1.0f, 2.0f, 1.0f}, RED);
    // gameobjects
    for (size_t i = 0; i < LEVEL_SIZE; i++)
    {
        DrawText3D(GetFontDefault(), gos[i].name, (Vector3){gos[i].transform.translation.x, gos[i].transform.translation.y + 0.01f, gos[i].transform.translation.z + 0.5f},
                   FONT_SIZE_12, FONT_SPACING_1, FONT_LINE_SPACING_1, BACKFACE_FALSE, BLACK);
    }
    // TODO: physics
    for (size_t i = 0; i < LEVEL_SIZE; i++)
    {
        DrawCubeWiresV((Vector3){
                           gos[i].transform.translation.x + gos[i].transform.scale.x / 2,
                           gos[i].transform.translation.y + gos[i].transform.scale.y / 2,
                           gos[i].transform.translation.z + gos[i].transform.scale.z / 2},
                       (Vector3){gos[i].transform.scale.x, gos[i].transform.scale.y, gos[i].transform.scale.z}, GREEN);
    }
    // light spot
    DrawCubeWiresV(LIGHT_TRANSFORM,
                   (Vector3){1.0f, 1.0f, 1.0f}, YELLOW);
}

void Draw_2D(UnrealThirdPerson_State *state)
{
    if (state->showConsole == 1)
    {
        // LogConsole(TextFormat("animationTick.current/lastUpdate: %i %i", animationTick.current, animationTick.lastUpdate));
        LogConsole(TextFormat("playerController.direction.x,y,z: %f %f %f", playerController.direction.x, playerController.direction.y, playerController.direction.z));
        DrawConsole();
    }
    else
    {
        DrawText("Press [TAB] to toggle menu", 10, 10 + 30 * 0, 20, GRAY);
        DrawText("Use keys [W][A][S][D][SPACE] or arrows to move character", 10, 10 + 30 * 1, 20, GRAY);
        DrawText("Press [F1] to toggle console", 10, 10 + 30 * 2, 20, GRAY);
    }
    if (state->appConfig.fps_counter_show == true)
    {
        DrawFPS(state->appConfig.screen_width - 100, 15);
    }
}

void Draw_3D_Skybox(UnrealThirdPerson_State *state)
{
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

void Draw_3D_Models(UnrealThirdPerson_State *state)
{
    BeginMode3D(state->camera);
    {
        for (size_t i = 0; i < LEVEL_SIZE; i++)
            Draw_GameObject(gos[i]);
        if (state->showConsole == 1)
            Draw_3D_Console(state);
    }
    EndMode3D();
}

void Draw_PostProcessing(UnrealThirdPerson_State *state, RenderTexture2D *target)
{
    BeginShaderMode(state->postproShader);
    {
        DrawTextureRec(                               //
            target->texture,                          //
            (Rectangle){                              //
                        0,                            //
                        0,                            //
                        (float)target->texture.width, //
                        (float)-target->texture.height},
            (Vector2){0, 0}, //
            WHITE);
    }
    EndShaderMode();
}

void Draw_Pipeline_Default(UnrealThirdPerson_State *state, RenderTexture2D *target)
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        Draw_3D_Skybox(state);
        Draw_3D_Models(state);
        Draw_2D(state);
    }
    EndDrawing();
}

void Draw_Pipeline_PostProcessing(UnrealThirdPerson_State *state, RenderTexture2D *target)
{
    // 1/2 Render
    BeginTextureMode(*target);
    {
        ClearBackground(RAYWHITE);

        Draw_3D_Skybox(state);
        Draw_3D_Models(state);
    }
    EndTextureMode();
    // 2/2 Draw
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        Draw_PostProcessing(state, target);
        Draw_2D(state);
    }
    EndDrawing();
}

void Draw_UnrealThirdPerson(UnrealThirdPerson_State *state, RenderTexture2D *target)
{
    if (postprocessing == true)
    {
        Draw_Pipeline_PostProcessing(state, target);
        return;
    }
    Draw_Pipeline_Default(state, target);
}

void Unload_UnrealThirdPerson(UnrealThirdPerson_State *state)
{
    // skybox
    UnloadSkybox(state->skybox);
    // player
    UnloadModelAnimation(anim0);
    UnloadModelAnimation(anim1);
    // level
    for (int i = 1; i < sizeof(gos); i++)
    {
        UnloadModel(gos[i].model);
    }
    // shaders
    UnloadShader(shader_fog);
    // TODO: UnloadShader() shaders
}
