#pragma once
#include <stdbool.h>

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    CONSOLE_STR_LENGTH = 999,
} ConsoleConst;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

void LogConsole(const char* str);
void DrawConsole();
