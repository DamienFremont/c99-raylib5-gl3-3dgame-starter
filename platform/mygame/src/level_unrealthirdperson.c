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
    char *tex_path = "resources\\models\\character_diffuse.png";
    const char texture_str[999];
    snprintf(texture_str, sizeof(texture_str), "%s\\%s", appConfig.res_path, tex_path);
    // load
    Texture2D texture = LoadTexture(texture_str);
    Model model = LoadModel(mod_str);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    return model;
}

Model LoadCubeModel(AppConfiguration appConfig)
{
    // resolve paths
    char *mod_path = "resources\\models\\SM_Cube.obj";
    const char mod_str[999];
    snprintf(mod_str, sizeof(mod_str), "%s\\%s", appConfig.res_path, mod_path);
    // resolve paths
    char *tex_path = "resources\\models\\character_diffuse.png";
    const char texture_str[999];
    snprintf(texture_str, sizeof(texture_str), "%s\\%s", appConfig.res_path, tex_path);
    // load
    Texture2D texture = LoadTexture(texture_str);
    Model model = LoadModel(mod_str);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    return model;
}

Model LoadRampModel(AppConfiguration appConfig)
{
    // resolve paths
    char *mod_path = "resources\\models\\SM_Ramp.obj";
    const char mod_str[999];
    snprintf(mod_str, sizeof(mod_str), "%s\\%s", appConfig.res_path, mod_path);
    // resolve paths
    char *tex_path = "resources\\models\\character_diffuse.png";
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

Shader LoadShaderBloom(AppConfiguration appConfig)
{
    // 	shaders[FX_BLOOM] = LoadShader(0, pathJoin(appProps.res_path, TextFormat("resources\\shaders\\glsl%i\\bloom.fs", appProps.glsl_version)));
    // resolve paths
    char *sha_path = TextFormat("resources\\shaders\\glsl%i\\bloom.fs", appConfig.glsl_version);
    const char sha_str[999];
    snprintf(sha_str, sizeof(sha_str), "%s\\%s", appConfig.res_path, sha_path);
    // load
    Shader shader = LoadShader(0, sha_str);
    return shader;
}

Shader LoadShaderDefault(AppConfiguration appConfig)
{
    // 	shaders[FX_DEFAULT] = LoadShader(0, pathJoin(appProps.res_path, TextFormat("resources\\shaders\\glsl%i\\default.fs", appProps.glsl_version)));
    // resolve paths
    char *shadef_path = TextFormat("resources\\shaders\\glsl%i\\default.fs", appConfig.glsl_version);
    const char shadef_str[999];
    snprintf(shadef_str, sizeof(shadef_str), "%s\\%s", appConfig.res_path, shadef_path);
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
    state.postproShader = (appConfig.postpro_bloom_enable == true) ? shaderBloom : shaderDefault;

    state.playerPosition = (Vector3){9.0f, 0.0f, 11.0f};
    state.model = model;

    state.rampModel = LoadRampModel(appConfig);
    state.cubeModel = LoadCubeModel(appConfig);

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
        -6.0f + state->playerPosition.x,
        1.5f + state->playerPosition.y,
        0.0f + state->playerPosition.z}; // Camera position
    state->camera.target = (Vector3){
        0.0f + state->playerPosition.x,
        1.5f + state->playerPosition.y,
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
        const Vector3 V0 = (Vector3){0, 0, 0};
        const Vector3 VY = (Vector3){0, 1, 0};
        const float F0 = 0.0f;

        DrawGrid(50, 1.0f);
        DrawCubeWiresV((Vector3){9.0f, 1.0f, 11.0f}, (Vector3){1.0f, 2.0f, 1.0f}, RED);

        DrawModelEx(state->model, state->playerPosition, VY, 90, (Vector3){0.45f, 0.45f, 0.45f}, WHITE);

        // ramp 1
        DrawModelEx(state->rampModel, (Vector3){17.0f, 0.0f, 6.0f}, VY, -90, (Vector3){2.0f, 0.3f, 4.0f}, GRAY);
        // ramp 2
        DrawModelEx(state->rampModel, (Vector3){18.0f, 0.0f, 15.0f}, VY, -90, (Vector3){2.0f, 1.0f, 4.0f}, GRAY);
        DrawModelEx(state->cubeModel, (Vector3){18.0f, 0.0f, 15.0f}, V0, F0, (Vector3){2.0f, 1.0f, 7.0f}, GRAY);
        DrawModelEx(state->rampModel, (Vector3){20.0f, 1.0f, 21.0f}, VY, -180, (Vector3){2.0f, 1.0f, 4.0f}, GRAY);
        // big cube
        DrawModelEx(state->cubeModel, (Vector3){12.0f, 0.0f, 17.0f}, V0, F0, (Vector3){6.0f, 2.0f, 5.0f}, GRAY);
        // Playgrounf
        DrawModelEx(state->cubeModel, (Vector3){0.0f, -0.5f, 0.1f}, V0, F0, (Vector3){30.0f, 0.5f, 35.0f}, LIGHTGRAY);
        DrawModelEx(state->cubeModel, (Vector3){0.0f, 0.0f, 0.0f}, V0, F0, (Vector3){30.0f, 4.0f, 1.0f}, GRAY);
        DrawModelEx(state->cubeModel, (Vector3){0.0f, 0.0f, 34.0f}, V0, F0, (Vector3){30.0f, 4.0f, 1.0f}, GRAY);
        DrawModelEx(state->cubeModel, (Vector3){1.0f, 0.0f, 1.0f}, VY, -90, (Vector3){33.0f, 4.0f, 1.0f}, GRAY);
        DrawModelEx(state->cubeModel, (Vector3){30.0f, 0.0f, 1.0f}, VY, -90, (Vector3){33.0f, 4.0f, 1.0f}, GRAY);

        // ChamferCube
        DrawModelEx(state->cubeModel, (Vector3){11.5f, 0.0f, 20.5f}, V0, F0, (Vector3){1.0f, 1.0f, 1.0f}, BLUE);
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