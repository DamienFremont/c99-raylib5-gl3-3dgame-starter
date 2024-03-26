#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>

extern "C"
{
#include "resolutions.h"
#include "config.h"
#include "game.h"
}

#define GLSL_VERSION 330 // PLATFORM_DESKTOP

// const char *getRespath();
AppConfiguration initConfig();

typedef enum {
    TEXTURE_FILTER_POINT = 0,               // No filter, just pixel approximation
    TEXTURE_FILTER_BILINEAR,                // Linear filtering
    TEXTURE_FILTER_TRILINEAR,               // Trilinear filtering (linear with mipmaps)
    TEXTURE_FILTER_ANISOTROPIC_4X,          // Anisotropic filtering 4x
    TEXTURE_FILTER_ANISOTROPIC_8X,          // Anisotropic filtering 8x
    TEXTURE_FILTER_ANISOTROPIC_16X,         // Anisotropic filtering 16x
} TextureFilter;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
    return main(initConfig());
}

AppConfiguration initConfig()
{
    // getRespath
    std::string str = _pgmptr;
    int exepath_len = str.size();
    std::string exebin = "\\desktop_win64.exe";
    int exebin_len = exebin.size();
    std::string str2 = str.substr(0, exepath_len - exebin_len);
    const char *cstr = str2.c_str();
    // init config
    AppConfiguration appConfig;
    strcpy(appConfig.appName, "game-3d-c17-raylib5-gl3-starter");
    strcpy(appConfig.res_path, cstr);
    appConfig.glsl_version = GLSL_VERSION;
    appConfig.screen_width = getScreen(HD).width;
    appConfig.screen_height = getScreen(HD).height;
    appConfig.fps_limit = 0;
    appConfig.fps_counter_show = true;
    appConfig.postpro_msaa_enable = true;
    appConfig.postpro_texturefilter = TEXTURE_FILTER_ANISOTROPIC_4X;
    appConfig.postpro_bloom_enable = true;
    return appConfig;
}
