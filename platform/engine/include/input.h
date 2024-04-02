#include <raylib.h>

#pragma once

typedef struct InputActionValue
{
    bool Button;
    float Axis1D;
    Vector2 Axis2D;
} InputActionValue;

typedef struct InputAction
{
    bool MoveAction;
    InputActionValue MoveAction_InputActionValue;
} InputAction;

InputAction ExecuteInputEvent();
