#include <raylib.h>
#include "control.h"

#pragma once

Camera InitCamera();

void TankControl_Look(Camera *camera, Controller player, int cameraDistance, int cameraHeight);
