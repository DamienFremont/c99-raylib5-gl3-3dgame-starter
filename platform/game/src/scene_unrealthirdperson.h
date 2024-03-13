#include "config.h"

#pragma once

#include <raylib.h>
#include <time.h>
#include "input.h"
#include "gameobject.h"

typedef enum
{
    LEVEL_SIZE = 13,
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


