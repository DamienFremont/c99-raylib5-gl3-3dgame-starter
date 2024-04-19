#include <raylib.h>

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    PATH_MAX = 260,
} ResourceUtils;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Shader LoadShader_GetShaderPath(const char *vsFileName, const char *fsFileName);
Image LoadImage_GetAssetPath(const char *fileName);
Model LoadModel_GetAssetPath();
Texture2D LoadTexture_GetAssetPath(const char *fileName);
ModelAnimation *LoadModelAnimations_GetAssetPath(const char *fileName);

void InitAssets(char *path, int glsl_ver);
char *GetAssetPath(char *tmp, char *assetpath);
char *GetShaderPath(char *tmp, char *assetpath);
char *JoinPath(char *dest, char *path_1, char *path_2);


