#pragma once
#include <raylib.h>

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Model LoadSkyboxFromImage(Image img);
void DrawSkybox(Model skybox, Camera3D camera);
void UnloadSkybox(Model skybox);
