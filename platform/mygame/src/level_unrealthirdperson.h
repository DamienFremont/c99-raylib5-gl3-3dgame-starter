#include "config.h"

#include <raylib.h>
#include <time.h>
#include "input.h"
#include "component.h"

typedef enum
{
    LEVEL_SIZE = 12,
} UnrealThirdPersonLevelSize;

typedef struct UnrealThirdPerson_State UnrealThirdPerson_State;
struct UnrealThirdPerson_State
{
    AppConfiguration appConfig;
    RenderTexture2D *target;
    char consoleOut;

    int showConsole;
    Shader postproShader;
    Shader tilingShader;
    Camera camera;

    Vector3 playerPosition;

    StaticMeshComponent components[LEVEL_SIZE];
    Model cubeModel;
    Model rampModel;
    Model chamferCubeModel;

    Model skybox;

    Shader *shaders;
    InputEvent_State input_State;
    int animIndex;
    ModelAnimation *modelAnimations;
    unsigned int animCurrentFrame;
};

UnrealThirdPerson_State Init_UnrealThirdPerson(AppConfiguration appConfig, RenderTexture2D *target, char *consoleOut);

int Update_UnrealThirdPerson(UnrealThirdPerson_State *state);

void Texture_UnrealThirdPerson(UnrealThirdPerson_State *state);

void Draw_UnrealThirdPerson(UnrealThirdPerson_State *state, RenderTexture2D target);

void Unload_UnrealThirdPerson(UnrealThirdPerson_State *state);
