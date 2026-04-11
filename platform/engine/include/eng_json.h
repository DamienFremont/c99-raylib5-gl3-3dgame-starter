#pragma once
#include <cjson.h>

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    JSON_FILE_BUFFER_SIZE = 128 * 1024
} JsonConst;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

cJSON* ReadJsonFile(const char *fileName);
