#include <stdbool.h>

#pragma once

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef struct AppConfiguration
{
    char appName[999];

    char res_path[999];
    int glsl_version; // 330 desktop
    
    // APP CFG
    int screen_resolution;
    bool screen_windowed;
    // RUNTIME CFG
    int screen_width;
    int screen_height;

    int fps_limit; // 0 = Unlimited
    bool fps_counter_show;

    bool postpro_antialias_msaa;
    bool postpro_effect_bloom;
    int postpro_texturefilter; // raylib.h:TextureFilter
} AppConfiguration;
