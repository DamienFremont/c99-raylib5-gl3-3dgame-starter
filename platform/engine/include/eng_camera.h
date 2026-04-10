#pragma once
#include <raylib.h>
#include "eng_control.h"

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Camera InitCamera(float fov, Vector3 distance);
void CameraSecondPerson_Look(Camera *camera, Controller player);
void CameraThirdPerson_Look(Camera *camera, Controller player);
void CameraFollow_Look(Camera *camera, Controller player);
