#include "level_unrealthirdperson.h"

#include <raylib.h>
#include <time.h>
#include "config.h"
#include "console.h"
#include "camera.h"
#include "input.h"

typedef enum
{
    FX_DEFAULT = 0,
    FX_BLOOM,
} PostproShader;

Model LoadModels2(AppConfiguration appConfig)
{
    // 	models[CHARACTER] = LoadModel(pathJoin(appProps.res_path, "resources\\models\\church.obj"));
    // resolve paths
    char *mod_path = "resources\\models\\character.glb";
    const char mod_str[999];
    snprintf(mod_str, sizeof(mod_str), "%s\\%s", appConfig.res_path, mod_path);
    // resolve paths
    char *tex_path = "resources\\models\\church_diffuse.png";
    const char texture_str[999];
    snprintf(texture_str, sizeof(texture_str), "%s\\%s", appConfig.res_path, tex_path);
    // load
    Texture2D texture = LoadTexture(texture_str);
    Model model = LoadModel(mod_str);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    return model;
}
ModelAnimation *LoadAnimations2(AppConfiguration appConfig)
{
    // resolve paths
    char *mod_path = "resources\\models\\character.glb";
    const char mod_str[999];
    snprintf(mod_str, sizeof(mod_str), "%s\\%s", appConfig.res_path, mod_path);
    // load
    int animsCount = 0;
    ModelAnimation *modelAnimations = LoadModelAnimations(mod_str, &animsCount);
    return modelAnimations;
}
Shader LoadShaderBloom(AppConfiguration appProps)
{
    // 	shaders[FX_BLOOM] = LoadShader(0, pathJoin(appProps.res_path, TextFormat("resources\\shaders\\glsl%i\\bloom.fs", appProps.glsl_version)));
    // resolve paths
    char *sha_path = TextFormat("resources\\shaders\\glsl%i\\bloom.fs", appProps.glsl_version);
    const char sha_str[999];
    snprintf(sha_str, sizeof(sha_str), "%s\\%s", appProps.res_path, sha_path);
    // load
    Shader shader = LoadShader(0, sha_str);
    return shader;
}

Shader LoadShaderDefault(AppConfiguration appProps)
{
    // 	shaders[FX_DEFAULT] = LoadShader(0, pathJoin(appProps.res_path, TextFormat("resources\\shaders\\glsl%i\\default.fs", appProps.glsl_version)));
    // resolve paths
    char *shadef_path = TextFormat("resources\\shaders\\glsl%i\\default.fs", appProps.glsl_version);
    const char shadef_str[999];
    snprintf(shadef_str, sizeof(shadef_str), "%s\\%s", appProps.res_path, shadef_path);
    // load
    Shader shader = LoadShader(0, shadef_str);
    return shader;
}

void UnloadShaders2(Shader *shaders)
{
    for (int i = 0; i < sizeof(shaders); i++)
        UnloadShader(shaders[i]);
}

UnrealThirdPerson_State Init_UnrealThirdPerson(AppConfiguration appConfig, RenderTexture2D *target, char consoleOut)
{
    // load
    Model model = LoadModels2(appConfig);
    ModelAnimation *modelAnimations = LoadAnimations2(appConfig);
    Shader shaderBloom = LoadShaderBloom(appConfig);
    Shader shaderDefault = LoadShaderDefault(appConfig);
    InputEvent_State input_State = InitInputEvent();
    Camera camera = InitCamera();
    // init
    UnrealThirdPerson_State state = {0};
    state.consoleOut = consoleOut;
    state.showConsole = 1;
    state.appConfig = appConfig;
    state.camera = camera;
    state.playerPosition = (Vector3){0.0f, 0.0f, 0.0f};
    state.postproShader = (appConfig.postpro_bloom_enable == true) ? shaderBloom : shaderDefault;
    state.model = model;
    // state.shaders = shaders;
    state.target = target;
    state.input_State = input_State;
    state.modelAnimations = modelAnimations;
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
        0.0f + state->playerPosition.x,
        1.0f + state->playerPosition.y,
        -1.0f + state->playerPosition.z}; // Camera position
    state->camera.target = (Vector3){
        0.0f + state->playerPosition.x,
        0.25f + state->playerPosition.y,
        0.0f + state->playerPosition.z}; // Camera looking at point
    // Animation
    if (animationEnable == 1)
    {
        ModelAnimation anim = state->modelAnimations[state->animIndex];
        state->animCurrentFrame = (state->animCurrentFrame + 1) % anim.frameCount; // TODO: tickCount
        UpdateModelAnimation(state->model, anim, state->animCurrentFrame);
    }
}

void Texture_UnrealThirdPerson(UnrealThirdPerson_State *state)
{
    // 3D
    BeginMode3D(state->camera);
    {
        DrawModel(state->model, state->playerPosition, 0.1f, WHITE);
        DrawCubeWiresV((Vector3){0.0f, 0.5f, 1.0f}, (Vector3){1.0f, 1.0f, 1.0f}, RED);
        DrawCubeV((Vector3){0.0f, 0.5f, 1.0f}, (Vector3){1.0f, 1.0f, 1.0f}, PURPLE);
        DrawCubeWiresV((Vector3){0.0f, 0.5f, -1.0f}, (Vector3){1.0f, 1.0f, 1.0f}, DARKGREEN);
        DrawCubeV((Vector3){0.0f, 0.5f, -1.0f}, (Vector3){1.0f, 1.0f, 1.0f}, YELLOW);
        DrawGrid(10, 1.0f);
    }
    EndMode3D();
}

void Draw_UnrealThirdPerson(UnrealThirdPerson_State *state, RenderTexture2D target)
{
    // postprocessing
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
    // UnloadShaders2(state->shaders);
}