#include <raylib.h>

#pragma once

typedef struct InputConfig
{
    Vector3 playerPosition;
    int showConsole;
    float char_speed;
} InputConfig;

typedef struct InputOut
{
    Vector3 playerPosition;
    int showConsole;
    int animIndex;
} InputOut;

typedef struct InputEvent_State
{
    int KEY_F1_press;
} InputEvent_State;

InputEvent_State InitInputEvent();

InputOut ExecuteInputEvent(InputEvent_State state, InputConfig cfg);