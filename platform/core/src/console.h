#include <stdbool.h>

#pragma once

typedef struct ConsoleConfig ConsoleConfig;
struct ConsoleConfig
{
    int *showConsole;
    bool showFPS;
    int screen_width;
    char consoleOut[999];
};

void DrawConsole(ConsoleConfig cfg);