#include <raylib.h>

#pragma once

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Model LoadSkyboxFromImage(Image img);
void Draw_3D_Skybox(Model skybox, Camera camera);
Model UnloadSkybox(Model skybox);
