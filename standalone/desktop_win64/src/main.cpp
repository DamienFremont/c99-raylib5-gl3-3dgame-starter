#include <windows.h>

#include <core.h>
#include <resolutions.h>

#include <stdio.h>
#include <stdlib.h>

#include <string>

#define GLSL_VERSION 330 // PLATFORM_DESKTOP

// const char *getRespath();
AppConfiguration initConfig();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
    return main(initConfig());
}

AppConfiguration initConfig()
{
    std::string str = _pgmptr;
    int exepath_len = str.size();
    std::string exebin = "\\desktop_win64.exe";
    int exebin_len = exebin.size();
    std::string str2 = str.substr(0, exepath_len - exebin_len);
    const char *cstr = str2.c_str();

    AppConfiguration appConfig;
    strcpy(appConfig.appName, "cpp20-raylib-starter");
    strcpy(appConfig.res_path, cstr);
    appConfig.glsl_version = GLSL_VERSION;
    appConfig.screen_width = getScreen(SD).width;
    appConfig.screen_height = getScreen(SD).height;
    appConfig.fps_limit = 60;
    appConfig.fps_counter_show = true;
    appConfig.postpro_msaa_enable = true;
    appConfig.postpro_bloom_enable = false;
    return appConfig;
}

// const char* getRespath() {
//     std::string str = _pgmptr;
//     int exepath_len = str.size();
//     std::string exebin = "\\desktop_win64.exe";
//     int exebin_len = exebin.size();
//     std::string str2 = str.substr(0, exepath_len - exebin_len);
//     const char *cstr = str2.c_str();
//     return str2.c_str();
// }