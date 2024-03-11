#include <raylib.h>

#include "config.h"

typedef struct Texture2DTiling Texture2DTiling;
struct Texture2DTiling
{
    float x;
    float y;
    // offset ?
};

Shader TileTexture2D(AppConfiguration appConfig, Texture2DTiling tiling);
