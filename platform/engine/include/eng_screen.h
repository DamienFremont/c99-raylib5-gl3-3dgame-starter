#pragma once

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    NATIVE = 0,
    CUSTOM = 1,
    CGA,
    SD,
    HD,
    FHD,
    QHD,
    UHD,
    STEAMDECK,
} ScreenResolutionType;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

int GetScreenWidthByResolutionTYpe(ScreenResolutionType type);
int GetScreenHeightByResolutionType(ScreenResolutionType type);
