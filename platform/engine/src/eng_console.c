#include "eng_console.h"

#include <stdbool.h>
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include "eng_text.h"

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

static char consoleOut[CONSOLE_STR_LENGTH];

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

void LogConsole(const char *str)
{
    strcpy_s(consoleOut, sizeof(consoleOut), str);
}

void DrawConsole(void)
{
    // Init
    char line1[CONSOLE_STR_LENGTH] = "Console:";
    char line2[CONSOLE_STR_LENGTH];
    snprintf(line2, sizeof(line2), "> %s", consoleOut);
    // Draw 3D
    DrawGrid(50, 1.0f);
    DrawCubeWiresV((Vector3){9.0f, 1.0f, 11.0f}, (Vector3){1.0f, 2.0f, 1.0f}, RED);
    // Draw 2D
    int LINE_HEIGHT_20 = 20;
    DrawRectangle(0, 0, GetScreenWidth(), LINE_HEIGHT_20 * 4, BLACK);
    DrawText(line1, 10, 20 + LINE_HEIGHT_20 * 1, FONT_SIZE_10, WHITE);
    DrawText(line2, 10, 20 + LINE_HEIGHT_20 * 2, FONT_SIZE_10, WHITE);
}
