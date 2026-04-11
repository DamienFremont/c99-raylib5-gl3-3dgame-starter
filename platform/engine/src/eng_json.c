#include "eng_json.h"
#include <stdio.h>
#include "eng_assets.h"

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

cJSON* ReadJsonFile(const char *fileName) {
    char path[PATH_MAX];
    GetAssetPath(path, fileName);
    // open the file
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }
    // read the file contents into a string
    char buffer[JSON_FILE_BUFFER_SIZE];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    // check buffer size
    if (len >= sizeof(buffer)) {
        printf("Error: Buffer overflow.\n");
        return 1;
    }
    fclose(fp);
    // parse the JSON data
    cJSON* json = cJSON_Parse(buffer);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        return 1;
    }
    return json;
}

