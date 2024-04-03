#include "input.h"

#include <stdbool.h>

InputActionValue Build_ActionValue_2D(float x, float y)
{
    return (InputActionValue){
        0, // NULL
        0, // NULL
        (Vector2){x, y}};
}

bool MoveAction(InputActions *out)
{
    out->MoveAction.Value = Build_ActionValue_2D(0, 0);
    // input
    bool up = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
    bool down = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
    bool left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    bool right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
    // action
    if (left || right || up || down)
        out->MoveAction.State.Triggered = true;
    // X
    if (left)
        out->MoveAction.Value.Axis2D.x = -1;
    else if (right)
        out->MoveAction.Value.Axis2D.x = 1;
    // Y
    if (up)
        out->MoveAction.Value.Axis2D.y = 1;
    else if (down)
        out->MoveAction.Value.Axis2D.y = -1;
    // find out which way is forward
    // const FRotator Rotation = Controller->GetControlRotation();
    // const FRotator YawRotation(0, Rotation.Yaw, 0);
    // float ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    // float RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
}

bool ConsoleAction(InputActions *out)
{
}

InputActions ExecuteInputEvent()
{
    InputActions out = {0};
    ConsoleAction(&out);
    MoveAction(&out);
    // TODO: JumpAction(&out);
    // TODO: LookAction(&out);
    return out;
}
