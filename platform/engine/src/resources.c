#include "resources.h"

void pathJoin(char *dest, char *path1, char *path2)
{
    // TODO: end with or start with '/'
    const char tmp[MAX_PATH];
    snprintf(tmp, sizeof(tmp), "%s/%s", path1, path2);
    strcpy(dest, tmp);
}