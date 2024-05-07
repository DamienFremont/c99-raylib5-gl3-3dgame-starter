#include <raylib.h>
#include <rlgl.h>
#include <stdbool.h>

#pragma once

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    FONT_SIZE_10 = 10,
    FONT_SIZE_12 = 12,
    FONT_SIZE_24 = 24,
    FONT_SIZE_48 = 48
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

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

void DrawTextCodepoint3D(Font font, int codepoint, Vector3 position, float fontSize, bool backface, Color tint);
void DrawText3D(Font font, const char *text, Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, Color tint);
