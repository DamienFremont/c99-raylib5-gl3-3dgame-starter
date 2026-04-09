#include "eng_scene.h"
#include "eng_assets.h"

#include <stdio.h>

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

Color Parse_ColorJson(const cJSON* src) {
    int r = cJSON_GetObjectItemCaseSensitive(src, "r")->valueint;
    return (Color) {
        r,
        cJSON_GetObjectItemCaseSensitive(src, "g")->valueint,
        cJSON_GetObjectItemCaseSensitive(src, "b")->valueint,
        cJSON_GetObjectItemCaseSensitive(src, "a")->valueint
    };
}

NodeMaterial Parse_MaterialJson(const cJSON* src) {
    NodeMaterial tgt = (NodeMaterial){ 0 };
    strcpy(tgt.albedo, cJSON_GetObjectItem(src, "albedo")->valuestring);
    return tgt;
}

Transform2 Parse_Transform2Json(const cJSON *src) {
        cJSON* translation = cJSON_GetObjectItemCaseSensitive(src, "translation");
        //cJSON* rotation = cJSON_GetObjectItemCaseSensitive(src, "rotation");
        cJSON* scale = cJSON_GetObjectItemCaseSensitive(src, "scale");
        Transform2 tgt = (Transform2){
                (Vector3) {
                    cJSON_GetObjectItemCaseSensitive(translation, "x")->valueint,
                    cJSON_GetObjectItemCaseSensitive(translation, "y")->valueint,
                    cJSON_GetObjectItemCaseSensitive(translation, "z")->valueint
                },
                (Rotation2){
                    { 0.0f, 0.0f, 0.0f }, // TODO
                    ROTATE_ZERO}, // TODO
                (Vector3){
                    cJSON_GetObjectItemCaseSensitive(scale, "x")->valueint,
                    cJSON_GetObjectItemCaseSensitive(scale, "y")->valueint,
                    cJSON_GetObjectItemCaseSensitive(scale, "z")->valueint
                }
        };
        return tgt;
}

Node3D Parse_Node3dJson(const cJSON *json) {
    Node3D node3d = (Node3D) {0};
        strcpy(node3d.name, cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring);
        strcpy(node3d.model, cJSON_GetObjectItemCaseSensitive(json, "model")->valuestring);
        node3d.transform = Parse_Transform2Json(cJSON_GetObjectItemCaseSensitive(json, "transform"));
        //node3d.color = Parse_ColorJson(cJSON_GetObjectItemCaseSensitive(json, "color")); // TODO: FIXME
        node3d.color = DARKGRAY;
        node3d.material = Parse_MaterialJson(cJSON_GetObjectItemCaseSensitive(json, "material"));
        return node3d;
}

cJSON* Read_SceneJsonFile(const char *fileName) {
    char tmp[PATH_MAX];
    char* path = GetAssetPath(tmp, fileName);
    // open the file
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }
    // read the file contents into a string
    char buffer[SCENE_JSON_FILE_BUFFER_SIZE];
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

Scene Parse_SceneJson(const cJSON * json) {
    Node3D nodes[SCENE_NODES_SIZE] = { 0 };
    cJSON* array = cJSON_GetObjectItemCaseSensitive(json,"nodes");
    int size = cJSON_GetArraySize(array);
    cJSON* item = NULL;
    for (int i = 0; i < size; i++)
    {
        item = cJSON_GetArrayItem(array, i);
        nodes[i] = Parse_Node3dJson(item);
     }
    Scene scene = (Scene){0};
    scene.nodes = nodes;
    scene.nodesSize = size;
    return scene;
}
