#include <raylib.h>
#include <rlgl.h>
#include <stdbool.h>

#pragma once

typedef enum
{
    FONT_SIZE_12 = 12
} FontSize;
typedef enum
{
    FONT_SPACING_1 = 1
} FontSpacing;

typedef enum
{
    FONT_LINE_SPACING_1 = 0
} FontSpacing;

typedef enum
{
    BACKFACE_FALSE = 0
} BackFace;

void DrawTextCodepoint3D(Font font, int codepoint, Vector3 position, float fontSize, bool backface, Color tint);

void DrawText3D(Font font, const char *text, Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, Color tint);
