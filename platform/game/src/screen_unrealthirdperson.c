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
    state.components = Load_LevelTree(appConfig), sizeof(state.components);
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