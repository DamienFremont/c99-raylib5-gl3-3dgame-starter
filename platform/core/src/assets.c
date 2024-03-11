#include "assets.h"

#include <raylib.h>
#include "resources.h"

Image LoadImageResource(char* resourcesPath, char* assetPath) {
    const char fileName[MAX_PATH];
    pathJoin(fileName, resourcesPath, assetPath);
    Image image = LoadImage(fileName);
    return image;
}

Model LoadModelResource(char *resourcesPath, char *assetPath)
{
    const char fileName[MAX_PATH];
    pathJoin(fileName, resourcesPath, assetPath);
    Model model = LoadModel(fileName);
    return model;
}

Texture2D LoadTextureResource(char* resourcesPath, char* assetPath)
{
    const char fileName[MAX_PATH];
    pathJoin(fileName, resourcesPath, assetPath);
    Texture2D texture = LoadTexture(fileName);
    return texture;
}

ModelAnimation* LoadAnimationsResource(char* resourcesPath, char* assetPath)
{
    int animsCount = 0;
    const char fileName[MAX_PATH];
    pathJoin(fileName, resourcesPath, assetPath);
    ModelAnimation* modelAnimations = LoadModelAnimations(fileName, &animsCount);
    return modelAnimations;
}

Shader LoadShaderResource(char* resourcesPath, char* assetPath)
{
    const char fileName[MAX_PATH];
    pathJoin(fileName, resourcesPath, assetPath);
    Shader shader = LoadShader(0, fileName);
    return shader;
}

Shader LoadShaderResource2(char* resourcesPath, char* vsAssetPath, char* fsAssetPath)
{
    const char vsFileName[MAX_PATH];
    pathJoin(vsFileName, resourcesPath, vsAssetPath);

    const char fsFileName[MAX_PATH];
    pathJoin(fsFileName, resourcesPath, fsAssetPath);

    Shader shader = LoadShader(vsFileName, fsFileName);
    return shader;
}

//void UnloadShadersAll(Shader *shaders)
//{
//    for (int i = 0; i < sizeof(shaders); i++)
//        UnloadShader(shaders[i]);
//}