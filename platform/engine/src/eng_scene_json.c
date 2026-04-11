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

Texture2D Transform_Texture(const char *src, Vector3 scale)
{
    if (strcmp("CHECKBOARD", src) == 0) {
        return CheckboardTexture2D(
            scale.x,
            scale.y,
            scale.z);
    }
    return LoadTextureFromImage(GenImageColor(16,16,GRAY));
}

Vector3 Transform_Axis(const char *axis)
{
    if (strcmp("YAW", axis) == 0)
        return ROTATION_YAW;
    return (Vector3){0.0f, 0.0f, 0.0f};
}

Rotation2 Parse_Rotation2Json(const cJSON *src)
{
    return (Rotation2){
        Transform_Axis(cJSON_GetObjectItem(src, "axis")->valuestring),
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
    strcpy_s(node3d.name, sizeof(node3d.name), cJSON_GetObjectItem(json, "name")->valuestring);
    strcpy_s(node3d.model, sizeof(node3d.model), cJSON_GetObjectItem(json, "model")->valuestring);
    node3d.transform = Parse_Transform2Json(cJSON_GetObjectItem(json, "transform"));
    node3d.color = Transform_Color(cJSON_GetObjectItem(json, "color")->valuestring);
    node3d.texture = Transform_Texture(cJSON_GetObjectItem(json, "texture")->valuestring, node3d.transform.scale);
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
