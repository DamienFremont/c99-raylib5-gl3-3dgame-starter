#include "config.h"

#pragma once

#include <raylib.h>
#include <time.h>
#include "input.h"
#include "gameobject.h"

typedef struct UnrealThirdPerson_State
{
    AppConfiguration appConfig;
    RenderTexture2D *target;

    int showConsole;
    Shader postproShader;
    Shader tilingShader;

    Model cubeModel;
    Model rampModel;
    Model chamferCubeModel;

    Shader *shaders;
    int animIndex;
    unsigned int animCurrentFrame;
} UnrealThirdPerson_State;

UnrealThirdPerson_State Init_UnrealThirdPerson(AppConfiguration appConfig, RenderTexture2D *target);

int Update_UnrealThirdPerson(UnrealThirdPerson_State *state);

void Draw_UnrealThirdPerson(UnrealThirdPerson_State *state, RenderTexture2D *target);

void Unload_UnrealThirdPerson(UnrealThirdPerson_State *state);
