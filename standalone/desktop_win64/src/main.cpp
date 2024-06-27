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
    strcpy(appConfig.appName, "c99-raylib5-gl3-3dgame-starter");
    appConfig.glsl_version = 330;
    appConfig.screen_resolution = HD;
    appConfig.screen_windowed = true;
    appConfig.fps_limit = 0;
    appConfig.fps_counter_show = true;
    appConfig.postpro_antialias_msaa = true;
    appConfig.postpro_texturefilter = 3;
    appConfig.postpro_effect_bloom = true;
    return appConfig;
}

char* GetExecutionPath()
{
    char res[999];
    std::string str = _pgmptr;
    int exepath_len = str.size();
    std::string exebin = "\\desktop_win64.exe";
    int exebin_len = exebin.size();
    std::string str2 = str.substr(0, exepath_len - exebin_len);
    strcpy(res, str2.c_str());
    return res;
}
