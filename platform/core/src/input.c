#include "input.h"

#include <raylib.h>
#include <stdbool.h>

InputEvent_State InitInputEvent()
{
    InputEvent_State state = {0};
    state.KEY_F1_press = 0;
    return state;
}

InputOut ExecuteInputEvent(InputEvent_State state, InputConfig cfg)
{
    const int ANIM_RUN = 6;
    const int ANIM_IDLE = 4;

    InputOut out;
    out.showConsole = cfg.showConsole;
    out.animIndex = ANIM_IDLE;
    out.playerPosition = (Vector3){
        cfg.playerPosition.x,
        cfg.playerPosition.y,
        cfg.playerPosition.z};

    // Common
    if (IsKeyPressed(KEY_F1) && state.KEY_F1_press == 0)
    {
        out.showConsole = !out.showConsole;
        state.KEY_F1_press = 1;
    }
    else if (IsKeyReleased(KEY_F1))
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