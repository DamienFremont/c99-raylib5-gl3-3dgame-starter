#include "eng_scene_json.h"

#include <stdio.h>

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

static bool IsStringEquals(const char *str1, const char *str2)
{
    return strcmp(str1, str2) == 0;
}

Color Parse_ColorJson(const cJSON *src)
{
    return (Color){
        cJSON_GetObjectItem(src, "r")->valueint,
        cJSON_GetObjectItem(src, "g")->valueint,
        cJSON_GetObjectItem(src, "b")->valueint,
        cJSON_GetObjectItem(src, "a")->valueint};
}

NodeMaterial Parse_MaterialJson(const cJSON *src)
{
    NodeMaterial tgt = (NodeMaterial){0};
    strcpy(tgt.albedo, cJSON_GetObjectItem(src, "albedo")->valuestring);
    return tgt;
}

Vector3 Transform_Axis(const char *axisCode)
{
    if (IsStringEquals("YAW", axisCode))
        return ROTATION_YAW;
    return (Vector3){0.0f, 0.0f, 0.0f};
}

Rotation2 Parse_Rotation2Json(const cJSON *src)
{
    return (Rotation2){
        Transform_Axis(cJSON_GetObjectItem(src, "axisCode")->valuestring),
        cJSON_GetObjectItem(src, "angle")->valueint};
}

Vector3 Parse_Vector3Json(const cJSON *src)
{
    return (Vector3){
        cJSON_GetObjectItem(src, "x")->valueint,
        cJSON_GetObjectItem(src, "y")->valueint,
        cJSON_GetObjectItem(src, "z")->valueint};
}

Transform2 Parse_Transform2Json(const cJSON *src)
{
    return (Transform2){
        Parse_Vector3Json(cJSON_GetObjectItem(src, "translation")),
        Parse_Rotation2Json(cJSON_GetObjectItem(src, "rotation")),
        Parse_Vector3Json(cJSON_GetObjectItem(src, "scale"))};
}

Node3D Parse_Node3dJson(const cJSON *json)
{
    Node3D node3d = (Node3D){0};
    strcpy(node3d.name, cJSON_GetObjectItem(json, "name")->valuestring);
    strcpy(node3d.model, cJSON_GetObjectItem(json, "model")->valuestring);
    node3d.transform = Parse_Transform2Json(cJSON_GetObjectItem(json, "transform"));
    // node3d.color = Parse_ColorJson(cJSON_GetObjectItem(json, "color")); // TODO: FIXME
    node3d.color = DARKGRAY;
    node3d.material = Parse_MaterialJson(cJSON_GetObjectItem(json, "material"));
    return node3d;
}

Scene Parse_SceneJson(const cJSON *json)
{
    Scene scene = (Scene){0};
    // read
    cJSON *array = cJSON_GetObjectItem(json, "nodes");
    int size = cJSON_GetArraySize(array);
    // check
    if (size > SCENE_NODES_SIZE)
    {
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
