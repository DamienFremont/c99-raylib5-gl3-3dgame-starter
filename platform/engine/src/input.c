#include "input.h"

#include <stdbool.h>

InputActionValue Helper_Axis2D(float x, float y)
{
    return (InputActionValue){
        0, // NULL
        0, // NULL
        (Vector2){x, y}};
}

bool MoveAction(InputAction *out)
{
    out->MoveAction_InputActionValue = Helper_Axis2D(0, 0);
    // input
    bool up = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
    bool down = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
    bool left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    bool right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
    // action
    if (left || right || up || down)
    {
        out->MoveAction = true;
    }
    // X
    if (left)
    {
        out->MoveAction_InputActionValue.Axis2D.x = -1;
    }
    else if (right)
    {
        out->MoveAction_InputActionValue.Axis2D.x = 1;
    }
    // Y
    if (up)
    {
        out->MoveAction_InputActionValue.Axis2D.y = 1;
    }
    else if (down)
    {
        out->MoveAction_InputActionValue.Axis2D.y = -1;
    }
}

InputAction ExecuteInputEvent()
{
    InputAction out = {0};
    MoveAction(&out);
    // TODO: JumpAction(&out);
    // TODO: LookAction(&out);
    return out;
}
