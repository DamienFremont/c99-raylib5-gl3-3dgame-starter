#include "console.h"

#include <stdbool.h>
#include <raylib.h>

void DrawConsole(ConsoleConfig cfg)
{
    const int LINE_HEIGHT = 20;
    const int LINE_1 = 0;
    const int LINE_2 = 1;
    const int LINES_COUNT = 4;

    if (cfg.showConsole == 1)
    {
        // Init
        const char line1[999] = "Console:";
        const char line2[999];
        snprintf(line2, sizeof(line2), "> %s", cfg.consoleOut);
        // Draw
        DrawRectangle(0, 0, cfg.screen_width, LINE_HEIGHT * LINES_COUNT, BLACK);
        DrawText(line1, 10, 20 + LINE_HEIGHT * LINE_1, 10, WHITE);
        DrawText(line2, 10, 20 + LINE_HEIGHT * LINE_2, 10, WHITE);
    }
    if (cfg.showFPS == true)
    {
        DrawFPS(cfg.screen_width - 100, 15);
    }
}