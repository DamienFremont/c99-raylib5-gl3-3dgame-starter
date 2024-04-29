#include "eng_input.h"
#include "eng_gameobject.h"

#pragma once

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

void ControlTank_Move(Controller *player, InputActionValue value, float char_speed, float char_rot);
float ControlTank_ModelRotationAngle(Vector3 playerDirection);
