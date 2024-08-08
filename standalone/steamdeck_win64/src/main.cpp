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
std::string GetExecutionPath();

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, 
                     int nCmdShow)
{
    AppConfiguration cfg = InitConfig();
    strcpy(cfg.res_path, GetExecutionPath().c_str());
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
    appConfig.screen_resolution = STEAMDECK;
    appConfig.screen_windowed = false;
    appConfig.fps_limit = 30;
    appConfig.fps_counter_show = true;
    appConfig.postpro_antialias_msaa = false;
    appConfig.postpro_texturefilter = 1;
    appConfig.postpro_effect_bloom = true;
    return appConfig;
}

std::string GetExecutionPath()
{
    std::string str = _pgmptr;
    int exepath_len = str.size();
    std::string exebin = "\\steamdeck_win64.exe";
    int exebin_len = exebin.size();
    std::string str2 = str.substr(0, exepath_len - exebin_len);
    return str2;
}
