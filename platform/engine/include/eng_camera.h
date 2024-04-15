#include <raylib.h>
#include "eng_control.h"

#pragma once

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Camera InitCamera();
void CameraFixed_Look(Camera *camera, Controller player, Vector3 cameraTransform);
void CameraFollow_Look(Camera *camera, Controller player, Vector3 cameraTransform);
