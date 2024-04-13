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

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

#define WINDOW_TITLE "game-3d-c17-raylib5-gl3-starter"
#define EXEC_FILENAME "\\desktop_win64.exe" // PLATFORM_DESKTOP_WIN64
#define GLSL_VERSION 330 // PLATFORM_DESKTOP
// #define GRAPHICS_API_OPENGL_ES2
// #define PLATFORM_DESKTOP_SDL
#define TEXTURE_FILTER_ANISOTROPIC_4X 3

//---------------------------------------------------------
// Local Functions Declaration
//---------------------------------------------------------

AppConfiguration InitConfig();
char* GetExecutionPath();

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, 
                     int nCmdShow)
{
    AppConfiguration cfg = InitConfig();
    strcpy(cfg.res_path, GetExecutionPath());
    return main(cfg);
}

//---------------------------------------------------------
// Local Functions Definition
//---------------------------------------------------------

AppConfiguration InitConfig()
{
    AppConfiguration appConfig = { 0 };
    strcpy(appConfig.appName, WINDOW_TITLE);
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

char* GetExecutionPath()
{
    char res[999];
    std::string str = _pgmptr;
    int exepath_len = str.size();
    std::string exebin = EXEC_FILENAME;
    int exebin_len = exebin.size();
    std::string str2 = str.substr(0, exepath_len - exebin_len);
    strcpy(res, str2.c_str());
    return res;
}
