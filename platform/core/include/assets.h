#include <raylib.h>

Image LoadImageResource(char* resourcesPath, char* assetPath);

Model LoadModelResource(char *resourcesPath, char *assetPath);

Texture2D LoadTextureResource(char* resourcesPath, char* assetPath);

ModelAnimation* LoadAnimationsResource(char* resourcesPath, char* assetPath);

Shader LoadShaderResource(char* resourcesPath, char* assetPath);
Shader LoadShaderResource2(char* resourcesPath, char* vsAssetPath, char* fsAssetPath);
