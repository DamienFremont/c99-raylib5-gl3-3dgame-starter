#include "screen_unrealthirdperson.h"

#include <raylib.h>
#include <time.h>
#include "loader_unrealthirdperson.h"
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

#define RLIGHTS_IMPLEMENTATION
#include <rlights.h>
#include <raymath.h>

static ModelAnimation anim0 = {0};
static ModelAnimation anim1 = {0};

static Shader shader_fog = {0};
static Light light = {0};
static Vector3 light_transform = {0.0f, 9.0f, 0.0f};

UnrealThirdPerson_State Init_UnrealThirdPerson(AppConfiguration appConfig, RenderTexture2D *target, char consoleOut)
{
    char tmp[PATH_MAX];
    // Shader
    Shader shaderDefault = LoadShader(0, GetAssetPath(tmp, "resources/shaders/glsl%i/default.fs"));
    Shader shaderPostpro = LoadShader(0, GetAssetPath(tmp, "resources/shaders/glsl%i/blur.fs"));
    // init
    UnrealThirdPerson_State state = {0};
    state.consoleOut = consoleOut;
    state.showConsole = 0;
    state.appConfig = appConfig;
    state.camera = InitCamera();
    state.postproShader = (appConfig.postpro_blur_enable == true) ? shaderPostpro : shaderDefault;
    state.playerPosition = (Vector3){9.0f, 0.0f, 11.0f};
    state.gameobjects = Load_LevelTree(appConfig), sizeof(state.gameobjects);
    state.skybox = LoadSkyboxResource(appConfig, "resources/images/skybox.png");
    int animCount = 0;
    anim0 = LoadModelAnimations(GetAssetPath(tmp, "resources/animations/Idle.m3d"), &animCount)[0];
    anim1 = LoadModelAnimations(GetAssetPath(tmp, "resources/animations/Slow_Run.m3d"), &animCount)[0];
    // TODO: state.shaders = shaders;
    state.target = target;
    state.input_State = InitInputEvent();
    state.animCurrentFrame = 0;

    Init_Models(state.gameobjects);
    Model model = state.gameobjects[0].model;

    // Load shader and set up some uniforms
    Shader shader = LoadShader(
        TextFormat("C:\\Users\\damien\\git\\cpp20-raylib-starter\\build\\desktop_win64\\Debug\\resources/shaders/glsl%i/lighting.vs", 330), // TODO
        TextFormat("C:\\Users\\damien\\git\\cpp20-raylib-starter\\build\\desktop_win64\\Debug\\resources/shaders/glsl%i/fog.fs", 330)); // TODO
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
    // model.materials[0].shader = shader;
    state.gameobjects[1].model.materials[0].shader = shader;
    state.gameobjects[2].model.materials[0].shader = shader;
    state.gameobjects[3].model.materials[0].shader = shader;
    state.gameobjects[4].model.materials[0].shader = shader;
    state.gameobjects[5].model.materials[0].shader = shader;
    state.gameobjects[6].model.materials[0].shader = shader;
    state.gameobjects[7].model.materials[0].shader = shader;
    state.gameobjects[8].model.materials[0].shader = shader;
    state.gameobjects[9].model.materials[0].shader = shader;
    state.gameobjects[10].model.materials[0].shader = shader;
    state.gameobjects[11].model.materials[0].shader = shader;
    // state.skybox.materials[0].shader = shader;

    // Using just 1 point lights
    light = CreateLight(LIGHT_POINT, (Vector3){0, 2, 6}, Vector3Zero(), GRAY, shader);

    shader_fog = shader;

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
        -4.0f + state->playerPosition.x,
        1.0f + state->playerPosition.y,
        0.0f + state->playerPosition.z};
    state->camera.target = (Vector3){
        0.0f + state->playerPosition.x,
        1.0f + state->playerPosition.y,
        0.0f + state->playerPosition.z};
    state->gameobjects[0].transform.translation = (Vector3){
        state->playerPosition.x,
        state->playerPosition.y,
        state->playerPosition.z};
    // Animation
    if (animationEnable == 1)
    {
        ModelAnimation anim = state->animIndex == 0 ? anim0 : anim1;
        state->animCurrentFrame = (state->animCurrentFrame + 1) % anim.frameCount; // TODO: tickCount
        UpdateModelAnimation(state->gameobjects[0].model, anim, state->animCurrentFrame);
    }
    // TODO: https://www.raylib.com/examples/models/loader.html?name=models_box_collisions

    Shader shader = shader_fog;
    Camera camera = state->camera;
    // Update the light shader with the camera view position
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position.x, SHADER_UNIFORM_VEC3);
}

void DrawConsole3D(UnrealThirdPerson_State *state)
{
    // grid
    DrawGrid(50, 1.0f);
    // player hitbox
    GameObject player = state->gameobjects[0];
    DrawCubeWiresV((Vector3){
                       player.transform.translation.x,
                       player.transform.translation.y + 1.0f,
                       player.transform.translation.z},
                   (Vector3){1.0f, 2.0f, 1.0f}, RED);
    // light spot
    DrawCubeWiresV(light_transform,
                   (Vector3){1.0f, 1.0f, 1.0f}, YELLOW);
}

void Texture_UnrealThirdPerson(UnrealThirdPerson_State *state)
{
    // 3D
    BeginMode3D(state->camera);
    {
        for (size_t i = 0; i < LEVEL_SIZE; i++)
            Draw_Component(state->gameobjects[i]);
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

void Draw_UnrealThirdPerson(UnrealThirdPerson_State *state, RenderTexture2D *target)
{
    // postprocessing
    // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_hybrid_render
    // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_basic_lighting
    // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_fog
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