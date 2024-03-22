#include "assets.h"

#include <raylib.h>

static char RESOURCES_PATH[PATH_MAX];

void InitAssets(char *path)
{
    strcpy(RESOURCES_PATH, path);
}

char *GetAssetPath(char *tmp, char *assetpath)
{
    JoinPath(tmp, RESOURCES_PATH, assetpath);
    return tmp;
}

char *JoinPath(char *dest, char *path1, char *path2)
{
    // TODO: end with or start with '/'
    const char tmp[PATH_MAX];
    snprintf(tmp, sizeof(tmp), "%s/%s", path1, path2);
    strcpy(dest, tmp);
    return dest;
}