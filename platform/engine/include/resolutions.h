#pragma once

struct ScreenResolution
{
    char name[4];
    int width;
    int height;
};

typedef enum
{
    SD = 0,
    HD,
    FHD,
    QHD,
    UHD,
} ScreenResolutionType;

ScreenResolution getScreen(ScreenResolutionType type)
{
    ScreenResolution reso[5] = {0};
    reso[SD] = ScreenResolution("SD", 720, 480);
    reso[HD] = ScreenResolution("HD", 1280, 720);
    reso[FHD] = ScreenResolution("FHD", 1920, 1080);
    reso[QHD] = ScreenResolution("QHD", 2560, 1440);
    reso[UHD] = ScreenResolution("UHD", 3840, 2160);
    return reso[type];
}