#include <raylib.h>

#pragma once

#include "control.h"

Camera InitCamera();

void TankControl_Look(Camera *camera, Controller player, int cameraDistance, int cameraHeight);
