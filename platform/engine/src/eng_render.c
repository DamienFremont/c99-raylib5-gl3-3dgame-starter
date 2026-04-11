#include "eng_render.h"

#include <raylib.h>

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

void DrawPostProcessing(Shader postproShader, const RenderTexture2D *target)
{
    BeginShaderMode(postproShader);
    {
        DrawTextureRec(                               //
            target->texture,                          //
            (Rectangle){                              //
                        0,                            //
                        0,                            //
                        (float)target->texture.width, //
                        (float)-target->texture.height},
            (Vector2){0, 0}, //
            WHITE);
    }
    EndShaderMode();
}
