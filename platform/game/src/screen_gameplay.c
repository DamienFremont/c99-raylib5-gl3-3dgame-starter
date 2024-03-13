#include "screens.h"

#include <raylib.h>
#include <time.h>

#include "screen_gameplay_loader.h"

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

void DrawConsole3D();
void DrawConsole2D();

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static RenderTexture2D *render_texture;
static char consoleOut;

static int showConsole;
static Shader postproShader;
static Shader tilingShader;
static Camera camera;

static Vector3 playerPosition;

static StaticMeshComponent *components;
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

Shader shaderTexture;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

void InitGameplayScreen(AppConfiguration appConfig, RenderTexture2D *t, char consoleOut)
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
    postproShader = shaderPostpro;
    playerPosition = (Vector3){9.0f, 0.0f, 11.0f};
    components = Load_LevelTree(appConfig);
    // skybox = LoadSkyboxResource(appConfig, "resources/images/skybox.png");
    modelAnimations = LoadAnimationsResource(RESOURCES, "resources/models/character.glb");
    // TODO: shaders = shaders;
    render_texture = t;
    input_State = InitInputEvent();
    animCurrentFrame = 0;
    screenWidth = GetScreenWidth();
    fps_counter_show = appConfig.fps_counter_show;

    const float SCALE_1024 = 4.0f;
    shaderTexture = TileTexture2D(RESOURCES, GLSL_VERSION, (Vector2){SCALE_1024, SCALE_1024});
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

void DrawSkybox()
{
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    {
        DrawModel(skybox, (Vector3){0, 0, 0}, 1.0f, SKYBLUE);
    }
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
}

void Draw3D()
{
    for (size_t i = 0; i < LEVEL_SIZE; i++)
        Draw_Component(components[i]);
}

void Draw3DTextured()
{
    for (size_t i = 0; i < LEVEL_SIZE; i++)
        Draw_Component(components[i]);
}

void RenderPipeline_DEFAULT()
{
    BeginMode3D(camera);
    {
        Draw3D();
        DrawConsole3D();
    }
    EndMode3D();
    DrawConsole2D();
}

void RenderPipeline_POSTPRO()
{
    // postprocessing
    BeginTextureMode(*render_texture); // Enable drawing to texture
    {
        ClearBackground(RAYWHITE); // Clear screen background
        Draw3DTextured();
        // DrawSkybox();
    }

    BeginDrawing();
    {
        ClearBackground(RAYWHITE); // Clear screen background

        // postprocessing
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_hybrid_render
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_basic_lighting
        // TODO: https://www.raylib.com/examples/shaders/loader.html?name=shaders_fog
        Texture2D texture = render_texture->texture;
        BeginShaderMode(postproShader);
        {
            // postprocess
            DrawTextureRec(                       //
                texture,                          //
                (Rectangle){                      //
                            0,                    //
                            0,                    //
                            (float)texture.width, //
                            (float)-texture.height},
                (Vector2){0, 0}, //
                WHITE);
        }
        EndShaderMode();

        DrawConsole3D();
        DrawConsole2D();
    }
    EndDrawing();
}

void RenderPipeline_TEXTURED()
{
    BeginMode3D(camera);
    {
        DrawSkybox();
        BeginShaderMode(shaderTexture);
        {
            Draw3DTextured();
        }
        EndShaderMode();
        DrawConsole3D();
    }
    EndMode3D();
    DrawConsole2D();
}

void DrawGameplayScreen()
{
    // RenderPipeline_DEFAULT();
    RenderPipeline_TEXTURED();
    // RenderPipeline_POSTPRO();
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
    if (showConsole == 1)
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
}

void DrawConsole2D()
{
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