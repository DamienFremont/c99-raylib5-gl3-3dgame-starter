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
    // X
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        out->MoveAction_InputActionValue = Helper_Axis2D(1, 0);
        out->MoveAction = true;
    }
    else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        out->MoveAction_InputActionValue = Helper_Axis2D(-1, 0);
        out->MoveAction = true;
    }
    // Y
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        out->MoveAction_InputActionValue = Helper_Axis2D(0, 1);
        out->MoveAction = true;
    }
    else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        out->MoveAction_InputActionValue = Helper_Axis2D(0, -1);
        out->MoveAction = true;
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
