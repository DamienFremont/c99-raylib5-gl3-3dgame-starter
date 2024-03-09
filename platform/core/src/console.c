#include <stdbool.h>
#include <raylib.h>

typedef struct ConsoleConfig ConsoleConfig;
struct ConsoleConfig
{
    int* showConsole;
    bool showFPS;
    int screen_width;
    char consoleOut[999];
};

void DrawConsole(ConsoleConfig cfg)
{
    if (cfg.showConsole == 1)
    {
        DrawRectangle(0, 9, cfg.screen_width, 30 * 3, Fade(BLACK, 0.7f));
        DrawText(cfg.consoleOut, 10, 20, 10, WHITE);
    }
    if (cfg.showFPS == true)
    {
        DrawFPS(cfg.screen_width - 100, 15);
    }
}