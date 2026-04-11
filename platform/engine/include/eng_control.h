#pragma once
#include "eng_input.h"

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef struct Controller
{
    Vector3 position;
    Vector3 direction;
} Controller;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

void ControlTankMove(Controller *player, InputActionValue value, float char_speed, float char_rot);
float ControlTankGetRotationAngle(Vector3 playerDirection);
