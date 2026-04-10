#include <cjson.h>

#pragma once

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

cJSON* Read_JsonFile(const char *fileName);
