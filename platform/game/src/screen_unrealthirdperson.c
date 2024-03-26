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
#include "screens.h"

#include "text.h"
#include "tick.h"
#include "console.h"

#define RLIGHTS_IMPLEMENTATION
#include <rlights.h>
#include <raymath.h>

static ModelAnimation anim0 = {0};
static ModelAnimation anim1 = {0};

static Shader shader_fog = {0};
static Light light = {0};
static Vector3 light_transform = {0.0f, 9.0f, 0.0f};

static GameObject gos[LEVEL_SIZE];
static bool postprocessing = false;

static TickState animationTick = {0};
static TickState inputTick = {0};
static TickState renderTick = {0};

UnrealThirdPerson_State Init_UnrealThirdPerson(AppConfiguration appConfig, RenderTexture2D *target)
{
    char tmp[PATH_MAX];
    char tmp2[PATH_MAX];

    // init
    UnrealThirdPerson_State state = {0};
    state.showConsole = 1;
    state.appConfig = appConfig;
    state.camera = InitCamera();

    // SOURCE: https://www.raylib.com/examples/shaders/loader.html?name=shaders_postprocessing
    // shaders_postprocessing
    Shader shaderDefault = LoadShader(0, GetAssetPath(tmp, "resources/shaders/glsl%i/default.fs"));
    Shader shaderPostpro = LoadShader(0, GetAssetPath(tmp, "resources/shaders/glsl%i/bloom.fs"));
    postprocessing = appConfig.postpro_bloom_enable;
    state.postproShader = (postprocessing == true) ? shaderPostpro : shaderDefault;

    state.playerPosition = (Vector3){9.0f, 0.0f, 11.0f};

    Load_LevelTree(gos);

    Color LIGHTYELLOW = (Color){255, 255, 230, 255};
    Color sunColor = LIGHTYELLOW;

    // SOURCE: https://www.raylib.com/examples/textures/loader.html?name=textures_image_processing
    Image skyboxImg = LoadImage(GetAssetPath(tmp, "resources/images/skybox.png"));
    if (postprocessing == true)
    {
        ImageColorBrightness(&skyboxImg, -80);
    }
    ImageColorTint(&skyboxImg, sunColor);
    state.skybox = LoadSkyboxImage(appConfig, skyboxImg);

    int animCount = 0;
    anim0 = LoadModelAnimations(GetAssetPath(tmp, "resources/animations/Idle.m3d"), &animCount)[0];
    anim1 = LoadModelAnimations(GetAssetPath(tmp, "resources/animations/Running.m3d"), &animCount)[0];

    state.target = target;
    state.input_State = InitInputEvent();
    state.animCurrentFrame = 0;

    // Init_Models(gos);
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
    light = CreateLight(LIGHT_POINT, (Vector3){0, 2, 30}, Vector3Zero(), sunColor, shader);

    shader_fog = shader;

    animationTick = InitTick(25);
    inputTick = InitTick(120);
    renderTick = InitTick(30);

    StartTick(&animationTick);
    StartTick(&inputTick);
    StartTick(&renderTick);

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

void UpdatePlayerCamera(UnrealThirdPerson_State* state) {
    state->camera.position = (Vector3){
        -4.0f + state->playerPosition.x,
        1.0f + state->playerPosition.y,
        0.0f + state->playerPosition.z };
    state->camera.target = (Vector3){
        0.0f + state->playerPosition.x,
        1.0f + state->playerPosition.y,
        0.0f + state->playerPosition.z };
}

void UpdatePlayerPosition(UnrealThirdPerson_State* state) {
    gos[0].transform.translation = (Vector3){
        state->playerPosition.x,
        state->playerPosition.y,
        state->playerPosition.z };
    gos[12].transform.translation = (Vector3){
        state->playerPosition.x,
        state->playerPosition.y + 0.01f,
        state->playerPosition.z };
}

void UpdatePlayerInput(UnrealThirdPerson_State *state)
{
    if (!IsTickUpdate(&inputTick))
        return;
    else
        UpdateTick(&inputTick);

    float char_speed = 0.1f; // TODO: tickCount
    InputOut inout = ExecuteInputEvent(state->input_State, (InputConfig){
                                                               state->playerPosition,
                                                               state->showConsole,
                                                               char_speed});
    state->playerPosition.x = inout.playerPosition.x;
    state->playerPosition.y = inout.playerPosition.y;
    state->playerPosition.z = inout.playerPosition.z;
    state->showConsole = inout.showConsole;
    if (inout.animIndex != state->animIndex) {
        state->animIndex = inout.animIndex;
    }
}

void UpdatePhysics(UnrealThirdPerson_State *state)
{
    // TODO: https://www.raylib.com/examples/models/loader.html?name=models_box_collisions
}

int Update_UnrealThirdPerson(UnrealThirdPerson_State *state)
{
    UpdatePlayerInput(state);
    UpdatePlayerAnimation(state);
    UpdatePlayerCamera(state);
    UpdatePlayerPosition(state);
    UpdateRender(state);
    //UpdatePhysics(state);
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
    DrawCubeWiresV(light_transform,
                   (Vector3){1.0f, 1.0f, 1.0f}, YELLOW);
}

void Draw_2D(UnrealThirdPerson_State *state)
{
    if (state->showConsole == 1)
    {
        LogConsole(TextFormat("animationTick.current/lastUpdate: %i %i", animationTick.current, animationTick.lastUpdate));
        ConsoleConfig cfg = (ConsoleConfig){
            &state->showConsole,
            state->appConfig.fps_counter_show,
            state->appConfig.screen_width};
        DrawConsole(cfg);
    }
    else
    {
        DrawText("Press [TAB] to toggle menu", 10, 10 + 30 * 0, 20, GRAY);
        DrawText("Use keys [W][A][S][D][SPACE] to move character", 10, 10 + 30 * 1, 20, GRAY);
        DrawText("Press [F1] to toggle console", 10, 10 + 30 * 2, 20, GRAY);
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
            Draw_Component(gos[i]);
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
    UnloadModel(state->skybox); // Unload skybox model
    // UnloadShadersAll(state->shaders);
}
