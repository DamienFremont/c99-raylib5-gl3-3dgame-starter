#include "eng_scene_json.h"

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
        cJSON* rotation = cJSON_GetObjectItemCaseSensitive(src, "rotation");
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

Scene Parse_SceneJson(const cJSON* json)
{
    Scene scene = (Scene){0};
    // read
    cJSON *array = cJSON_GetObjectItemCaseSensitive(json, "nodes");
    int size = cJSON_GetArraySize(array);
    // check
    if(size > SCENE_NODES_SIZE) {
        printf("Error: Unable to open the file.\n");
        return scene;
    }
    // parse
    Node3D nodes[SCENE_NODES_SIZE] = {0};
    cJSON *node = NULL;
    for (int i = 0; i < size; i++)
    {
        node = cJSON_GetArrayItem(array, i);
        nodes[i] = Parse_Node3dJson(node);
    }
    scene.nodes = nodes;
    scene.nodesSize = size;
    return scene;
}
