#pragma once

#include "config.h"
#include <raylib.h>

#define MAX_MODELS 1

typedef enum
{
    ANIM_RUN = 6,
    ANIM_IDLE = 4,
} AcharAnim;

typedef enum
{
    CHARACTER = 0,
} Models;

typedef struct LvlResources LvlResources;
struct LvlResources
{
    Model models[MAX_MODELS];
    Texture2D textures[MAX_MODELS];
    ModelAnimation *modelAnimations;
};

LvlResources LoadLevel(AppConfiguration appProps);

void UnloadLevel();

void DrawLevel();

Vector3 PlayerPosition();
