#include "eng_scene_json.h"
#include <stdio.h>
#include <string.h>
#include "eng_material.h"

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

// DEPRECATED TODO: REMOVE ME
 Color Parse_ColorJson(const cJSON *src)
 {
     return (Color){
         cJSON_GetObjectItem(src, "r")->valueint,
         cJSON_GetObjectItem(src, "g")->valueint,
         cJSON_GetObjectItem(src, "b")->valueint,
         cJSON_GetObjectItem(src, "a")->valueint};
 }

Color Transform_Color(const char *src)
{
    if (strcmp("BLUE", src) == 0)
        return BLUE;
    if (strcmp("GRAY", src) == 0)
        return GRAY;
    if (strcmp("DARKGRAY", src) == 0)
        return DARKGRAY;
    return GRAY;
}

Vector3 Transform_Axis(const char *axis)
{
    if (strcmp("YAW", axis) == 0)
        return AXIS_YAW;
    return (Vector3){0.0f, 0.0f, 0.0f};
}

Rotation2 Parse_Rotation2Json(const cJSON *src)
{
    if(!cJSON_HasObjectItem(src, "rotation"))
        return (Rotation2){ AXIS_YAW, 0 };
    cJSON *json = cJSON_GetObjectItem(src, "rotation");
    return (Rotation2){
        Transform_Axis(cJSON_GetObjectItem(json, "axis")->valuestring),
        cJSON_GetObjectItem(json, "angle")->valueint};
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
        Parse_Rotation2Json(src),
        Parse_Vector3Json(cJSON_GetObjectItem(src, "scale"))};
}

Node3D Parse_Node3dJson(const cJSON *json)
{
    Node3D node3d = (Node3D){0};
    node3d.id = cJSON_GetObjectItem(json, "id")->valueint;
    strcpy_s(node3d.name, sizeof(node3d.name), cJSON_GetObjectItem(json, "name")->valuestring);
    strcpy_s(node3d.model, sizeof(node3d.model), cJSON_GetObjectItem(json, "model")->valuestring);
    node3d.transform = Parse_Transform2Json(cJSON_GetObjectItem(json, "transform"));
    node3d.color = Transform_Color(cJSON_GetObjectItem(json, "color")->valuestring);
    if(cJSON_HasObjectItem(json, "texture"))
        strcpy_s(node3d.texture, sizeof(node3d.texture), cJSON_GetObjectItem(json, "texture")->valuestring);
    return node3d;
}

Scene ParseSceneJson(const cJSON *json)
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
    for (int i = 0; i < size; i++)
        nodes[i] = Parse_Node3dJson(cJSON_GetArrayItem(array, i));
    scene.nodes = nodes;
    scene.nodesSize = size;
    return scene;
}
