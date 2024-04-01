#include "input.h"

#include <raylib.h>
#include <stdbool.h>

InputEvent_State InitInputEvent()
{
    InputEvent_State state = {0};
    state.KEY_F1_press = 0;
    return state;
}

InputOut Console(InputOut out, InputEvent_State state)
{
    if (IsKeyDown(KEY_F1) && state.KEY_F1_press == 0)
    {
        out.showConsole = !out.showConsole;
        state.KEY_F1_press = 1;
    }
    else if (IsKeyUp(KEY_F1))
    {
        state.KEY_F1_press = 0;
    }
        return out;

}

InputOut TankControl(InputOut out, InputConfig cfg)
{
    const int ANIM_RUN = 1;
    const int ANIM_IDLE = 0;

    out.animIndex = ANIM_IDLE;
    out.playerPosition = (Vector3){
        cfg.playerPosition.x,
        cfg.playerPosition.y,
        cfg.playerPosition.z};


    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        out.playerPosition.z = cfg.playerPosition.z + cfg.char_speed;
        out.animIndex = ANIM_RUN;
    }
    else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        out.playerPosition.z = cfg.playerPosition.z - cfg.char_speed;
        out.animIndex = ANIM_RUN;
    }

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        out.playerPosition.x = cfg.playerPosition.x + cfg.char_speed;
        out.animIndex = ANIM_RUN;
    }
    else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        out.playerPosition.x = cfg.playerPosition.x - cfg.char_speed;
        out.animIndex = ANIM_RUN;
    }

    return out;
}

InputOut ExecuteInputEvent(InputEvent_State state, InputConfig cfg)
{
    InputOut out;
    out.showConsole = cfg.showConsole;
    out = Console(out, state);
    out = TankControl(out, cfg);
    return out;
}
