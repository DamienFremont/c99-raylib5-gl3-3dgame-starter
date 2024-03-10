#include "config.h"
#include <raylib.h>

#pragma once

#define MAX_POSTPRO_SHADERS 2

typedef enum
{
	FX_DEFAULT = 0,
	FX_BLOOM,
} PostproShader;

typedef struct ShaderResources ShaderResources;
struct ShaderResources
{
	Shader shaders[MAX_POSTPRO_SHADERS];
};

ShaderResources LoadShaders(AppConfiguration appProps);

void UnloadShaders(ShaderResources res);
