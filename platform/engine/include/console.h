#include <stdbool.h>

#pragma once

typedef struct ConsoleConfig
{
    int *showConsole;
    bool showFPS;
    int screen_width;
} ConsoleConfig;

void LogConsole(char* str);

void DrawConsole(ConsoleConfig cfg);
