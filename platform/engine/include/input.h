#include <raylib.h>

#pragma once

typedef struct InputActionState
{
    bool Started;
    bool Completed;
    bool Triggered;
} InputActionState;

typedef struct InputActionValue
{
    bool Button;
    float Axis1D;
    Vector2 Axis2D;
} InputActionValue;

typedef struct InputAction
{
    InputActionState State;
    InputActionValue Value;
} InputAction;

typedef struct InputActions
{
    InputAction ConsoleAction;
    InputAction MoveAction;
} InputActions;

void InitInputActions(InputActions *actions);

void ExecuteInputActions(InputActions *actions);
