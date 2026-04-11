#pragma once
#include <raylib.h>

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    PATH_MAX = 1024,
} ResourceUtils;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

Shader LoadShaderFile(const char *vsFileName, const char *fsFileName);
Image LoadImageFile(const char *fileName);
Model LoadModelFile();
Texture2D LoadTextureFile(const char *fileName);
ModelAnimation *LoadAnimationFile(const char *fileName);

void InitAssets(const char *path, int glsl_ver);
char *GetAssetPath(char *tmp, const char *assetPath);
char *GetShaderPath(char *tmp, const char *assetPath);
char *JoinPath(char *dest, const char *path_1, const char *path_2);


