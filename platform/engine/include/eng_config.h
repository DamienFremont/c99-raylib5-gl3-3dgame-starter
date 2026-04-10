#pragma once
#include <stdbool.h>

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    APPNAME_STR_LENGTH = 999,
    PATH_STR_LENGTH = 1024,
} ConfigConst;

typedef struct AppConfiguration
{
    char appName[APPNAME_STR_LENGTH];

    char res_path[PATH_STR_LENGTH];
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
