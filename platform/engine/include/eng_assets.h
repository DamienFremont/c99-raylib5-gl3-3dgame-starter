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

void InitAssets(const char *path, int glsl_ver);
char *GetAssetPath(char *tmp, const char *assetpath);
char *GetShaderPath(char *tmp, const char *assetpath);
char *JoinPath(char *dest, const char *path_1, const char *path_2);


