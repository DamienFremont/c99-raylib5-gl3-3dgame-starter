#include "input.h"

#pragma once

#include "control.h"
#include "gameobject.h"

typedef struct Controller
{
    Vector3 position;
    Vector3 direction;
} Controller;

void TankControl_Move(Controller *player, InputActionValue value, float char_speed, float char_rot);

void TankControl_Look(Camera *camera, Controller player, int cameraDistance, int cameraHeight);

float TankControl_ModelRotationAngle(Vector3 playerDirection);
