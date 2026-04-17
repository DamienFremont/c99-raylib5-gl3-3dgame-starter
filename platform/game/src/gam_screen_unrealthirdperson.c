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
#include "eng_lighting.h"
#include "eng_skybox.h"
#include "eng_render.h"

//---------------------------------------------------------
// Local Variables Definition (local to this module)
//---------------------------------------------------------

// TODO: move to Load_LevelTree()
const float PLAYER_SPEED = 0.12f;
const float CAM_FOV = 60;
const Vector3 CAM_TRS = {0, 1, 4};
const Vector3 CAM_POS = {5, 1, 11};
const Vector3 LIGHT_TRANSFORM = {0, 9, 39};
const Color LIGHT_COLOR = {255, 255, 230, 255}; // YELLOW
Camera3D camera;
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
TickState worldTick = {0};
InputActions actionsGLOBAL; // move to global "state" struct 
Controller playerController;
ModelAnimation playerAnimations[2];

bool showConsole;
bool fps_counter_show;

int animIndex;
int animCurrentFrame;

//---------------------------------------------------------
// Local Functions Declaration
//---------------------------------------------------------

void Draw_Pipeline_Default(void);
void Draw_Pipeline_PostProcessing(const RenderTexture2D *target);
void UpdatePlayerAnimation(void);
void UpdatePlayerCamera(void);
void UpdatePlayerPosition(InputActions *actions);
void UpdatePlayerInput(void);
void UpdateRender(void);
void Init_PostProcess(RenderTexture2D *target, bool postprocessing_enable);
void Init_Animation(void);
void Init_Lighting(void);

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
    worldTick = InitTick(60);
    StartTick(&animationTick);
    StartTick(&inputTick);
    StartTick(&renderTick);
    StartTick(&worldTick);
    // PLAYER
    // TODO: move to Load_LevelTree()
    playerController = (Controller){
        gos[0].transform.translation, // player position
        (Vector3){1, 0, 0},           // screen forward
    };
    camera.position = CAM_POS;
    InitInputActions(&actionsGLOBAL);
}

int Update_UnrealThirdPerson(void)
{
    // tick
    UpdatePlayerInput();
    UpdatePlayerPosition(&actionsGLOBAL);
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

void Draw_UnrealThirdPerson(const RenderTexture2D *target)
{
    if (postpro == true)
    {
        Draw_Pipeline_PostProcessing(target);
        return;
    }
    Draw_Pipeline_Default();
}

void Unload_UnrealThirdPerson(void)
{
    // skybox
    UnloadSkybox(skybox);
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

void UpdateRender(void)
{
    UpdateLighting(light_shader, camera);
}

void UpdatePlayerAnimation(void)
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

void UpdatePlayerCamera(void)
{
    // CameraThirdPerson_Look(&camera, playerController);
    // CameraSecondPerson_Look(&camera, playerController);
    CameraFollow_Look(&camera, playerController);
}

void UpdatePlayerPosition(InputActions *actions)
{
    if (!IsTickUpdate(&worldTick))
        return;
    else
        UpdateTick(&worldTick);
    // Moving
    if (actions->MoveAction.State.Triggered == true)
        ControlTankMove(&playerController, actions->MoveAction.Value, PLAYER_SPEED, PLAYER_SPEED * 15);
    // TODO: Jumping
    // TODO: Looking
    // player
    gos[LEVEL_PLAYER_MODEL].transform.translation = (Vector3){
        playerController.position.x,
        playerController.position.y,
        playerController.position.z};
    gos[LEVEL_PLAYER_MODEL].transform.rotation = (Rotation2){
        AXIS_YAW,
        ControlTankGetRotationAngle(playerController.direction)};
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
}

void SetupPlayerAnimation(const InputActions *actions)
{
    animIndex = 0;
    if (actions->MoveAction.State.Triggered == true)
        animIndex = 1;
}

void UpdatePlayerInput(void)
{
    if (!IsTickUpdate(&inputTick))
        return;
    else
        UpdateTick(&inputTick);
    ExecuteInputActions(&actionsGLOBAL);
    SetupPlayerInputComponent(&actionsGLOBAL);
    SetupPlayerAnimation(&actionsGLOBAL);
}

void UpdatePhysics(void)
{
    // TODO: https://www.raylib.com/examples/models/loader.html?name=models_box_collisions
}

void Draw_3D_Console(void)
{
    for (size_t i = 0; i < LEVEL_SIZE; i++)
    {
        DrawGameObject(gos[i]);
        DrawConsoleGameObject(gos[i]);
    }
    DrawGrid(50, 1.0f);
    DrawConsolePlayerHitBox(gos[0]);
    // TODO: move to Load_LevelTree()
    // light spot
    DrawCubeWiresV(LIGHT_TRANSFORM, (Vector3){1.0f, 1.0f, 1.0f}, YELLOW);
}

void Draw_2D(void)
{
    if (showConsole == 1)
    {
        // LogConsole(TextFormat("animationTick.current/lastUpdate: %i %i", animationTick.current, animationTick.lastUpdate));
        LogConsole(TextFormat("playerController.direction.x,y,z: %f %f %f", playerController.direction.x, playerController.direction.y, playerController.direction.z));
        DrawConsole();
    }
    else
    {
        const int LINE_HEIGHT_30 = 30;
        DrawText("Press [TAB] to toggle menu", 10, 10 + LINE_HEIGHT_30 * 0, FONT_SIZE_24, GRAY);
        DrawText("Use keys [W][A][S][D] or Arrows or GamePad to move character", 10, 10 + LINE_HEIGHT_30 * 1, FONT_SIZE_24, GRAY);
        DrawText("Press [ALT+ENTER] to toggle screen", 10, 10 + LINE_HEIGHT_30 * 3, FONT_SIZE_24, GRAY);
        DrawText("Press [F1] to toggle console", 10, 10 + LINE_HEIGHT_30 * 2, FONT_SIZE_24, GRAY);
    }
    if (fps_counter_show == true)
    {
        DrawFPS(GetScreenWidth() - 100, 15);
    }
}

void Draw_3D_Models(void)
{
    BeginMode3D(camera);
    {
        if (showConsole == 1)
        {
            for (size_t i = 0; i < LEVEL_SIZE; i++)
                DrawGameObject(gos[i]);
            Draw_3D_Console();
        }
        else
            for (size_t i = 0; i < LEVEL_SIZE; i++)
                DrawGameObject(gos[i]);
    }
    EndMode3D();
}

void Draw_Pipeline_Default(void)
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        // Stage 1/2 Geometry
        DrawSkybox(skybox, camera);
        Draw_3D_Models();
        // Stage 2/2 2D
        Draw_2D();
    }
    EndDrawing();
}

void Draw_Pipeline_PostProcessing(const RenderTexture2D *target)
{
    BeginTextureMode(*target);
    {
        ClearBackground(RAYWHITE);
        // Stage 1/3 Geometry
        DrawSkybox(skybox, camera);
        Draw_3D_Models();
    }
    EndTextureMode();
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        // Stage 2/3 PostProcessing
        DrawPostProcessing(postproShader, target);
        // Stage 3/3 2D
        Draw_2D();
    }
    EndDrawing();
}

void Init_PostProcess(RenderTexture2D *target, bool postprocessing_enable)
{
    const char *POSTPROC_DEFAULT = "resources/shaders/glsl%i/default.fs";
    const char *POSTPROC_BLOOM = "resources/shaders/glsl%i/bloom.fs";
    const char *shaderPath = (postprocessing_enable == true) ? POSTPROC_BLOOM : POSTPROC_DEFAULT;
    // TODO: move to Load_LevelTree()
    // SOURCE: https://www.raylib.com/examples/shaders/loader.html?name=shaders_postprocessing
    postpro = postprocessing_enable;
    postproShader = LoadShaderFile(0, shaderPath);
    postproTarget = target;
}

// TODO: move to Load_LevelTree()
void Init_Animation(void)
{
    playerAnimations[0] = LoadAnimationFile("resources/animations/Idle.m3d")[0];
    playerAnimations[1] = LoadAnimationFile("resources/animations/Running.m3d")[0];
    animCurrentFrame = 0;
}

void Init_Lighting(void)
{
    // TODO: move to Load_LevelTree()
    light_shader = LoadLighting();
    light_point = CreateLight(LIGHT_POINT, LIGHT_TRANSFORM, (Vector3){0.0f, 0.0f, 0.0f}, LIGHT_COLOR, light_shader);
    // world
    for (int i = 0; i < LEVEL_SIZE; i++)
        SetModelLighting(gos[i].model, light_shader);
    // player
    gos[0].model.materials[1].shader = light_shader;
    gos[0].model.materials[2].shader = light_shader;
}
