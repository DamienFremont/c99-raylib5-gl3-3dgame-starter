#include "resources.h"

static char RESOURCES_PATH[MAX_PATH];

void InitResources(char *path) {
    // RESOURCES_PATH = path;
    strcpy(RESOURCES_PATH, path);
}

char *GetResourcePath(char *tmp, char* assetpath) {
    pathJoin(tmp, RESOURCES_PATH, assetpath);
    return tmp;
}

char *JoinPath(char *path1, char *path2)
{
    // TODO: end with or start with '/'
    const char res[MAX_PATH];
    snprintf(res, sizeof(res), "%s/%s", path1, path2);
    return res;
}

void pathJoin(char *dest, char *path1, char *path2)
{
    // TODO: end with or start with '/'
    const char tmp[MAX_PATH];
    snprintf(tmp, sizeof(tmp), "%s/%s", path1, path2);
    strcpy(dest, tmp);
}