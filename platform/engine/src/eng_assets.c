#include "eng_assets.h"

#include <raylib.h>
#include <stdio.h>
#include <string.h>

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

static char RESOURCES_PATH[PATH_MAX];
static int GLSL_VERSION = 330;

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

void InitAssets(const char *path, int glsl_ver)
{
    strcpy_s(RESOURCES_PATH, sizeof(RESOURCES_PATH), path);
    GLSL_VERSION = glsl_ver;
}

char *GetAssetPath(char *tmp, const char *assetPath)
{
    JoinPath(tmp, RESOURCES_PATH, assetPath);
    return tmp;
}

// TODO
char *GetShaderPath(char *tmp, const char *assetPath)
{
    JoinPath(tmp, RESOURCES_PATH, TextFormat(assetPath, GLSL_VERSION));
    return tmp;
}

char *JoinPath(char *dest, const char *path1, const char *path2)
{
    // TODO: end with or start with '/'
    const char tmp[PATH_MAX];
    snprintf(tmp, sizeof(tmp), "%s/%s", path1, path2);
    strcpy_s(dest, PATH_MAX, tmp);
    return dest;
}

Shader LoadShader_GetShaderPath(const char *vsFileName, const char *fsFileName)
{
    char tmp[PATH_MAX];
    char tmp2[PATH_MAX];
    return LoadShader(
        (vsFileName == 0) ? 0 : GetShaderPath(tmp, vsFileName),
        GetShaderPath(tmp2, fsFileName));
}

Image LoadImage_GetAssetPath(const char *fileName)
{
    char tmp[PATH_MAX];
    return LoadImage(
        GetAssetPath(tmp, fileName));
}

Model LoadModel_GetAssetPath(const char *fileName)
{
    char tmp[PATH_MAX];
    return LoadModel(
        GetAssetPath(tmp, fileName));
}

Texture2D LoadTexture_GetAssetPath(const char *fileName)
{
    char tmp[PATH_MAX];
    return LoadTexture(
        GetAssetPath(tmp, fileName));
}

ModelAnimation *LoadModelAnimations_GetAssetPath(const char *fileName)
{
    char tmp[PATH_MAX];
    int tmpAnimCount = 0;
    return LoadModelAnimations(
        GetAssetPath(tmp, fileName),
        &tmpAnimCount);
}
