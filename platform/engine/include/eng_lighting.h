#pragma once
#include <raylib.h>

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Shader LoadLighting(void);
void UpdateLighting(Shader shader, Camera3D camera);
void UnloadLighting(Shader shader);
Model SetModelLighting(Model model, const Shader shader);
