#include <windows.h>

#include <core.h>
#include <resolutions.h>

#include <stdio.h>
#include <stdlib.h>

#include <string>

#define GLSL_VERSION 330 // PLATFORM_DESKTOP

// const char *getRespath();
AppProperties initConfig();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
    return main(initConfig());
}

AppProperties initConfig()
{
    std::string str = _pgmptr;
    int exepath_len = str.size();
    std::string exebin = "\\desktop_win64.exe";
    int exebin_len = exebin.size();
    std::string str2 = str.substr(0, exepath_len - exebin_len);
    const char *cstr = str2.c_str();

    AppProperties appProperties;
    strcpy(appProperties.appName, "cpp20-raylib-starter");
    strcpy(appProperties.res_path, cstr);
    appProperties.glsl_version = GLSL_VERSION;
    appProperties.screen_width = getScreen(SD).width;
    appProperties.screen_height = getScreen(SD).height;
    appProperties.fps_limit = 0;
    appProperties.fps_counter_show = true;
    appProperties.postpro_msaa_enable = true;
    appProperties.postpro_bloom_enable = false;
    return appProperties;
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