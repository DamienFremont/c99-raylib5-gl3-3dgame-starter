#include "eng_screen.h"
#include <raylib.h>

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef struct ScreenResolution
{
    char name[8];
    int width;
    int height;
} ScreenResolution;

//---------------------------------------------------------
// Local Functions Declaration
//---------------------------------------------------------

ScreenResolution getScreen(ScreenResolutionType type);

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

int ENG_GetScreenWidth(ScreenResolutionType type)
{
    return getScreen(type).width;
}

int ENG_GetScreenHeight(ScreenResolutionType type)
{
    return getScreen(type).height;
}

//---------------------------------------------------------
// Local Functions Definition
//---------------------------------------------------------

ScreenResolution getScreen(ScreenResolutionType type)
{
    ScreenResolution reso[7] = {0};
    reso[NATIVE] = (ScreenResolution){"Native", GetScreenWidth(), GetScreenHeight()};
    reso[CGA] = (ScreenResolution){"CGA", 320, 200};
    reso[SD] = (ScreenResolution){"SD", 854, 480};
    reso[HD] = (ScreenResolution){"HD", 1280, 720};
    reso[FHD] = (ScreenResolution){"FHD", 1920, 1080};
    reso[QHD] = (ScreenResolution){"QHD", 2560, 1440};
    reso[UHD] = (ScreenResolution){"UHD", 3840, 2160};
    return reso[type];
}
