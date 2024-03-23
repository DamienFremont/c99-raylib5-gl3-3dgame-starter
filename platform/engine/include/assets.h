#include <raylib.h>

typedef enum
{
    PATH_MAX = 260,
} ResourceUtils;

void InitAssets(char *path, int glsl_ver);

char *GetAssetPath(char *tmp, char *assetpath);

char *GetShaderPath(char *tmp, char *assetpath);

char *JoinPath(char *dest, char *path_1, char *path_2);
