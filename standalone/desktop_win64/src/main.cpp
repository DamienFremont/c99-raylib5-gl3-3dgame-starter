#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>

extern "C"
{
#include "gam_game.h"
#include "eng_screen.h"
#include "eng_config.h"
}

#define GLSL_VERSION 330 // PLATFORM_DESKTOP
#define TEXTURE_FILTER_ANISOTROPIC_4X 3

// const char *getRespath();
AppConfiguration initConfig();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
    AppConfiguration cfg = initConfig();
    return main(cfg);
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
    appConfig.screen_width = ENG_GetScreenWidth(HD);
    appConfig.screen_height = ENG_GetScreenHeight(HD);
    appConfig.fps_limit = 0;
    appConfig.fps_counter_show = true;
    appConfig.postpro_antialias_msaa = true;
    appConfig.postpro_texturefilter = TEXTURE_FILTER_ANISOTROPIC_4X;
    appConfig.postpro_effect_bloom = true;
    return appConfig;
}
