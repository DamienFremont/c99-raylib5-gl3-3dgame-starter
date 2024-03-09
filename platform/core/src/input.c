#include <raylib.h>
#include <stdbool.h>

#include "level.h"

typedef struct InputConfig InputConfig;
struct InputConfig
{
    Vector3 playerPosition;
    int showConsole;
    float char_speed;
};

typedef struct InputOut InputOut;
struct InputOut
{
    Vector3 playerPosition;
    int showConsole;
    int animIndex;
};

typedef struct InputEventState InputEventState;
struct InputEventState
{
    int KEY_F1_press;
};

InputEventState state = {0};

InputOut InitInputEvent()
{
    state.KEY_F1_press = 0;
}

InputOut ExecuteInputEvent(InputConfig cfg)
{
    InputOut out;
    out.showConsole = cfg.showConsole;
    out.animIndex = ANIM_IDLE;
    out.playerPosition = (Vector3){
        cfg.playerPosition.x,
        cfg.playerPosition.y,
        cfg.playerPosition.z};

    // Common
    if (IsKeyDown(KEY_F1) && state.KEY_F1_press == 0)
    {
        out.showConsole = !out.showConsole;
        state.KEY_F1_press = 1;
    }
    else if (IsKeyUp(KEY_F1))
    {
        state.KEY_F1_press = 0;
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