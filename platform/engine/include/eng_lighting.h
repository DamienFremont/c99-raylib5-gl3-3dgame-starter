#include <raylib.h>

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Shader LoadLighting();
void UpdateLighting(Shader shader, Camera camera);
void UnloadLighting(Shader shader);
Model SetModelLighting(Model model, const Shader shader);
