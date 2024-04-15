#include "gam_screen_unrealthirdperson.h"

#include <raylib.h>
#define RLIGHTS_IMPLEMENTATION
#include <rlights.h>
#include "gam_screens.h"
#include "gam_loader_unrealthirdperson.h"
#include "eng_config.h"
#include "eng_console.h"
#include "eng_camera.h"
#include "eng_input.h"
#include "eng_control.h"
#include "eng_assets.h"
#include "eng_gameobject.h"
#include "eng_text.h"
#include "eng_tick.h"
#include "eng_console.h"
#include "eng_lighting.h"
#include "eng_skybox.h"
#include "eng_render.h"

//---------------------------------------------------------
// Local Variables Definition (local to this module)
//---------------------------------------------------------

// TODO: move to Load_LevelTree()
const float CAM_FOV = 60.0f;
const Vector3 CAM_TRS = {0, 1.5, 4};
const Vector3 LIGHT_TRANSFORM = {0.0f, 9.0f, 39.0f};
const Color LIGHT_COLOR = {255, 255, 230, 255}; // YELLOW
Camera camera;
Model skybox;
Shader light_shader = {0};
Light light_point = {0};
GameObject gos[LEVEL_SIZE];

bool postpro = false;
Shader postproShader = {0};
RenderTexture2D *postproTarget;
TickState animationTick = {0};
TickState inputTick = {0};
TickState renderTick = {0};
InputActions actions;
Controller playerController;
ModelAnimation playerAnimations[2];

bool showConsole;
bool fps_counter_show;

int animIndex;
unsigned int animCurrentFrame;

//---------------------------------------------------------
// Local Functions Declaration
//---------------------------------------------------------

void Draw_Pipeline_Default();
void Draw_Pipeline_PostProcessing(RenderTexture2D *target);
void UpdatePlayerAnimation();
void UpdatePlayerCamera();
void UpdatePlayerPosition();
void UpdatePlayerInput();
void UpdateRender();
void Init_PostProcess(RenderTexture2D *target, bool postprocessing_enable);
void Init_Animation();
void Init_Lighting();

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

void Init_UnrealThirdPerson(RenderTexture2D *target, AppConfiguration appConfig)
{
    showConsole = 0;
    fps_counter_show = appConfig.fps_counter_show;
    camera = InitCamera(CAM_FOV, CAM_TRS);
    Init_PostProcess(target, appConfig.postpro_effect_bloom);
    Load_LevelTree(gos);
    skybox = Load_LevelSkybox(LIGHT_COLOR, postpro);
    Init_Animation();
    Init_Lighting();
    // TICKS
    animationTick = InitTick(25);
    inputTick = InitTick(120);
    renderTick = InitTick(30);
    StartTick(&animationTick);
    StartTick(&inputTick);
    StartTick(&renderTick);
    // PLAYER
    // TODO: move to Load_LevelTree()
    playerController = (Controller){
        gos[0].transform.translation, // player position
        (Vector3){1, 0, 0},           // screen forward
    };
    InitInputActions(&actions);
}

int Update_UnrealThirdPerson()
{
    // tick
    UpdatePlayerInput();
    UpdatePlayerPosition();
    UpdatePlayerAnimation();
    UpdatePlayerCamera();
    // TODO: UpdatePhysics();
    // no tick
    UpdateRender();
    if (IsKeyPressed(KEY_TAB))
    {
        return MENU;
    }
    return GAMEPLAY;
}

void Draw_UnrealThirdPerson(RenderTexture2D *target)
{
    if (postpro == true)
    {
        Draw_Pipeline_PostProcessing(target);
        return;
    }
    Draw_Pipeline_Default();
}

void Unload_UnrealThirdPerson()
{
    // skybox
    // FIXME: UnloadSkybox(skybox);
    // animations
    // FIXME: UnloadModelAnimations(playerAnimations, LEVEL_PLAYER_ANIMATIONS);
    // level
    for (int i = 0; i < LEVEL_SIZE; i++)
        UnloadModel(gos[i].model);
    // shaders
    UnloadShader(light_shader);
    UnloadShader(postproShader);
}

//---------------------------------------------------------
// Local Functions Definition
//---------------------------------------------------------

void UpdateRender()
{
    UpdateLighting(light_shader, camera);
}

void UpdatePlayerAnimation()
{
    if (!IsTickUpdate(&animationTick))
        return;
    else
        UpdateTick(&animationTick);
    ModelAnimation anim = animIndex == 0 ? playerAnimations[0] : playerAnimations[1];
    int animationFPS = 25; // Blender export
    int HACK = 3;          // FIXME
    int frameInMs = TIME_1_SECOND / animationFPS;
    int tickInMs = TIME_1_SECOND / animationTick.rateInHz;
    int frames = tickInMs / frameInMs;
    animCurrentFrame = (animCurrentFrame + frames * HACK) % anim.frameCount;
    UpdateModelAnimation(gos[LEVEL_PLAYER_MODEL].model, anim, animCurrentFrame);
    UpdateModelAnimation(gos[LEVEL_PLAYER_SHADOW].model, anim, animCurrentFrame);
}

void UpdatePlayerCamera()
{
    CameraFixed_Look(&camera, playerController);
}

void UpdatePlayerPosition()
{
    // player
    gos[LEVEL_PLAYER_MODEL].transform.translation = (Vector3){
        playerController.position.x,
        playerController.position.y,
        playerController.position.z};
    gos[LEVEL_PLAYER_MODEL].transform.rotation = (Rotation2){
        ROTATION_YAW,
        TankControl_ModelRotationAngle(playerController.direction)};
    // shadow
    gos[LEVEL_PLAYER_SHADOW].transform.translation = (Vector3){
        playerController.position.x,
        playerController.position.y + 0.01f,
        playerController.position.z};
    gos[LEVEL_PLAYER_SHADOW].transform.rotation = gos[0].transform.rotation;
}

void SetupPlayerInputComponent(InputActions *actions)
{
    // console
    if (actions->ConsoleAction.State.Completed == true)
    {
        showConsole = !showConsole;
        actions->ConsoleAction.State.Completed = false;
    }
    // TODO: Jumping
    // Moving
    if (actions->MoveAction.State.Triggered == true)
        TankControl_Move(&playerController.position, actions->MoveAction.Value, 0.08f, 0.08f * 15);
    // TODO: Looking
}

void SetupPlayerAnimation(InputActions *actions)
{
    animIndex = 0;
    if (actions->MoveAction.State.Triggered == true)
        animIndex = 1;
}

void UpdatePlayerInput()
{
    if (!IsTickUpdate(&inputTick))
        return;
    else
        UpdateTick(&inputTick);
    ExecuteInputActions(&actions);
    SetupPlayerInputComponent(&actions);
    SetupPlayerAnimation(&actions);
}

void UpdatePhysics()
{
    // TODO: https://www.raylib.com/examples/models/loader.html?name=models_box_collisions
}

void Draw_3D_Console()
{
    for (size_t i = 0; i < LEVEL_SIZE; i++)
    {
        Draw_GameObject(gos[i]);
        Draw_GameObject_Console(gos[i]);
    }
    DrawGrid(50, 1.0f);
    Draw_GameObject_PlayerHitBox(gos[0]);
    // TODO: move to Load_LevelTree()
    // light spot
    DrawCubeWiresV(LIGHT_TRANSFORM, (Vector3){1.0f, 1.0f, 1.0f}, YELLOW);
}

void Draw_2D()
{
    if (showConsole == 1)
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
    if (fps_counter_show == true)
    {
        DrawFPS(GetScreenWidth() - 100, 15);
    }
}

void Draw_3D_Models()
{
    BeginMode3D(camera);
    {
        if (showConsole == 1)
        {
            for (size_t i = 0; i < LEVEL_SIZE; i++)
                Draw_GameObject(gos[i]);
            Draw_3D_Console();
        }
        else
            for (size_t i = 0; i < LEVEL_SIZE; i++)
                Draw_GameObject(gos[i]);
    }
    EndMode3D();
}

void Draw_Pipeline_Default()
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        // Stage 1/2 Geometry
        Draw_3D_Skybox(skybox, camera);
        Draw_3D_Models();
        // Stage 2/2 2D
        Draw_2D();
    }
    EndDrawing();
}

void Draw_Pipeline_PostProcessing(RenderTexture2D *target)
{
    BeginTextureMode(*target);
    {
        ClearBackground(RAYWHITE);
        // Stage 1/3 Geometry
        Draw_3D_Skybox(skybox, camera);
        Draw_3D_Models();
    }
    EndTextureMode();
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        // Stage 2/3 PostProcessing
        Draw_PostProcessing(postproShader, target);
        // Stage 3/3 2D
        Draw_2D();
    }
    EndDrawing();
}

void Init_PostProcess(RenderTexture2D *target, bool postprocessing_enable)
{
    const char *POSTPROC_DEFAULT = "resources/shaders/glsl%i/default.fs";
    const char *POSTPROC_BLOOM = "resources/shaders/glsl%i/bloom.fs";
    char *shaderPath = (postprocessing_enable == true) ? POSTPROC_BLOOM : POSTPROC_DEFAULT;
    char tmp[PATH_MAX];
    // TODO: move to Load_LevelTree()
    // SOURCE: https://www.raylib.com/examples/shaders/loader.html?name=shaders_postprocessing
    postpro = postprocessing_enable;
    postproShader = LoadShader(0, GetAssetPath(tmp, shaderPath));
    postproTarget = target;
}

// TODO: move to Load_LevelTree()
void Init_Animation()
{
    char tmp[PATH_MAX];
    int tmpAnimCount = 0;
    playerAnimations[0] = LoadModelAnimations(GetAssetPath(tmp, "resources/animations/Idle.m3d"), &tmpAnimCount)[0];
    playerAnimations[1] = LoadModelAnimations(GetAssetPath(tmp, "resources/animations/Running.m3d"), &tmpAnimCount)[0];
    animCurrentFrame = 0;
}

void Init_Lighting()
{
    light_shader = LoadLighting();
    // TODO: move to Load_LevelTree()
    light_point = CreateLight(LIGHT_POINT, LIGHT_TRANSFORM, (Vector3){0.0f, 0.0f, 0.0f}, LIGHT_COLOR, light_shader);
    for (int i = 0; i < LEVEL_SIZE; i++)
        SetModelLighting(gos[i].model, light_shader);
}
