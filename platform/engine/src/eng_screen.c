#include "eng_screen.h"
#include <raylib.h>

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef struct ScreenResolution
{
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
    ScreenResolution reso[16] = {0};
    reso[NATIVE] = (ScreenResolution){GetMonitorWidth(0), GetMonitorHeight(0)};
    // reso[CUSTOM] = ;
    reso[CGA] = (ScreenResolution){320, 200};
    reso[SD] = (ScreenResolution){854, 480};
    reso[HD] = (ScreenResolution){1280, 720};
    reso[FHD] = (ScreenResolution){1920, 1080};
    reso[QHD] = (ScreenResolution){2560, 1440};
    reso[UHD] = (ScreenResolution){3840, 2160};
    reso[STEAMDECK] = (ScreenResolution){1280, 800};
    return reso[type];
}
