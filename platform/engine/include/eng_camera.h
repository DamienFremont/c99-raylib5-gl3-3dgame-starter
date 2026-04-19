#pragma once
#include <raylib.h>
#include "eng_control.h"

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

void InitCamera(Camera3D *camera, float fov, Vector3 distance);
void CameraSecondPerson_Look(Camera3D *camera, Controller player);
void CameraThirdPerson_Look(Camera3D *camera, Controller player);
void CameraFollow_Look(Camera3D *camera, Controller player);
