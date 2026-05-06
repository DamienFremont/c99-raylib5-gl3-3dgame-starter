#include "eng_material.h"

#include "eng_config.h"
#include "eng_assets.h"

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

Texture2D CheckboardTexture2D(Color col1, Color col2, float x, float y, float z) 
{
    int checkByMeter = 1;
    int textureResolutionByMeter = 64;
    int checksX= y * 32;
    int checksY= x * 1;

    int width = x * checkByMeter * textureResolutionByMeter;
    int height = y * checkByMeter * textureResolutionByMeter;

    Texture2D texture = LoadTextureFromImage(GenImageChecked(
        width, height,
        checksX, checksY,
        col1, col2));

    return texture;
}

