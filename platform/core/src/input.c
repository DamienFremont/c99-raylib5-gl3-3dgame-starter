#include <raylib.h>
#include <stdbool.h>

#include "level.h"

typedef struct InputConfig InputConfig;
struct InputConfig
{
    Vector3 playerPosition;
    float char_speed;
};

typedef struct InputOut InputOut;
struct InputOut
{
    Vector3 playerPosition;
    int showConsole;
    int animIndex;
};

InputOut CheckInputEvent(InputConfig cfg)
{
    InputOut out;
    out.playerPosition.x = cfg.playerPosition.x;
    out.playerPosition.y = cfg.playerPosition.y;
    out.playerPosition.z = cfg.playerPosition.z;
    out.animIndex = ANIM_IDLE;
    out.showConsole = 0;

    // Common
    if (IsKeyDown(KEY_F1))
    {
        out.showConsole = 1;
    }

    // Level
    if (IsKeyDown(KEY_D))
    {
        out.playerPosition.x = cfg.playerPosition.x - cfg.char_speed;
        out.animIndex = ANIM_RUN;
    }
    else if (IsKeyDown(KEY_A))
    {
        out.playerPosition.x = cfg.playerPosition.x + cfg.char_speed;
        out.animIndex = ANIM_RUN;
    }
    else if (IsKeyDown(KEY_W))
    {
        out.playerPosition.z = cfg.playerPosition.z + cfg.char_speed;
        out.animIndex = ANIM_RUN;
    }
    else if (IsKeyDown(KEY_S))
    {
        out.playerPosition.z = cfg.playerPosition.z - cfg.char_speed;
        out.animIndex = ANIM_RUN;
    }

    return out;
}