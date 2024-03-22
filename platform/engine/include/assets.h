#include <raylib.h>

typedef enum
{
    PATH_MAX = 260,
} ResourceUtils;

void InitAssets(char *path);

char *GetAssetPath(char *tmp, char *assetpath);

char *JoinPath(char *dest, char *path_1, char *path_2);
