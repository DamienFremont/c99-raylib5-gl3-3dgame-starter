typedef enum
{
    MAX_PATH = 260,
} ResourceUtils;

void InitResources(char *resource_path);

char * GetResourcePath(char *tmp, char* path1);

char *JoinPath(char *path1, char *path2);

void pathJoin(char *dest, char *path1, char *path2);