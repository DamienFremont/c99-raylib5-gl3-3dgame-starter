#include "config.h"

#pragma once

#include <raylib.h>

// void Init_Light(AppConfiguration appConfig);
Shader LoadLightShader(char *RESOURCES,int  GLSL_VERSION);

Shader LoadLightShader_lighting(char* RESOURCES, int GLSL_VERSION);
