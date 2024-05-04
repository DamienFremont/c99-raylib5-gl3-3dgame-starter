#include "eng_input.h"

#include <stdbool.h>

// source: https://www.raylib.com/examples/core/loader.html?name=core_input_gamepad

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

int gamepad = 0; // which gamepad to display
float gamepad_left_joy_deadzone = 0.25f;

typedef enum
{
    KEY = 0,
    BTN,
    AXE,
} InputDefinitionType;

typedef enum
{
    MINUS = -1,
    PLUS = 1
} InputDefinitionDirection;

typedef struct
{
    InputDefinitionType type;
    int name;
    InputDefinitionDirection axeDirection; // optionnal
} InputDefinition;

#define INPUTS_MAX 4

const InputDefinition INPUT_DEF_UP[INPUTS_MAX] = {{KEY, KEY_W}, {KEY, KEY_UP}, {BTN, GAMEPAD_BUTTON_LEFT_FACE_UP}, {AXE, GAMEPAD_AXIS_LEFT_Y, MINUS}};
const InputDefinition INPUT_DEF_DOWN[INPUTS_MAX] = {{KEY, KEY_S}, {KEY, KEY_DOWN}, {BTN, GAMEPAD_BUTTON_LEFT_FACE_DOWN}, {AXE, GAMEPAD_AXIS_LEFT_Y, PLUS}};
const InputDefinition INPUT_DEF_LEFT[INPUTS_MAX] = {{KEY, KEY_A}, {KEY, KEY_LEFT}, {BTN, GAMEPAD_BUTTON_LEFT_FACE_LEFT}, {AXE, GAMEPAD_AXIS_LEFT_X, MINUS}};
const InputDefinition INPUT_DEF_RIGHT[INPUTS_MAX] = {{KEY, KEY_D}, {KEY, KEY_RIGHT}, {BTN, GAMEPAD_BUTTON_LEFT_FACE_RIGHT}, {AXE, GAMEPAD_AXIS_LEFT_X, PLUS}};

//---------------------------------------------------------
// Local Functions Declaration
//---------------------------------------------------------

bool MoveAction(InputActions *actions);
bool ConsoleAction(InputActions *out);
float GetFirstInputValue(InputDefinition* inputs);
float GetInputValue(InputDefinition inputs);
bool IsInInterval(float value, float min, float max);

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

void InitInputActions(InputActions *actions)
{
    actions->ConsoleAction = (InputAction){(InputActionState){false, false, false}, 0};
    actions->MoveAction = (InputAction){0, 0};
}

void ExecuteInputActions(InputActions *actions)
{
    ConsoleAction(actions);
    MoveAction(actions);
    // TODO: JumpAction(&out);
    // TODO: LookAction(&out);
}

//---------------------------------------------------------
// Local Functions Definition
//---------------------------------------------------------

bool ConsoleAction(InputActions *out)
{
    if (IsKeyDown(KEY_F1) && !out->ConsoleAction.State.Started)
    {
        out->ConsoleAction.State.Started = true;
        out->ConsoleAction.State.Completed = false;
    }
    if (IsKeyUp(KEY_F1) && out->ConsoleAction.State.Started)
    {
        out->ConsoleAction.State.Started = false;
        out->ConsoleAction.State.Completed = true;
    }
}

bool MoveAction(InputActions *actions)
{
    const InputActionValue axis2D = (InputActionValue){0, 0, {0, 0}};
    actions->MoveAction.Value = axis2D;
    actions->MoveAction.State.Triggered = false;
    // input
    bool up = GetFirstInputValue(INPUT_DEF_UP);
    bool down = GetFirstInputValue(INPUT_DEF_DOWN);
    bool left = GetFirstInputValue(INPUT_DEF_LEFT);
    bool right = GetFirstInputValue(INPUT_DEF_RIGHT);
    // action
    if (left || right || up || down)
        actions->MoveAction.State.Triggered = true;
    // X
    if (left)
        actions->MoveAction.Value.Axis2D.x = -left;
    else if (right)
        actions->MoveAction.Value.Axis2D.x = right;
    // Y
    if (up)
        actions->MoveAction.Value.Axis2D.y = up;
    else if (down)
        actions->MoveAction.Value.Axis2D.y = -down;
    // find out which way is forward
    // const FRotator Rotation = Controller->GetControlRotation();
    // const FRotator YawRotation(0, Rotation.Yaw, 0);
    // float ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    // float RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
}

float GetFirstInputValue(InputDefinition* inputs)
{
    for (int i = 0; i < INPUTS_MAX; i++)
    {
        float val = GetInputValue(inputs[i]);
        if (val != 0) {
            return val;
        }
    }
    return 0;
}

float GetInputValue(InputDefinition input)
{

    if (input.type == KEY && IsKeyDown(input.name))
    {
        return 1;
    }
    else if (input.type == BTN && IsGamepadButtonDown(gamepad, input.name))
    {
        return 1;
    }
    else if (input.type == AXE && (GetGamepadAxisMovement(gamepad, input.name) != 0))
    {
        float axis = GetGamepadAxisMovement(gamepad, input.name);
        if (IsInInterval(axis, -gamepad_left_joy_deadzone, gamepad_left_joy_deadzone))
        {
            return 0;
        }
        if (input.axeDirection == PLUS && axis > 0)
        {
            return axis;
        }
        else if (input.axeDirection == MINUS && axis < 0)
        {
            return -axis;
        }
    }
    return 0;
}

// TODO: move to eng_math.h
bool IsInInterval(float value, float min, float max)
{
    return (value <= max && value >= min);
}
